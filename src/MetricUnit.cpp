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
const std::string MetricUnit::m_subPrefix[ METRIC_UNIT_MAX ] = {
	"Files: ",
	"Functions/Methods: ",
	"",
	""
};

const std::string MetricUnit::m_metricNames[ METRIC_TYPE_MAX ] = {
	"IF statements",
	"ELSE statements",
	"FOR loops",
	"RETURN statements",
	"WHILE loops",
	"SWITCH statements",
	"CASE statements",
	"DEFAULT statements",
	"LOGICAL AND operators",
	"LOGICAL OR operators",
	"TERNARY operators",
	"GOTO statements",
	"LABEL statements",
	"Variables",
	"Return points",
	"Statements",
	"McCabe",
	"Modified McCabe",
	"Local Functions"
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

MetricUnitType_e MetricUnit::GetType( void ) const
{
	return m_type;
}

MetricUnit::counter_t MetricUnit::getSubUnitCount( const MetricUnitType_e p_type ) const
{
	counter_t ret_val = 0;

	for( SubUnitMap_t::const_iterator unitIt = m_subUnits.begin();
		 unitIt != m_subUnits.end();
		 ++unitIt )
	{
		if( (*unitIt).second->GetType() == p_type )
		{
			ret_val++;
		}
		ret_val += (*unitIt).second->getSubUnitCount( p_type );
	}

	return ret_val;
}

MetricUnit::counter_t MetricUnit::getCounter( const MetricType_e p_metricType ) const
{
	counter_t ret_val = 0;

	switch( p_metricType ) 
	{
		case METRIC_TYPE_MODIFIED_CYCLOMATIC:
			if(( m_type == METRIC_UNIT_FUNCTION ) ||
			   ( m_type == METRIC_UNIT_METHOD ))
			{
				ret_val = getCounter( METRIC_TYPE_FORLOOP ) +
						  getCounter( METRIC_TYPE_IF ) +
						  getCounter( METRIC_TYPE_WHILELOOP ) +
						  getCounter( METRIC_TYPE_SWITCH ) +
						  getCounter( METRIC_TYPE_LOGICAL_AND ) +
						  getCounter( METRIC_TYPE_LOGICAL_OR ) +
						  getCounter( METRIC_TYPE_TERNARY ) + 1;
			}
			break;
		case METRIC_TYPE_CYCLOMATIC:
			if(( m_type == METRIC_UNIT_FUNCTION ) ||
			   ( m_type == METRIC_UNIT_METHOD ))
			{
				// TODO: What about the 'DEFAULT'?
				ret_val = getCounter( METRIC_TYPE_FORLOOP ) +
						  getCounter( METRIC_TYPE_IF ) +
						  getCounter( METRIC_TYPE_WHILELOOP ) +
						  getCounter( METRIC_TYPE_CASE ) +
						  getCounter( METRIC_TYPE_LOGICAL_AND ) +
						  getCounter( METRIC_TYPE_LOGICAL_OR ) +
						  getCounter( METRIC_TYPE_TERNARY ) + 1;
			}
			break;
		default:
			ret_val  = m_counters[ p_metricType ];
			for( SubUnitMap_t::const_iterator unitIt = m_subUnits.begin();
				 unitIt != m_subUnits.end();
				 ++unitIt )
			{
				ret_val += (*unitIt).second->getCounter( p_metricType );
			}
			break;
	}

	return ret_val;
}

void MetricUnit::dump( std::ostream& out, const bool p_output[ METRIC_UNIT_MAX ], const MetricDumpFormat_e p_fmt ) const
{
	std::string sep;

	if( p_output[ m_type ] ) 
	{
		switch( p_fmt ) {
		case METRIC_DUMP_FORMAT_TREE:
			// TODO: Should be endl
			sep = "\r\n";
			break;
		case METRIC_DUMP_FORMAT_TSV:
			sep = "\t";
			break;
		case METRIC_DUMP_FORMAT_CSV:
			sep = ",";
			break;
		}

		if(( p_fmt == METRIC_DUMP_FORMAT_TSV ) ||
		   ( p_fmt == METRIC_DUMP_FORMAT_CSV )) {

			if( m_type == METRIC_UNIT_GLOBAL )
			{
				out << "Name" << sep 
					<< m_subPrefix[ m_type ] << sep
					<< "Functions" << sep;
				unsigned loop;
				for( loop = 0;
					 loop < METRIC_TYPE_MAX;
					 loop++ )
				{
					out << m_metricNames[loop] << sep;
				}
				out << std::endl;
			}
		}

		if( p_fmt == METRIC_DUMP_FORMAT_TREE ) {
			out << m_namePrefix[ m_type ]; 
		}
		out << m_name << sep;
		if( p_fmt == METRIC_DUMP_FORMAT_TREE ) {
			if( m_subPrefix[ m_type ].length() ) {
				out << m_dumpPrefix[ m_type ] << m_subPrefix[ m_type ] << m_subUnits.size() << sep;
			}
		} else {
			out << m_subUnits.size() << sep;
		}
		/* Don't output function count at the function/method level */
		if(( m_type != METRIC_UNIT_FUNCTION ) &&
		   ( m_type != METRIC_UNIT_METHOD ))
		{
			if( p_fmt == METRIC_DUMP_FORMAT_TREE ) {
				out << m_dumpPrefix[ m_type ] << "Functions: ";
			}
			out << getSubUnitCount( METRIC_UNIT_FUNCTION ) << sep;
		} else if( p_fmt != METRIC_DUMP_FORMAT_TREE )
		{
			out << sep;
		}
		unsigned loop;
		for( loop = 0;
			 loop < METRIC_TYPE_MAX;
			 loop++ )
		{
			/* Filter out metrics which only apply at file/method level */
			if(((( loop != METRIC_TYPE_CYCLOMATIC ) &&
			     ( loop != METRIC_TYPE_MODIFIED_CYCLOMATIC )) ||
			    (( m_type == METRIC_UNIT_FUNCTION ) ||
			     ( m_type == METRIC_UNIT_METHOD ))) &&
			   (( loop != METRIC_TYPE_LOCAL_FUNCTIONS ) ||
			   (( m_type == METRIC_UNIT_GLOBAL ) ||
			     ( m_type == METRIC_UNIT_FILE ))))
			{
				if( p_fmt == METRIC_DUMP_FORMAT_TREE ) {
					out << m_dumpPrefix[ m_type ] << m_metricNames[loop] << ": ";
				} 
				out << getCounter((MetricType_e) loop ) << sep;
			} else if( p_fmt != METRIC_DUMP_FORMAT_TREE )
			{
				out << sep;
			}
		}

		if(( p_fmt == METRIC_DUMP_FORMAT_TSV ) ||
		   ( p_fmt == METRIC_DUMP_FORMAT_CSV )) {
			out << std::endl;
		}
	}

	for( SubUnitMap_t::const_iterator unitIt = m_subUnits.begin();
		 unitIt != m_subUnits.end();
		 ++unitIt )
	{
		(*unitIt).second->dump( out, p_output, p_fmt );
	}
}

MetricUnit* MetricUnit::getSubUnit( const std::string& p_name, const MetricUnitType_e p_type )
{
	MetricUnit* ret_val = NULL;
	SubUnitMap_t::iterator name_it = m_subUnits.find( p_name );
	if( name_it == m_subUnits.end() )
	{
		ret_val = new MetricUnit( p_name, p_type );

		if(( p_type == METRIC_UNIT_FUNCTION ) ||
		   ( p_type == METRIC_UNIT_METHOD ))
		{
			/* By default, every function/method has 1 return point */
			ret_val->increment( METRIC_TYPE_RETURNPOINTS );
		}
		m_subUnits[ p_name ] = ret_val;
	} else {
		ret_val = (*name_it).second;
	}
	return ret_val;
}
