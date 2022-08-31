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
#include "MetricUnit.hpp"
#include "StandardHeaders.hpp"
#include "ccsm_ver.h"

#include <clang/Basic/Version.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/Regex.h>

#include <iostream>

static const std::pair<std::string, std::string> metricAliasListData[] = {
#define METRIC_ALIAS(_name, _alias) std::make_pair(_name, _alias),
#define METRIC(_enum, _short_name, _long_name, _applies_global, _applies_file, _applies_function,  \
               _applies_method, _cumulative, _multipass, _report_local, _scaling, _description)
#include "metrics.def"
#undef METRIC
#undef METRIC_ALIAS
};
static const std::set<std::pair<std::string, std::string>>
    metricAliasList(metricAliasListData, metricAliasListData + sizeof metricAliasListData /
                                                                   sizeof metricAliasListData[0]);

// Set up the command line options
llvm::cl::OptionCategory CCSMToolCategory("ccsm options");

static std::vector<std::string> ExcludeFunctionList;

static llvm::cl::list<std::string, std::vector<std::string>>
    ExcludeFunctions("exclude-function",
                     llvm::cl::desc("Exclude specified function from the metrics"),
                     llvm::cl::CommaSeparated, llvm::cl::ZeroOrMore,
                     llvm::cl::location(ExcludeFunctionList), llvm::cl::cat(CCSMToolCategory));

static std::vector<std::string> ExcludeFileList;

static llvm::cl::list<std::string, std::vector<std::string>>
    ExcludeFiles("exclude-file", llvm::cl::desc("Exclude specified file from the metrics"),
                 llvm::cl::CommaSeparated, llvm::cl::ZeroOrMore, llvm::cl::value_desc("regex"),
                 llvm::cl::location(ExcludeFileList), llvm::cl::cat(CCSMToolCategory));

static std::vector<std::string> IncludeInParentFileList;

static llvm::cl::list<std::string, std::vector<std::string>> IncludeInParentFiles(
    "def-file",
    llvm::cl::desc("Specifies files whose contents should not be individually counted "
                   "and should be included in the including file.  Intended to help "
                   "support the def file idiom"),
    llvm::cl::CommaSeparated, llvm::cl::ZeroOrMore, llvm::cl::location(IncludeInParentFileList),
    llvm::cl::cat(CCSMToolCategory));

static llvm::cl::opt<bool> UseShortNames("output-short-names",
                                         llvm::cl::desc("Use short metric names in the output"),
                                         llvm::cl::init(false), llvm::cl::cat(CCSMToolCategory));

static llvm::cl::opt<bool>
    UseAbsoluteFileNames("output-absolute-fn",
                         llvm::cl::desc("Use absolute filenames in the output"),
                         llvm::cl::init(false), llvm::cl::cat(CCSMToolCategory));

static llvm::cl::opt<bool>
    ExcludeStandardHeaders("exclude-std-headers",
                           llvm::cl::desc("Exclude standard library headers"),
                           llvm::cl::init(false), llvm::cl::cat(CCSMToolCategory));

static llvm::cl::opt<bool> DumpTokens("dump-tokens",
                                      llvm::cl::desc("Dump tokens as they are lexed"),
                                      llvm::cl::init(false), llvm::cl::cat(CCSMToolCategory));

static llvm::cl::opt<bool> DumpAST("dump-ast", llvm::cl::desc("Dump AST when it is processed"),
                                   llvm::cl::init(false), llvm::cl::cat(CCSMToolCategory));

static llvm::cl::opt<bool>
    DumpFnMap("dump-function-map", llvm::cl::desc("Dump the mapping of files/functions/locations"),
              llvm::cl::init(false), llvm::cl::cat(CCSMToolCategory));

static llvm::cl::opt<bool> NoGlobal("disable-global",
                                    llvm::cl::desc("Disable output of stats at the global level"),
                                    llvm::cl::init(false), llvm::cl::cat(CCSMToolCategory));

static llvm::cl::opt<bool> NoFile("disable-file",
                                  llvm::cl::desc("Disable output of stats at the file level"),
                                  llvm::cl::init(false), llvm::cl::cat(CCSMToolCategory));

