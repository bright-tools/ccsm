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

#include "MetricUnit.hpp"

const std::string MetricUnit::m_namePrefix[ METRIC_UNIT_MAX ] = {
	"",
	"File: ",
	"\tFunction: ",
	"\tMethod: "
};
const std::string MetricUnit::m_dumpPrefix[ METRIC_UNIT_MAX ] = {
	"",
	"\t",
	"\t\t",
	"\t\t"
};


MetricUnit::MetricUnit( const std::string& p_name, const MetricUnitType_e p_type ) : m_name( p_name ), m_type( p_type )
{
	uint16_t loop;

	for( loop = 0;
		 loop < METRIC_TYPE_MAX;
		 loop++ )
	{
		m_counters[ loop ] = 0;
	}
}

void MetricUnit::increment( const MetricType_e p_metricType )
{
	m_counters[ p_metricType ]++;
}

MetricUnit::counter_t MetricUnit::getCounter( const MetricType_e p_metricType ) const
{
	counter_t ret_val = m_counters[ p_metricType ];

	for( SubUnitMap_t::const_iterator unitIt = m_subUnits.begin();
		 unitIt != m_subUnits.end();
		 ++unitIt )
	{
		ret_val += (*unitIt).second->getCounter( p_metricType );
	}


	return ret_val;
}

void MetricUnit::dump( std::ostream& out ) const
{
	out << m_namePrefix[ m_type ] << m_name << std::endl;
	out << m_dumpPrefix[ m_type ] << "IF statements: " << getCounter( METRIC_TYPE_IF ) << std::endl;
	out << m_dumpPrefix[ m_type ] << "ELSE statements: " << getCounter( METRIC_TYPE_ELSE ) << std::endl;
	out << m_dumpPrefix[ m_type ] << "FOR loops: " << getCounter( METRIC_TYPE_FORLOOP ) << std::endl;

	for( SubUnitMap_t::const_iterator unitIt = m_subUnits.begin();
		 unitIt != m_subUnits.end();
		 ++unitIt )
	{
		(*unitIt).second->dump( out );
	}
}

MetricUnit* MetricUnit::getSubUnit( const std::string& p_name, const MetricUnitType_e p_type )
{
	MetricUnit* ret_val = NULL;
	SubUnitMap_t::iterator name_it = m_subUnits.find( p_name );
	if( name_it == m_subUnits.end() )
	{
		ret_val = new MetricUnit( p_name, p_type );
		m_subUnits[ p_name ] = ret_val;
	} else {
		ret_val = (*name_it).second;
	}
	return ret_val;
}
