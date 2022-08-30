/*
   @file
   @brief

   @author John Bailey
   @copyright Copyright 2015 John Bailey

   @section LICENSE

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

#if !defined(CCSM_OPTIONS_HANDLER_HPP)
#define CCSM_OPTIONS_HANDLER_HPP

#include "MetricOptions.hpp"
#include "StandardHeaders.hpp"
#include "clang/Tooling/CommonOptionsParser.h"

class CcsmOptionsHandler {

  private:
    MetricOptions *m_metricOptions = NULL;
    std::set<MetricType_e> m_outputMetrics;

    /** Indicates whether or not a particular standard is specified for any of the
     *  files to be analysed */
    bool m_usesStd[STD_COUNT] = {false};

    void analyseCompilerArgs(const char *const exeName,
                             clang::tooling::CommonOptionsParser &optionsParser);
    void checkCompilerArgs(const char *const exeName,
                           clang::tooling::CommonOptionsParser &optionsParser);
    bool addMetricToOutputBasedOnShortnameOrAlias(std::string metricName);
    void addAllMetricsToOutput(void);
    void processOutputMetricList(void);

  public:
    CcsmOptionsHandler();

    void ParseOptions(const char *const argv, clang::tooling::CommonOptionsParser &optionsParser);

    MetricOptions *getMetricOptions() const;

    virtual ~CcsmOptionsHandler();
};

extern llvm::cl::OptionCategory CCSMToolCategory;

#endif