static llvm::cl::opt<bool>
    NoFunction("disable-function", llvm::cl::desc("Disable output of stats at the function level"),
               llvm::cl::init(false), llvm::cl::cat(CCSMToolCategory));

static llvm::cl::opt<bool> NoMethod("disable-method",
                                    llvm::cl::desc("Disable output of stats at the method level"),
                                    llvm::cl::init(false), llvm::cl::cat(CCSMToolCategory));

static llvm::cl::opt<MetricDumpFormat_e> OutputFormat(
    "output-format", llvm::cl::desc("Format of metric output"),
    llvm::cl::values(clEnumValN(METRIC_DUMP_FORMAT_NONE, "none", "Don't output metric data"),
                     clEnumValN(METRIC_DUMP_FORMAT_TSV, "tsv", "Tab Seperated Values"),
                     clEnumValN(METRIC_DUMP_FORMAT_CSV, "csv", "Comma Separated Values"),
                     clEnumValN(METRIC_DUMP_FORMAT_SPARSE_TREE, "sparsetree",
                                "Sparse Tree (zero value nodes omitted)"),
                     clEnumValN(METRIC_DUMP_FORMAT_TREE, "tree", "Tree Structure")),
    llvm::cl::init(METRIC_DUMP_FORMAT_TREE), llvm::cl::cat(CCSMToolCategory));

static llvm::cl::opt<bool> PrototypesAreFileScope(
    "prototypes-are-filescope",
    llvm::cl::desc("The prototype part of a function declaration should be included in "
                   "the file scope metrics, not the function scope"),
    llvm::cl::init(false), llvm::cl::cat(CCSMToolCategory));

std::vector<std::string> OutputMetricList;

static llvm::cl::list<std::string, std::vector<std::string>>
    OutputOnlyMetrics("output-metrics", llvm::cl::desc("Only output the specified metrics"),
                      llvm::cl::CommaSeparated, llvm::cl::ZeroOrMore,
                      llvm::cl::location(OutputMetricList), llvm::cl::cat(CCSMToolCategory));

static llvm::cl::opt<std::string>
    OutputToFile("output-to-file", llvm::cl::desc("Specify a file to sent the output to"),
                 llvm::cl::Optional, llvm::cl::cat(CCSMToolCategory));

static llvm::cl::opt<bool>
    PrefixFunctionName("prefix-function-name",
                       llvm::cl::desc("Prefix function name with filename in output"),
                       llvm::cl::init(false), llvm::cl::cat(CCSMToolCategory));

static llvm::cl::opt<MetricLimitsFormat_e> OutputLimitWarnings(
    "output-limit-warnings", llvm::cl::desc("Output format for limits warnings"),
    llvm::cl::values(clEnumValN(METRIC_LIMITS_FORMAT_DEFAULT, "default", "Default output format"),
                     clEnumValN(METRIC_LIMITS_FORMAT_GROUP_BY_FILE, "byfile",
                                "Group warnings by file"),
                     clEnumValN(METRIC_LIMITS_FORMAT_GCC, "gccbrief", "Use GCC brief format")),
    llvm::cl::init(METRIC_LIMITS_FORMAT_DEFAULT), llvm::cl::cat(CCSMToolCategory));

static llvm::cl::list<std::string>
    LimitsFile("limits", llvm::cl::desc("Specify a file to read limits data from"),
               llvm::cl::CommaSeparated, llvm::cl::ZeroOrMore, llvm::cl::value_desc("filename"),
               llvm::cl::cat(CCSMToolCategory));

static llvm::cl::opt<bool>
    UseConditionAnalysis("enable-condition-analysis",
                         llvm::cl::desc("Enable condition analysis for path counting"),
                         llvm::cl::init(false), llvm::cl::cat(CCSMToolCategory));

static llvm::cl::extrahelp MoreHelp("\nVersion: " GEN_VER_VERSION_STRING);

static void PrintVersion(llvm::raw_ostream &OS) {
    OS << "CCSM (https://github.com/bright-tools/ccsm):" << '\n'
       << "  CCSM version " << GEN_VER_VERSION_STRING << '\n'
#if 0
	   << "  Built " << __DATE__ << " (" << __TIME__ << ").\n"
#endif
       << "Using " << clang::getClangFullVersion() << '\n';
}

