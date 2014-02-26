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
} MetricType_e;

#include <string>
#include <map>

class MetricUnit
{
protected:
	std::string m_name;
	std::map<std::string, MetricUnit*> m_subUnits;
public:
	MetricUnit( const std::string& p_name );

	void increment( const MetricType_e p_metricType );

	void dump( std::ostream& out );
};

#endif