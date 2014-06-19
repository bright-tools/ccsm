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
#include "MetricASTConsumer.hpp"
#include "MetricPPCustomer.hpp"

#include "clang/Tooling/Tooling.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/FrontendActions.h"
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

static cl::opt<std::string> OutputFormat(
	"output-format",
	cl::desc("Format of output - choose from tree, sparsetree, csv or tsv"),
	cl::value_desc("fmt-string"),
	cl::init("tree")
);

std::vector<std::string> OutputMetricList;

static cl::list<std::string, std::vector<std::string>> OutputOnlyMetrics(
	"output-metrics",
    cl::desc("Only output the specified metrics"),
	cl::CommaSeparated,
	cl::ZeroOrMore,
	cl::location( OutputMetricList ));

MetricUnit topUnit( NULL, "Global", METRIC_UNIT_GLOBAL);
MetricOptions options( &ExcludeFileList, &ExcludeFunctionList, &OutputMetricList );
std::set<std::string> commentFileList;

class MetricFrontendAction : public ASTFrontendAction {
public:
    virtual ASTConsumer *CreateASTConsumer(CompilerInstance &CI, StringRef file) {
		
		// TODO: More elegant way of getting topUnit, options & commentFileList in.
		MetricASTConsumer* ret_val = new MetricASTConsumer(&CI.getASTContext(),&topUnit,&options); // pass CI pointer to ASTConsumer
		MetricPPCustomer* customer = new MetricPPCustomer( &topUnit, &commentFileList, &options );
		CI.getPreprocessor().addCommentHandler(customer);
		CI.getPreprocessor().addPPCallbacks(customer);
		return ret_val;
    }
};

int main(int argc, const char **argv) {
  llvm::sys::PrintStackTraceOnErrorSignal();
  llvm::OwningPtr<CompilationDatabase> Compilations(
        FixedCompilationDatabase::loadFromCommandLine(argc, argv));
  cl::ParseCommandLineOptions(argc, argv);

  if (!Compilations) {  // Couldn't find a compilation DB from the command line
    std::string ErrorMessage;
    Compilations.reset(
#if 0
      !BuildPath.empty() ?
        CompilationDatabase::autoDetectFromDirectory(BuildPath, ErrorMessage) :
#endif
        CompilationDatabase::autoDetectFromSource(SourcePaths[0], ErrorMessage)
      );

    //  Still no compilation DB? - bail.
    if (!Compilations)
    {
      llvm::report_fatal_error(ErrorMessage);
    }
  }
  ClangTool Tool(*Compilations, SourcePaths);

  int Result = Tool.run(newFrontendActionFactory<MetricFrontendAction>());

	if( Result == 0 )
	{
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
		MetricDumpFormat_e fmt;
		// TODO: Case sensitivity?  Detect invalid option?
		if( OutputFormat == "tsv" )
		{
			fmt = METRIC_DUMP_FORMAT_TSV;
		} else if( OutputFormat == "csv" )
		{
			fmt = METRIC_DUMP_FORMAT_CSV;
		} else if( OutputFormat == "sparsetree" )
		{
			fmt = METRIC_DUMP_FORMAT_SPARSE_TREE;
		} else
		{
			fmt = METRIC_DUMP_FORMAT_TREE;
		}
		topUnit.dump( std::cout, output, fmt );
	}
	else
	{
		std::cout << "Tool.run returned " << Result << std::endl;
	}

  return Result;
}
