/*
   @file
   @brief

   @author John Bailey
   @copyright Copyright 2016 John Bailey

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

#if !defined(LIMITS_LIBRARY_HPP)
#define LIMITS_LIBRARY_HPP

#include "MetricUnit.hpp"

#include "minicsv.h"

#include <list>
#include <map>
#include <vector>

class LimitLibrary {
  protected:
    typedef struct {
        bool global;
        bool file;
        std::string fileName;
        std::string funcName;
        std::string operand;
        std::string text;
        MetricUnit::counter_t limit;
    } limitPattern_t;

    typedef std::list<limitPattern_t> patternSet_t;
    typedef std::map<MetricType_e, patternSet_t> patternMap_t;

    patternMap_t m_patternMap;

    std::string
    checkUnitPassesMetricLimit(const MetricUnit &p_unit,
                               const MetricOptions &p_options,
                               const MetricType_e p_metric,
                               const limitPattern_t *const p_pattern) const;
    static bool unitMatchesLimitPattern(const MetricUnit &p_unit,
                                        const MetricOptions &p_options,
                                        const limitPattern_t *const p_pattern);
    void parseCsvLine(csv::ifstream &p_is);
    const limitPattern_t *
    findHighestPresidenceRule(const patternSet_t &p_set,
                              const MetricUnit &p_unit,
                              const MetricOptions &p_options) const;

  public:
    LimitLibrary();
    bool load(const std::string p_fileName);
    bool load(const std::vector<std::string> &p_fileNames);

    std::string checkLimit(const MetricUnit &p_unit,
                           const MetricOptions &p_options) const;
};

#endif
