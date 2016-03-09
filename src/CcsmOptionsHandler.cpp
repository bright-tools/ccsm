/*

   Copyright 2015 John Bailey

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

#include "CcsmOptionsHandler.hpp"
#include "ccsm_ver.h"
#include "MetricUnit.hpp"

#include "llvm/Support/CommandLine.h"
#include "clang/Basic/Version.h"

#include <iostream>

// Set up the command line options
static llvm::cl::OptionCategory CCSMToolCategory("ccsm options");

static std::vector<std::string> ExcludeFunctionList;

static llvm::cl::list<std::string, std::vector<std::string>> ExcludeFunctions(
	"exclude-function",
	llvm::cl::desc("Exclude specified function from the metrics"),
	llvm::cl::CommaSeparated,
	llvm::cl::ZeroOrMore,
	llvm::cl::location(ExcludeFunctionList),
	llvm::cl::cat(CCSMToolCategory));

static std::vector<std::string> ExcludeFileList;

static llvm::cl::list<std::string, std::vector<std::string>> ExcludeFiles(
	"exclude-file",
	llvm::cl::desc("Exclude specified file from the metrics"),
	llvm::cl::CommaSeparated,
	llvm::cl::ZeroOrMore,
	llvm::cl::location(ExcludeFileList),
	llvm::cl::cat(CCSMToolCategory));

static std::vector<std::string> IncludeInParentFileList;

static llvm::cl::list<std::string, std::vector<std::string>> IncludeInParentFiles(
	"def-file",
	llvm::cl::desc("Specifies files whose contents should not be individually counted and should be included in the including file.  Intended to help support the def file idiom"),
	llvm::cl::CommaSeparated, 
	llvm::cl::ZeroOrMore,
	llvm::cl::location(IncludeInParentFileList),
	llvm::cl::cat(CCSMToolCategory));

static llvm::cl::opt<bool> UseShortNames(
	"output-short-names",
	llvm::cl::desc("Use short metric names in the output"),
	llvm::cl::init(false),
	llvm::cl::cat(CCSMToolCategory)
	);

static llvm::cl::opt<bool> UseAbsoluteFileNames(
	"output-absolute-fn",
	llvm::cl::desc("Use absolute filenames in the output"),
	llvm::cl::init(false),
	llvm::cl::cat(CCSMToolCategory)
	);

static llvm::cl::opt<bool> ExcludeStandardHeaders(
	"exclude-std-headers",
	llvm::cl::desc("Exclude standard library headers"),
	llvm::cl::init(false),
	llvm::cl::cat(CCSMToolCategory)
	);

static llvm::cl::opt<bool> DumpTokens(
	"dump-tokens",
	llvm::cl::desc("Dump tokens as they are lexed"),
	llvm::cl::init(false),
	llvm::cl::cat(CCSMToolCategory)
	);

static llvm::cl::opt<bool> DumpAST(
	"dump-ast",
	llvm::cl::desc("Dump AST when it is processed"),
	llvm::cl::init(false),
	llvm::cl::cat(CCSMToolCategory)
	);

static llvm::cl::opt<bool> DumpFnMap(
	"dump-function-map",
	llvm::cl::desc("Dump the mapping of files/functions/locations"),
	llvm::cl::init(false),
	llvm::cl::cat(CCSMToolCategory)
	);

static llvm::cl::opt<bool> NoGlobal(
	"disable-global",
	llvm::cl::desc("Disable output of stats at the global level"),
	llvm::cl::init(false),
	llvm::cl::cat(CCSMToolCategory)
	);

static llvm::cl::opt<bool> NoFile(
	"disable-file",
	llvm::cl::desc("Disable output of stats at the file level"),
	llvm::cl::init(false),
	llvm::cl::cat(CCSMToolCategory)
	);

static llvm::cl::opt<bool> NoFunction(
	"disable-function",
	llvm::cl::desc("Disable output of stats at the function level"),
	llvm::cl::init(false),
	llvm::cl::cat(CCSMToolCategory)
	);

static llvm::cl::opt<bool> NoMethod(
	"disable-method",
	llvm::cl::desc("Disable output of stats at the method level"),
	llvm::cl::init(false),
	llvm::cl::cat(CCSMToolCategory)
	);

static llvm::cl::opt<MetricDumpFormat_e> OutputFormat(
	"output-format",
	llvm::cl::desc("Format of output"),
	llvm::cl::values(
	    clEnumValN(METRIC_DUMP_FORMAT_TSV, "tsv", "Tab Seperated Values"),
    	clEnumValN(METRIC_DUMP_FORMAT_CSV, "csv", "Comma Separated Values"),
	    clEnumValN(METRIC_DUMP_FORMAT_SPARSE_TREE, "sparsetree", "Sparse Tree (zero value nodes omitted)"),
	    clEnumValN(METRIC_DUMP_FORMAT_TREE, "tree", "Tree Structure"),
	    clEnumValEnd
	),
	llvm::cl::init(METRIC_DUMP_FORMAT_TREE),
	llvm::cl::cat(CCSMToolCategory)
	);

static llvm::cl::opt<bool> PrototypesAreFileScope(
	"prototypes-are-filescope",
	llvm::cl::desc("The prototype part of a function declaration should be included in the file scope metrics, not the function scope"),
	llvm::cl::init(false),
	llvm::cl::cat(CCSMToolCategory)
	);

std::vector<std::string> OutputMetricList;

static llvm::cl::list<std::string, std::vector<std::string>> OutputOnlyMetrics(
	"output-metrics",
	llvm::cl::desc("Only output the specified metrics"),
	llvm::cl::CommaSeparated,
	llvm::cl::ZeroOrMore,
	llvm::cl::location(OutputMetricList),
	llvm::cl::cat(CCSMToolCategory));

static llvm::cl::opt<std::string> OutputToFile(
	"output-to-file",
	llvm::cl::desc("Specify a file to sent the output to"),
	llvm::cl::Optional,
	llvm::cl::cat(CCSMToolCategory));

static llvm::cl::extrahelp MoreHelp("\nVersion: " GEN_VER_VERSION_STRING);

static void PrintVersion() {
	std::cout << "CCSM (https://github.com/bright-tools/ccsm):" << '\n'
	          << "  CCSM version " << GEN_VER_VERSION_STRING << '\n'
			  << "  Built " << __DATE__ << " (" << __TIME__ << ").\n"
	          << "Using " << clang::getClangFullVersion() << '\n';
}

CcsmOptionsHandler::CcsmOptionsHandler() : m_optionsParser(NULL), m_metricOptions( NULL )
{
	llvm::cl::SetVersionPrinter(PrintVersion);
}

CcsmOptionsHandler::~CcsmOptionsHandler()
{
	delete(m_optionsParser);
	delete(m_metricOptions);
}

void CcsmOptionsHandler::ParseOptions(int argc,
                                      const char ** const argv )
{
	m_metricOptions = new MetricOptions(&ExcludeFileList, &ExcludeFunctionList, &OutputMetricList, &IncludeInParentFileList);
	m_optionsParser = new clang::tooling::CommonOptionsParser(argc, argv, CCSMToolCategory);

	m_metricOptions->setDumpTokens(DumpTokens);
	m_metricOptions->setDumpAST(DumpAST);
	m_metricOptions->setUseShortNames(UseShortNames);
	m_metricOptions->setUseAbsoluteFileNames(UseAbsoluteFileNames);
	m_metricOptions->setPrototypesAreFileScope(PrototypesAreFileScope);
	m_metricOptions->setOutputFormat(OutputFormat);
	m_metricOptions->setExcludeStdHeaders(ExcludeStandardHeaders);
	// TODO: Handle failure
	m_metricOptions->setOutputFile(OutputToFile);

	m_metricOptions->setOutputMetric(METRIC_UNIT_METHOD, !NoMethod);
	m_metricOptions->setOutputMetric(METRIC_UNIT_FUNCTION, !NoFunction);
	m_metricOptions->setOutputMetric(METRIC_UNIT_FILE, !NoFile);
	m_metricOptions->setOutputMetric(METRIC_UNIT_GLOBAL, !NoGlobal);

	m_metricOptions->setDumpFnMap(DumpFnMap);

	checkCompilerArgs(argv[0]);
}

MetricOptions* CcsmOptionsHandler::getMetricOptions() const
{
	return m_metricOptions;
}

clang::tooling::CommonOptionsParser* CcsmOptionsHandler::getOptionsParser() const
{
	return m_optionsParser;
}

#include "llvm/Support/Path.h"

const std::string sep = "[\\\\/^]";
const std::string end = "$";

const std::set<std::string> c89_std_headers = { 
	sep + "assert.h" + end, sep + "locale.h" + end, sep + "stddef.h" + end,
	sep + "ctype.h" + end,  sep + "math.h" + end,   sep + "stdio.h" + end,
	sep + "errno.h" + end,  sep + "setjmp.h" + end, sep + "stdlib.h" + end,
	sep + "float.h" + end,  sep + "signal.h" + end, sep + "string.h" + end,
	sep + "limits.h" + end, sep + "stdarg.h" + end, sep + "time.h" + end
};

const std::set<std::string> c99_std_headers = { 
	sep + "assert.h" + end,  sep + "inttypes.h" + end, sep + "signal.h" + end,  sep + "stdlib.h" + end,
	sep + "complex.h" + end, sep + "iso646.h" + end,   sep + "stdarg.h" + end,  sep + "string.h" + end,
	sep + "ctype.h" + end,   sep + "limits.h" + end,   sep + "stdbool.h" + end, sep + "tgmath.h" + end,
	sep + "errno.h" + end,   sep + "locale.h" + end,   sep + "stddef.h" + end,  sep + "time.h" + end,
	sep + "fenv.h" + end,    sep + "math.h" + end,     sep + "stdint.h" + end,  sep + "wchar.h" + end,
	sep + "float.h" + end,   sep + "setjmp.h" + end,   sep + "stdio.h" + end,   sep + "wctype.h" + end
};

const std::set<std::string> c11_std_headers = { 
	sep + "assert.h" + end,   sep + "math.h" + end,      sep + "stdlib.h" + end,
	sep + "complex.h" + end,  sep + "setjmp.h" + end,    sep + "stdnoreturn.h" + end,
	sep + "ctype.h" + end,    sep + "signal.h" + end,    sep + "string.h" + end,
	sep + "errno.h" + end,    sep + "stdalign.h" + end,  sep + "tgmath.h" + end,
	sep + "fenv.h" + end,     sep + "stdarg.h" + end,    sep + "threads.h" + end,
	sep + "float.h" + end,    sep + "stdatomic.h" + end, sep + "time.h" + end,
	sep + "inttypes.h" + end, sep + "stdbool.h" + end,   sep + "uchar.h" + end,
	sep + "iso646.h" + end,   sep + "stddef.h" + end,    sep + "wchar.h" + end,
	sep + "limits.h" + end,   sep + "stdint.h" + end,    sep + "wctype.h" + end,
	sep + "locale.h" + end,   sep + "stdio.h" + end
};

const std::set<std::string> cpp_std_headers = { 
	sep + "algorithm" + end,          sep + "fstream" + end,          sep + "list" + end,    sep + "regex" + end,            sep + "tuple" + end,
	sep + "array" + end,              sep + "functional" + end,       sep + "locale" + end,  sep + "scoped_allocator" + end, sep + "type_traits" + end,
	sep + "atomic" + end,             sep + "future" + end,           sep + "map" + end,     sep + "set" + end,              sep + "typeindex" + end,
	sep + "bitset" + end,             sep + "initializer_list" + end, sep + "memory" + end,  sep + "sstream" + end,          sep + "typeinfo" + end,
	sep + "chrono" + end,             sep + "iomanip" + end,          sep + "mutex" + end,   sep + "stack" + end,            sep + "unordered_map" + end,
	sep + "codecvt" + end,            sep + "ios" + end,              sep + "new" + end,     sep + "stdexcept" + end,        sep + "unordered_set" + end,
	sep + "complex" + end,            sep + "iosfwd" + end,           sep + "numeric" + end, sep + "streambuf" + end,        sep + "utility" + end,
	sep + "condition_variable" + end, sep + "iostream" + end,         sep + "ostream" + end, sep + "string" + end,           sep + "valarray" + end,
	sep + "deque" + end,              sep + "istream" + end,          sep + "queue" + end,   sep + "strstream" + end,        sep + "vector" + end,
	sep + "exception" + end,          sep + "iterator" + end,         sep + "random" + end,  sep + "system_error" + end,
	sep + "forward_list" + end,       sep + "limits" + end,           sep + "ratio" + end,   sep + "thread" + end,
	/* C++ headers for C library functions */
	sep + "cassert" + end,  sep + "cinttypes" + end, sep + "csignal" + end,   sep + "cstdio" + end,  sep + "cwchar" + end,
	sep + "ccomplex" + end, sep + "ciso646" + end,   sep + "cstdalign" + end, sep + "cstdlib" + end, sep + "cwctype" + end,
	sep + "cctype" + end,   sep + "climits" + end,   sep + "cstdarg" + end,   sep + "cstring" + end,
	sep + "cerrno" + end,   sep + "clocale" + end,   sep + "cstdbool" + end,  sep + "ctgmath" + end,
	sep + "cfenv" + end,    sep + "cmath" + end,     sep + "cstddef" + end,   sep + "ctime" + end,
	sep + "cfloat" + end,   sep + "csetjmp" + end,   sep + "cstdint" + end,   sep + "cuchar" + end
};


