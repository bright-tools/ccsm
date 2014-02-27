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

#if !defined( METRIC_UNIT_HPP )
#define       METRIC_UNIT_HPP

typedef enum
{
	METRIC_TYPE_IF = 0,
	METRIC_TYPE_ELSE,
	METRIC_TYPE_FORLOOP,
	METRIC_TYPE_MAX
} MetricType_e;

typedef enum
{
	METRIC_UNIT_GLOBAL,
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
	static const std::string m_subPrefix[ METRIC_UNIT_MAX ];
protected:
	typedef std::map<std::string, MetricUnit*> SubUnitMap_t;
	std::string m_name;
	MetricUnitType_e m_type;
	SubUnitMap_t m_subUnits;
public:
	typedef uint16_t counter_t;

	MetricUnit( const std::string& p_name, const MetricUnitType_e p_type );

	void increment( const MetricType_e p_metricType );

	void dump( std::ostream& out ) const;

	counter_t getCounter( const MetricType_e p_metricType ) const;

	MetricUnit* getSubUnit( const std::string& p_name, const MetricUnitType_e p_type );

protected:
	counter_t m_counters[ METRIC_TYPE_MAX ];
};

#endif