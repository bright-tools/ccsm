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

typedef enum
{
	METRIC_DUMP_FORMAT_TREE,
	METRIC_DUMP_FORMAT_TSV,
	METRIC_DUMP_FORMAT_CSV
} MetricDumpFormat_e;

typedef enum
{
	METRIC_TYPE_FILES = 0,
	METRIC_TYPE_FUNCTIONS,
	METRIC_TYPE_IF,
	METRIC_TYPE_ELSE,
	METRIC_TYPE_FORLOOP,
	METRIC_TYPE_RETURN,
	METRIC_TYPE_WHILELOOP,
	METRIC_TYPE_SWITCH,
	METRIC_TYPE_CASE,
	METRIC_TYPE_DEFAULT,
	METRIC_TYPE_LOGICAL_AND,
	METRIC_TYPE_LOGICAL_OR,
	METRIC_TYPE_TERNARY,
	METRIC_TYPE_GOTO,
	METRIC_TYPE_LABEL,
	METRIC_TYPE_VARIABLE,
	METRIC_TYPE_RETURNPOINTS,
	METRIC_TYPE_STATEMENTS,
	METRIC_TYPE_CYCLOMATIC,
	METRIC_TYPE_MODIFIED_CYCLOMATIC,
	METRIC_TYPE_LOCAL_FUNCTIONS,
	METRIC_TYPE_FUNCTION_CALLS,
	METRIC_TYPE_LOCAL_FUNCTION_CALLS,
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
	static const bool        m_metricApplies[ METRIC_UNIT_MAX ][ METRIC_TYPE_MAX ];
protected:
	typedef std::map<std::string, MetricUnit*> SubUnitMap_t;
	std::string m_name;
	MetricUnitType_e m_type;
	SubUnitMap_t m_subUnits;
	MetricUnit* m_parent;
public:
	typedef uint16_t counter_t;

	MetricUnit( MetricUnit* const p_parent, const std::string& p_name, const MetricUnitType_e p_type );

	void increment( const MetricType_e p_metricType );

	void dump( std::ostream& out, const bool p_output[ METRIC_UNIT_MAX ], const MetricDumpFormat_e p_fmt = METRIC_DUMP_FORMAT_TREE ) const;

	counter_t getCounter( const MetricType_e p_metricType ) const;

	MetricUnit* getSubUnit( const std::string& p_name, const MetricUnitType_e p_type );

	counter_t getSubUnitCount( const MetricUnitType_e p_type ) const;

	MetricUnitType_e GetType( void ) const;


protected:
	counter_t m_counters[ METRIC_TYPE_MAX ];
};

#endif