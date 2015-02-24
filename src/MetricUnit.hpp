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

#if !defined( METRIC_UNIT_HPP )
#define       METRIC_UNIT_HPP

#include "MetricOptions.hpp"

typedef enum
{
	METRIC_DUMP_FORMAT_TREE,
	METRIC_DUMP_FORMAT_SPARSE_TREE,
	METRIC_DUMP_FORMAT_TSV,
	METRIC_DUMP_FORMAT_CSV
} MetricDumpFormat_e;

typedef enum
{
#define METRIC( _enum, _short_name, _long_name, _applies_global, _applies_file, _applies_function, _applies_method, _cumulative, _multipass, _report_local, _scaling, _description  ) _enum ,
#include "metrics.def"
#undef  METRIC
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

#include <string>
#include <map>
#include <stdint.h>

class MetricUnit
{
private:
	static const std::string m_dumpPrefix[ METRIC_UNIT_MAX ];
	static const std::string m_namePrefix[ METRIC_UNIT_MAX ];
	static const std::string m_metricNames[ METRIC_TYPE_MAX ];
	static const std::string m_metricShortNames[ METRIC_TYPE_MAX ];
	static const bool        m_metricIsCumulative[ METRIC_TYPE_MAX ];
	static const bool        m_metricReportLocal[ METRIC_TYPE_MAX ];
	static const uint32_t    m_metricScaling[ METRIC_TYPE_MAX ];
	static const bool        m_metricApplies[ METRIC_UNIT_MAX ][ METRIC_TYPE_MAX ];
	static const bool        m_metricMultipass[ METRIC_TYPE_MAX ];
protected:
	typedef std::map<std::string, MetricUnit*> SubUnitMap_t;
	std::string m_name;
	MetricUnitType_e m_type;
	SubUnitMap_t m_subUnits;
	MetricUnit* m_parent;
	bool m_processed;

public:
	/* See also counter_t_Max */
	typedef uint16_t counter_t;

	static bool isMultiPassAllowed( const MetricType_e p_type );

	static std::string getMetricName( const MetricType_e p_type );

	static const uint16_t counter_t_Max;

	MetricUnit( MetricUnit* const p_parent, const std::string& p_name, const MetricUnitType_e p_type );

	void increment( const MetricType_e p_metricType, const counter_t p_inc = 1 );
	void set( const MetricType_e p_metricType, const MetricUnit::counter_t p_val );

	void dump( std::ostream& out, const bool p_output[ METRIC_UNIT_MAX ], const MetricDumpFormat_e p_fmt = METRIC_DUMP_FORMAT_TREE, const MetricOptions* const p_options = NULL ) const;
	void dumpMetric( std::ostream& out, const MetricType_e p_metric, const MetricDumpFormat_e p_fmt, const std::string& p_sep, const bool p_recurse ) const;


	counter_t getCounter( const MetricType_e p_metricType, const bool p_recurse = false ) const;

	MetricUnit* getSubUnit( const std::string& p_name, const MetricUnitType_e p_type, const bool p_create = true );

	counter_t getSubUnitCount( const MetricUnitType_e p_type ) const;

	MetricUnitType_e GetType( void ) const;

	bool hasBeenProcessed( void ) const;

	bool isFnOrMethod( void ) const;

	void setProcessed( void );

protected:
	counter_t m_counters[ METRIC_TYPE_MAX ];
};

#endif