void CcsmOptionsHandler::checkCompilerArgs(const char* const exeName)
{
	analyseCompilerArgs(exeName);

	if (m_usesCpp)
	{
		std::cerr << "WARNING: Proper support for C++ language constructs is not currently implemented";

	}

	if (m_metricOptions->getExcludeStdHeaders())
	{
		if (m_usesCpp)
		{
			ExcludeFileList.insert(ExcludeFileList.end(), cpp_std_headers.begin(), cpp_std_headers.end());
		}

		if (m_usesC89)
		{
			ExcludeFileList.insert(ExcludeFileList.end(), c89_std_headers.begin(), c89_std_headers.end());
		}

		if (m_usesC99)
		{
			ExcludeFileList.insert(ExcludeFileList.end(), c99_std_headers.begin(), c99_std_headers.end());
		}

		if (m_usesC11)
		{
			ExcludeFileList.insert(ExcludeFileList.end(), c11_std_headers.begin(), c11_std_headers.end());
		}
	}
}

#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Driver/Driver.h"
#include "clang/Driver/Compilation.h"
#include "clang/Driver/Types.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Tooling/CompilationDatabase.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Frontend/CompilerInstance.h"
#include "llvm/Support/raw_ostream.h"
#include "clang/Tooling/Tooling.h"
#include <iostream>

