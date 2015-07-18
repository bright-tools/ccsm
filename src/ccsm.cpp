/*

   Copyright 2014 John Bailey

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "MetricMatcher.hpp"
#include "MetricOptions.hpp"
#include "MetricFrontendActors.hpp"
#include "FunctionLocator.hpp"
#include "MetricLinkageResolver.hpp"
#include "MetricDumper.hpp"

#include "clang/Tooling/Tooling.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Tooling/CompilationDatabase.h"
#include "clang/Frontend/CompilerInstance.h"
#include "llvm/Support/CommandLine.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/raw_ostream.h"

#include <string>
#include <vector>
#include <iostream>

using namespace clang;
using namespace clang::tooling;
using namespace llvm;

// Set up the command line options

std::vector<std::string> ExcludeFunctionList;

static llvm::cl::OptionCategory CCSMToolCategory("ccsm options");

static cl::list<std::string, std::vector<std::string>> ExcludeFunctions(
	"exclude-function",
    cl::desc("Exclude specified function from the metrics"),
	cl::CommaSeparated,
	cl::ZeroOrMore,
	cl::location( ExcludeFunctionList ));

std::vector<std::string> ExcludeFileList;

static cl::list<std::string, std::vector<std::string>> ExcludeFiles(
	"exclude-file",
    cl::desc("Exclude specified file from the metrics"),
	cl::CommaSeparated,
	cl::ZeroOrMore,
	cl::location( ExcludeFileList ),
	cl::cat(CCSMToolCategory));

std::vector<std::string> IncludeInParentFileList;

static cl::list<std::string, std::vector<std::string>> IncludeInParentFiles(
	"def-file",
    cl::desc("Specifies files whose contents should not be individually counted and should be included in the including file.  Intended to help support the def file idiom"),
	cl::CommaSeparated,
	cl::ZeroOrMore,
	cl::location(IncludeInParentFileList),
	cl::cat(CCSMToolCategory));

static cl::opt<bool> UseShortNames(
	"output-short-names",
	cl::desc("Use short metric names in the output"),
	cl::init(false),
	cl::cat(CCSMToolCategory)
	);

static cl::opt<bool> DumpTokens(
  "dump-tokens",
  cl::desc("Dump tokens as they are lexed"),
  cl::init(false),
  cl::cat(CCSMToolCategory)
);

static cl::opt<bool> DumpAST(
	"dump-ast",
	cl::desc("Dump AST when it is processed"),
	cl::init(false),
	cl::cat(CCSMToolCategory)
);

static cl::opt<bool> DumpFnMap(
  "dump-function-map",
  cl::desc("Dump the mapping of files/functions/locations"),
  cl::init(false),
  cl::cat(CCSMToolCategory)
);

static cl::opt<bool> NoGlobal(
  "disable-global",
  cl::desc("Disable output of stats at the global level"),
  cl::init(false),
  cl::cat(CCSMToolCategory)
);

static cl::opt<bool> NoFile(
  "disable-file",
  cl::desc("Disable output of stats at the file level"),
  cl::init(false),
  cl::cat(CCSMToolCategory)
);

static cl::opt<bool> NoFunction(
  "disable-function",
  cl::desc("Disable output of stats at the function level"),
  cl::init(false),
  cl::cat(CCSMToolCategory)
);

static cl::opt<bool> NoMethod(
  "disable-method",
  cl::desc("Disable output of stats at the method level"),
  cl::init(false),
  cl::cat(CCSMToolCategory)
);

static cl::opt<MetricDumpFormat_e> OutputFormat(
	"output-format",
	cl::desc("Format of output"),
	cl::values(
     clEnumValN(METRIC_DUMP_FORMAT_TSV,         "tsv",        "Tab Seperated Values"),
     clEnumValN(METRIC_DUMP_FORMAT_CSV,         "csv",        "Comma Separated Values"),
     clEnumValN(METRIC_DUMP_FORMAT_SPARSE_TREE, "sparsetree", "Sparse Tree (zero value nodes omitted)"),
     clEnumValN(METRIC_DUMP_FORMAT_TREE,        "tree",       "Tree Structure"),
    clEnumValEnd),
	cl::init(METRIC_DUMP_FORMAT_TREE),
	cl::cat(CCSMToolCategory)
);

static cl::opt<bool> PrototypesAreFileScope(
	"prototypes-are-filescope",
	cl::desc("The prototype part of a function declaration should be included in the file scope metrics, not the function scope"),
	cl::init(false),
	cl::cat(CCSMToolCategory)
	);

std::vector<std::string> OutputMetricList;

static cl::list<std::string, std::vector<std::string>> OutputOnlyMetrics(
	"output-metrics",
    cl::desc("Only output the specified metrics"),
	cl::CommaSeparated,
	cl::ZeroOrMore,
	cl::location(OutputMetricList),
	cl::cat(CCSMToolCategory));

int main(int argc, const char **argv) {
	MetricUnit topUnit( NULL, "Global", METRIC_UNIT_GLOBAL);
	MetricOptions options( &ExcludeFileList, &ExcludeFunctionList, &OutputMetricList, &IncludeInParentFileList );
	std::set<std::string> commentFileList;
	GlobalFunctionLocator srcMap( options );

	llvm::sys::PrintStackTraceOnErrorSignal();
	CommonOptionsParser OptionsParser(argc, argv, CCSMToolCategory);

	options.setDumpTokens( DumpTokens );
	options.setDumpAST(DumpAST);
	options.setUseShortNames(UseShortNames);
	options.setPrototypesAreFileScope(PrototypesAreFileScope);

	ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());

	// First tool-run to gather metrics from the AST.  This is done separately from the second tool-rum
	// as different pre-processor options are used
	int Result = Tool.run(newASTMetricFrontendActionFactory(&options, &topUnit, &srcMap, &commentFileList));

	// Success?
	if( Result == 0 )
	{
		// Now that all TUs have been processed, try to resolve function references which couldn't be 
		//  resolved during the processing of individual translation units
		resolveLinkages( &topUnit );

		// Second tool run to gather metrics from the pre-processor.  This is performed after the AST
		//  generation as the details of the function locations gathered from the AST are used
		//  for determining whether or not a function should be included
		Result = Tool.run(newPPMetricFrontendActionFactory(&options, &topUnit, &srcMap, true));

		if (Result == 0)
		{
			Result = Tool.run(newPPMetricFrontendActionFactory(&options, &topUnit, &srcMap, false));
		}

		// Success?
		if( Result == 0 )
		{
			// Time to dump the results

			bool output[ METRIC_UNIT_MAX ] = { 0, };
			if( !NoMethod ) {
				output[ METRIC_UNIT_METHOD ] = true;
			}
			if( !NoFunction ) {
				output[ METRIC_UNIT_FUNCTION ] = true;
			}
			if( !NoFile ) {
				output[ METRIC_UNIT_FILE ] = true;
			}
			if( !NoGlobal ) {
				output[ METRIC_UNIT_GLOBAL ] = true;
			}

			if( DumpFnMap )
			{
				srcMap.dump( std::cout );
			}

			MetricDumper::dump(std::cout, &topUnit, output, OutputFormat, &options);
		}
		else
		{
			std::cout << "Tool.run returned " << Result << std::endl;
		}
	}
	else
	{
		std::cout << "Tool.run returned " << Result << std::endl;
	}

	return Result;
}