CcsmOptionsHandler::CcsmOptionsHandler() {
    llvm::cl::SetVersionPrinter(PrintVersion);
}

CcsmOptionsHandler::~CcsmOptionsHandler() {
    delete (m_metricOptions);
}

bool CcsmOptionsHandler::addMetricToOutputBasedOnShortnameOrAlias(std::string metricName) {
    bool added = false;

    /* Is it a regex? */
    if (metricName.find_first_of(".[{}()\\*+?|^$") == std::string::npos) {
        /* No - decorate it so that it will only match exactly */
        metricName = "^" + metricName + "$";
    }

    llvm::Regex metricNameRegex(metricName);

    /* Loop all the metrics and determine whether or not they match the regex */
    for (uint16_t metric = 0; metric < METRIC_TYPE_MAX; metric++) {
        if (metricNameRegex.match(
                MetricUnit::getMetricShortName(static_cast<MetricType_e>(metric)))) {
            m_outputMetrics.insert(static_cast<MetricType_e>(metric));
            added = true;
        }
    }

    /* Loop all the aliases and determine whether or not they match the regex */
    for (std::set<std::pair<std::string, std::string>>::const_iterator it = metricAliasList.begin();
         (it != metricAliasList.end()); it++) {
        if (metricNameRegex.match(it->second)) {
            m_outputMetrics.insert(MetricUnit::getMetricByShortName(it->first));
            added = true;
        }
    }

    return added;
}

void CcsmOptionsHandler::addAllMetricsToOutput(void) {
    for (uint16_t metric = 0; metric < METRIC_TYPE_MAX; metric++) {
        m_outputMetrics.insert(static_cast<MetricType_e>(metric));
    }
}

void CcsmOptionsHandler::processOutputMetricList() {
    const bool wereMetricsSpecifiedOnCommandLine = OutputMetricList.size() > 0;

    if (wereMetricsSpecifiedOnCommandLine) {
        /* Iterate all the metric strings specified on the command line */
        for (std::vector<std::string>::const_iterator it = OutputMetricList.begin();
             it != OutputMetricList.end(); it++) {

            if (!addMetricToOutputBasedOnShortnameOrAlias(*it)) {
                std::cerr << "Unrecognised parameter to --output-metrics: " << *it << "\n";
                exit(EXIT_FAILURE);
            }
        }
    } else {
        addAllMetricsToOutput();
    }
}

void CcsmOptionsHandler::ParseOptions(const char *const argv,
                                      clang::tooling::CommonOptionsParser &optionsParser) {
    processOutputMetricList();

    m_metricOptions = new MetricOptions(&ExcludeFileList, &ExcludeFunctionList, m_outputMetrics,
                                        &IncludeInParentFileList);

    m_metricOptions->setDumpTokens(DumpTokens);
    m_metricOptions->setDumpAST(DumpAST);
    m_metricOptions->setUseShortNames(UseShortNames);
    m_metricOptions->setUseAbsoluteFileNames(UseAbsoluteFileNames);
    m_metricOptions->setPrototypesAreFileScope(PrototypesAreFileScope);
    m_metricOptions->setOutputFormat(OutputFormat);
    m_metricOptions->setExcludeStdHeaders(ExcludeStandardHeaders);
    m_metricOptions->setLimitsFile(LimitsFile);
    // TODO: Handle failure
    m_metricOptions->setOutputFile(OutputToFile);
    m_metricOptions->setUsePrefix(PrefixFunctionName);
    m_metricOptions->setLimitWarningOutputFormat(OutputLimitWarnings);
    m_metricOptions->setUseConditionAnalysis(UseConditionAnalysis);

    m_metricOptions->setOutputMetric(METRIC_UNIT_METHOD, !NoMethod);
    m_metricOptions->setOutputMetric(METRIC_UNIT_FUNCTION, !NoFunction);
    m_metricOptions->setOutputMetric(METRIC_UNIT_FILE, !NoFile);
    m_metricOptions->setOutputMetric(METRIC_UNIT_GLOBAL, !NoGlobal);

    m_metricOptions->setDumpFnMap(DumpFnMap);

    checkCompilerArgs(argv, optionsParser);
}

