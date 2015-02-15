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

#include "clang/Tooling/Tooling.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Tooling/CompilationDatabase.h"
#include "clang/Frontend/CompilerInstance.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/raw_ostream.h"

#include <string>
#include <vector>
#include <iostream>

using namespace clang;
using namespace clang::tooling;
using namespace llvm;

// Set up the command line options

static cl::list<std::string> SourcePaths(
  cl::Positional,
  cl::desc("<source> [... <sourceN>]"),
  cl::OneOrMore);

std::vector<std::string> ExcludeFunctionList;

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
	cl::location( ExcludeFileList ));

static cl::opt<bool> DumpTokens(
  "dump-tokens",
  cl::desc("Dump tokens as they are lexed")
);

static cl::opt<bool> NoGlobal(
  "disable-global",
  cl::desc("Disable output of stats at the global level")
);

static cl::opt<bool> NoFile(
  "disable-file",
  cl::desc("Disable output of stats at the file level")
);

static cl::opt<bool> NoFunction(
  "disable-function",
  cl::desc("Disable output of stats at the function level")
);

static cl::opt<bool> NoMethod(
  "disable-method",
  cl::desc("Disable output of stats at the method level")
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
	cl::init(METRIC_DUMP_FORMAT_TREE)
);

std::vector<std::string> OutputMetricList;

static cl::list<std::string, std::vector<std::string>> OutputOnlyMetrics(
	"output-metrics",
    cl::desc("Only output the specified metrics"),
	cl::CommaSeparated,
	cl::ZeroOrMore,
	cl::location( OutputMetricList ));

int main(int argc, const char **argv) {
	MetricUnit topUnit( NULL, "Global", METRIC_UNIT_GLOBAL);
	MetricOptions options( &ExcludeFileList, &ExcludeFunctionList, &OutputMetricList );
	std::set<std::string> commentFileList;
	SrcStartToFunctionMap_t srcMap;

	llvm::sys::PrintStackTraceOnErrorSignal();
	std::unique_ptr<CompilationDatabase> Compilations(
			FixedCompilationDatabase::loadFromCommandLine(argc, argv));
	cl::ParseCommandLineOptions(argc, argv);

	options.setDumpTokens( DumpTokens );

	if (!Compilations) {  // Couldn't find a compilation DB from the command line
		std::string ErrorMessage;
		Compilations= 
	#if 0
		  !BuildPath.empty() ?
			CompilationDatabase::autoDetectFromDirectory(BuildPath, ErrorMessage) :
	#endif
			CompilationDatabase::autoDetectFromSource(SourcePaths[0], ErrorMessage);
		  
		//  Still no compilation DB? - bail.	
		if (!Compilations)
		{
		    llvm::report_fatal_error(ErrorMessage);
		}
	}

	ClangTool Tool(*Compilations, SourcePaths);
  
	// First tool-run to gather metrics from the AST.  This is done separately from the second tool-rum
	// as different pre-processor options are used
	int Result = Tool.run(newASTMetricFrontendActionFactory(&options, &topUnit, &srcMap, &commentFileList));

	// Success?
	if( Result == 0 )
	{
		// Second tool run to gather metrics from the pre-processor.  This is performed after the AST
		//  generation as the details of the function locations gathered from the AST are used
		//  for determining whether or not a function should be included
		Result = Tool.run(newPPMetricFrontendActionFactory(&options, &topUnit, &srcMap));

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

			topUnit.dump( std::cout, output, OutputFormat, &options );
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
