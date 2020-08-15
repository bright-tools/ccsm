/*
   @file
   @brief Class to support storing of metrics relating to C
          code

   @author John Bailey
   @copyright Copyright 2014 John Bailey

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

#if !defined(METRIC_UNIT_HPP)
#define METRIC_UNIT_HPP

#include "clang/Basic/SourceLocation.h"

typedef enum
{
#define METRIC_ALIAS(_name, _alias)
#define METRIC(_enum, _short_name, _long_name, _applies_global, _applies_file, _applies_function,  \
               _applies_method, _cumulative, _multipass, _report_local, _scaling, _description)    \
    _enum,
#include "metrics.def"
#undef METRIC_ALIAS
#undef METRIC
    METRIC_TYPE_MAX
} MetricType_e;

typedef enum
{
    METRIC_UNIT_GLOBAL = 0,
    METRIC_UNIT_FILE,
    METRIC_UNIT_FUNCTION,
    METRIC_UNIT_METHOD,
    METRIC_UNIT_MAX
} MetricUnitType_e;

typedef enum
{
    METRIC_UNIT_PROCESS_AST,
    METRIC_UNIT_PROCESS_LEX_UNEXPANDED,
    METRIC_UNIT_PROCESS_LEX_EXPANDED,
    METRIC_UNIT_PROCESS_MAX
} MetricUnitProcessingType_e;

#include <map>
#include <set>
#include <stdint.h>
#include <string>

typedef struct {
    bool Valid;
    unsigned LineNo;
    std::string FileName;
    unsigned Column;
} SourceFileAndLine_t;

class MetricOptions;

class MetricUnit {
  public:
    typedef std::map<std::string, MetricUnit *> SubUnitMap_t;

    /** Type for holding metric values.
        See also counter_t_Max */
    typedef uint32_t counter_t;

  private:
    static const std::string m_metricNames[METRIC_TYPE_MAX];
    static const std::string m_metricShortNames[METRIC_TYPE_MAX];
    static const bool m_metricIsCumulative[METRIC_TYPE_MAX];
    static const bool m_metricReportLocal[METRIC_TYPE_MAX];
    static const uint32_t m_metricScaling[METRIC_TYPE_MAX];
    static const bool m_metricApplies[METRIC_UNIT_MAX][METRIC_TYPE_MAX];
    static const bool m_metricMultipass[METRIC_TYPE_MAX];

  protected:
    std::string m_name;
    std::string m_alias;
    MetricUnitType_e m_type;
    SubUnitMap_t m_subUnits;
    MetricUnit *m_parent;
    bool m_processed[METRIC_UNIT_PROCESS_MAX];
    bool m_hasExternalLinkage;
    /* for METRIC_UNIT_FUNCTION types, a list of fuction calls where the target
       function body was not visible when the function hosting the call was
       processed */
    std::set<std::string> m_unresolvedFnCalls;
    counter_t m_counters[METRIC_TYPE_MAX];
    std::map<MetricType_e, std::set<std::string>> m_supplementary;
    SourceFileAndLine_t m_firstInstanceLocation[METRIC_TYPE_MAX];

    /** Set containing all of the metrics which are calcuated rather than
     * directly measured */
    const static std::set<MetricType_e> m_calculatedMetricSet;

  public:
    typedef std::map<std::string, MetricUnit *> FunctionMap_t;

    static bool isMultiPassAllowed(const MetricType_e p_type);
    static bool isMetricCumulative(const MetricType_e p_type);

    static std::string getMetricName(const MetricType_e p_type);
    static std::string getMetricShortName(const MetricType_e p_type);
    static MetricType_e getMetricByShortName(const std::string p_shortName);
    static uint32_t getMetricScaling(const MetricType_e p_type);
    static bool doesMetricApplyForUnit(const MetricType_e p_MetricType,
                                       const MetricUnitType_e p_unitType);
    static bool isMetricLocalAndCumulative(const MetricType_e p_type);
    static float getScaledMetric(const MetricType_e p_type, counter_t p_val);
    static std::string getScaledMetricString(const MetricType_e p_type, counter_t p_val);

    const std::set<std::string> getSupplementary(MetricType_e p_metric,
                                                 const bool p_recurse = false) const;
    void setSupplementary(MetricType_e p_metric, const std::set<std::string> p_val);

    /** Maximum metric count which can be stored in a count_t */
    static const counter_t counter_t_Max;
    /** Special value stored in a count_t to indicate infinity */
    static const counter_t counter_t_Inf;
    /** Special value stored in a count_t to indicate that the metric could not
     * be calculated */
    static const counter_t counter_t_Na;

    MetricUnit(MetricUnit *const p_parent, const std::string &p_name,
               const MetricUnitType_e p_type);

    void addUnresolvedFn(const std::string &p_fnName);
    const std::set<std::string> &getUnresolvedFns() const;

    void setExternalLinkage(const bool p_isExternal = true);
    bool hasExternalLinkage(void) const;
    FunctionMap_t getAllFunctionMap(void);

    /** Sets a metric to the maximum of the current value and the specified
     * value
     */
    void setMax(const MetricType_e p_metricType, const MetricUnit::counter_t p_val,
                const SourceFileAndLine_t &p_sourceLoc);
    void increment(const MetricType_e p_metricType, const SourceFileAndLine_t &p_sourceLoc,
                   const counter_t p_inc = 1);
    void set(const MetricType_e p_metricType, const MetricUnit::counter_t p_val,
             const SourceFileAndLine_t &p_sourceLoc);

    counter_t getCounter(const MetricType_e p_metricType, const bool p_recurse = false) const;

    MetricUnit *getSubUnit(const std::string &p_name, const MetricUnitType_e p_type,
                           const bool p_create = true);

    counter_t getSubUnitCount(const MetricUnitType_e p_type) const;

    MetricUnitType_e GetType(void) const;
    std::string getUnitName(const bool p_useAbsFN, const bool p_usePrefix) const;
    std::string getUnitName(const MetricOptions &p_options) const;

    const SubUnitMap_t *getSubUnits(void) const;

    bool hasBeenProcessed(const MetricUnitProcessingType_e p_type) const;

    bool isFnOrMethod(void) const;

    void setProcessed(const MetricUnitProcessingType_e p_type);

    const MetricUnit *getParent() const;

    const SourceFileAndLine_t &getFirstInstanceLocation(const MetricType_e p_type) const;

    std::string getFileName(const MetricOptions &p_options) const;
};

#endif