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
#include "clang/Tooling/CommonOptionsParser.h"

class CcsmOptionsHandler {

  private:
    clang::tooling::CommonOptionsParser *m_optionsParser = NULL;
    MetricOptions *m_metricOptions = NULL;
    std::set<MetricType_e> m_outputMetrics;

    /** Indicates whether or not the C89/90 standard is specified for any of the
     * files to be analysed */
    bool m_usesC89 = false;
    /** Indicates whether or not the C11 standard is specified for any of the
     * files to be analysed */
    bool m_usesC11 = false;
    /** Indicates whether or not the C99 standard is specified for any of the
     * files to be analysed */
    bool m_usesC99 = false;
    /** Indicates whether or not C++ is specified for any of the files to be
     * analysed */
    bool m_usesCpp = false;

    void analyseCompilerArgs(const char *const exeName);
    void checkCompilerArgs(const char *const exeName);
    void processOutputMetricList(void);

  public:
    CcsmOptionsHandler();

    void ParseOptions(int argc, const char **const argv);

    MetricOptions *getMetricOptions() const;
    clang::tooling::CommonOptionsParser *getOptionsParser() const;

    virtual ~CcsmOptionsHandler();
};

#endif