MetricOptions *CcsmOptionsHandler::getMetricOptions() const {
    return m_metricOptions;
}

void CcsmOptionsHandler::checkCompilerArgs(const char *const exeName,
                                           clang::tooling::CommonOptionsParser &optionsParser) {
    analyseCompilerArgs(exeName, optionsParser);

    if (m_usesStd[STD_CPP]) {
        std::cerr << "WARNING: Proper support for C++ language constructs is not "
                     "currently implemented";
    }

    if (m_metricOptions->getExcludeStdHeaders()) {
        AppendStandardHeaders(m_usesStd, ExcludeFileList);
    }
}

#include "clang/Basic/SourceManager.h"
#include "clang/Driver/Compilation.h"
#include "clang/Driver/Driver.h"
#include "clang/Driver/Types.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/CompilationDatabase.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"
#include <iostream>

using namespace clang;
using namespace clang::tooling;

void ccsm_marker(void) {
}

void CcsmOptionsHandler::analyseCompilerArgs(const char *const exeName,
                                             clang::tooling::CommonOptionsParser &optionsParser) {
    std::string Path = llvm::sys::fs::getMainExecutable(exeName, (void *)ccsm_marker);
    std::string TripleStr = llvm::sys::getProcessTriple();
    llvm::Triple T(TripleStr);

    for (int i = STD_FIRST; i < STD_COUNT; i++) {
        m_usesStd[i] = false;
    }

    for (const auto &SourcePath : optionsParser.getSourcePathList()) {
        std::string File(getAbsolutePath(SourcePath));
        std::vector<CompileCommand> CompileCommandsForFile =
            optionsParser.getCompilations().getCompileCommands(File);
        for (CompileCommand &CompileCommand : CompileCommandsForFile) {
            std::vector<std::string> CommandLine = CompileCommand.CommandLine;
            std::vector<const char *> Args;
            for (const std::string &Str : CommandLine) {
                Args.push_back(Str.c_str());
            }
            /* Only run the analyzer on the code (i.e. don't try a link phase)
             */
            Args.push_back("--analyze");
            IntrusiveRefCntPtr<DiagnosticOptions> DiagOpts = new DiagnosticOptions();
            TextDiagnosticPrinter *DiagClient = new TextDiagnosticPrinter(llvm::errs(), &*DiagOpts);
            IntrusiveRefCntPtr<DiagnosticIDs> DiagID(new DiagnosticIDs());
            DiagnosticsEngine Diags(DiagID, &*DiagOpts, DiagClient);
            clang::driver::Driver TheDriver(Path, T.str(), Diags);
            clang::driver::Compilation *compilation = TheDriver.BuildCompilation(Args);
            if (compilation->getJobs().empty()) {
                std::cout
                    << "No compile jobs identified.  Were valid file(s) specified for analysis?"
                    << std::endl;
                exit(-1);
            }
            std::unique_ptr<clang::driver::Compilation> C(compilation);
            const driver::JobList &Jobs = C->getJobs();
            const driver::Command &Cmd = cast<driver::Command>(*Jobs.begin());
            const llvm::opt::ArgStringList &CCArgs = Cmd.getArguments();
            std::unique_ptr<CompilerInvocation> CI(new CompilerInvocation);
            CompilerInvocation::CreateFromArgs(*CI, CCArgs, Diags);

            if ((*CI).getLangOpts()->C11) {
                m_usesStd[STD_C11] = true;
            } else if ((*CI).getLangOpts()->CPlusPlus) {
                m_usesStd[STD_CPP] = true;
            } else if ((*CI).getLangOpts()->C99) {
                m_usesStd[STD_C99] = true;
            } else {
                m_usesStd[STD_C89] = true;
            }

#if 0
			std::cout << "C++: " << m_usesCpp << " C11:" << m_usesC11 << " C89: " << m_usesC89 << " C99: " << m_usesC99 << "\n";
#endif
        }
    }
}