using namespace clang;
using namespace clang::tooling;

void ccsm_marker(void)
{
}

void CcsmOptionsHandler::analyseCompilerArgs(const char* const exeName)
{
	std::string Path = llvm::sys::fs::getMainExecutable(exeName, (void*)(intptr_t)ccsm_marker);
	std::string TripleStr = llvm::sys::getProcessTriple();
	llvm::Triple T(TripleStr);

	m_usesCpp = false;
	m_usesC11 = false;
	m_usesC99 = false;
	m_usesC89 = false;

	for (const auto &SourcePath : m_optionsParser->getSourcePathList() ) {
		std::string File(getAbsolutePath(SourcePath));
		std::vector<CompileCommand> CompileCommandsForFile = m_optionsParser->getCompilations().getCompileCommands(File);
		for (CompileCommand &CompileCommand : CompileCommandsForFile) {
			std::vector<std::string> CommandLine = CompileCommand.CommandLine;
			std::vector<const char*> Args;
			for (const std::string &Str : CommandLine)
			{
				Args.push_back(Str.c_str());
			}
			IntrusiveRefCntPtr<DiagnosticOptions> DiagOpts = new DiagnosticOptions();
			TextDiagnosticPrinter *DiagClient =
				new TextDiagnosticPrinter(llvm::errs(), &*DiagOpts);
			IntrusiveRefCntPtr<DiagnosticIDs> DiagID(new DiagnosticIDs());
			DiagnosticsEngine Diags(DiagID, &*DiagOpts, DiagClient);
			clang::driver::Driver TheDriver(Path, T.str(), Diags);
			TheDriver.setCheckInputsExist(false);
			std::unique_ptr<clang::driver::Compilation> C(TheDriver.BuildCompilation(Args));
			const driver::JobList &Jobs = C->getJobs();
			const driver::Command &Cmd = cast<driver::Command>(*Jobs.begin());
			const driver::ArgStringList &CCArgs = Cmd.getArguments();
			std::unique_ptr<CompilerInvocation> CI(new CompilerInvocation);
			CompilerInvocation::CreateFromArgs(*CI,
				const_cast<const char **>(CCArgs.data()),
				const_cast<const char **>(CCArgs.data()) +
				CCArgs.size(),
				Diags);

			if ((*CI).getLangOpts()->C11)
			{
				m_usesC11 = true;
			}
			else if ((*CI).getLangOpts()->CPlusPlus)
			{
				m_usesCpp = true;
			}
			else if ((*CI).getLangOpts()->C99)
			{
				m_usesC99 = true;
			}
			else
			{
				m_usesC89 = true;
			}

#if 0
			std::cout << "C++: " << m_usesCpp << " C11:" << m_usesC11 << " C89: " << m_usesC89 << " C99: " << m_usesC99 << "\r\n";
#endif
		}
	}
}