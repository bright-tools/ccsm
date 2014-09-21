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

#define IS_OUTPUT_TREE_FORM( _fmt ) (((_fmt) == METRIC_DUMP_FORMAT_TREE ) || ((_fmt) == METRIC_DUMP_FORMAT_SPARSE_TREE ))

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

const std::string MetricUnit::m_metricShortNames[ METRIC_TYPE_MAX ] = {
#define METRIC( _enum, _short_name, _long_name, _applies_global, _applies_file, _applies_function, _applies_method, _cumulative, _description  ) _short_name ,
#include "metrics.def"
#undef  METRIC
};

const std::string MetricUnit::m_metricNames[ METRIC_TYPE_MAX ] = {
#define METRIC( _enum, _short_name, _long_name, _applies_global, _applies_file, _applies_function, _applies_method, _cumulative, _description  ) _long_name ,
#include "metrics.def"
#undef  METRIC
};

const bool MetricUnit::m_metricIsCumulative[ METRIC_TYPE_MAX ] = {
#define METRIC( _enum, _short_name, _long_name, _applies_global, _applies_file, _applies_function, _applies_method, _cumulative, _description  ) _cumulative ,
#include "metrics.def"
#undef  METRIC
};

const bool MetricUnit::m_metricApplies[ METRIC_UNIT_MAX ][ METRIC_TYPE_MAX ] = {
	{
#define METRIC( _enum, _short_name, _long_name, _applies_global, _applies_file, _applies_function, _applies_method, _cumulative, _description  ) _applies_global ,
#include "metrics.def"
#undef  METRIC
	},
	{
#define METRIC( _enum, _short_name, _long_name, _applies_global, _applies_file, _applies_function, _applies_method, _cumulative, _description  ) _applies_file ,
#include "metrics.def"
#undef  METRIC
	},
	{
#define METRIC( _enum, _short_name, _long_name, _applies_global, _applies_file, _applies_function, _applies_method, _cumulative, _description  ) _applies_function ,
#include "metrics.def"
#undef  METRIC
	},
	{
#define METRIC( _enum, _short_name, _long_name, _applies_global, _applies_file, _applies_function, _applies_method, _cumulative, _description  ) _applies_method ,
#include "metrics.def"
#undef  METRIC
	}
};

MetricUnit::MetricUnit( MetricUnit* const p_parent, const std::string& p_name, const MetricUnitType_e p_type ) : 
					m_name( p_name ), m_type( p_type ), m_parent( p_parent ), m_processed( false )
{
	uint16_t loop;

	for( loop = 0;
		 loop < METRIC_TYPE_MAX;
		 loop++ )
	{
		m_counters[ loop ] = 0;
	}
}

void MetricUnit::increment( const MetricType_e p_metricType, const counter_t p_inc )
{
	/* TODO: Handle over-flow */
	m_counters[ p_metricType ]+= p_inc;
}

void MetricUnit::set( const MetricType_e p_metricType, const MetricUnit::counter_t p_val )
{
	m_counters[ p_metricType ] = p_val;
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
	    case METRIC_TYPE_OPERATOR_TYPES:
			/* TODO: This doesn't deal with C++ only operators */
			ret_val = (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_ASSIGN) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_ADDITION) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_SUBTRACTION) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_UNARY_PLUS) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_UNARY_MINUS) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_MULTIPLICATION) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_DIVISION) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_MODULO) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_INCREMENT_PRE ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_INCREMENT_POST ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_DECREMENT_PRE ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_DECREMENT_POST ) > 0) +

				      (getCounter(METRIC_TYPE_OPERATOR_COMP_EQUAL) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_COMP_NOT_EQUAL) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_COMP_GREATER_THAN) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_COMP_LESS_THAN) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_COMP_GREATER_THAN_EQUAL) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_COMP_LESS_THAN_EQUAL) > 0) +

					  (getCounter(METRIC_TYPE_OPERATOR_LOGICAL_NOT) > 0) +
					  (getCounter(METRIC_TYPE_OPERATOR_LOGICAL_AND) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_LOGICAL_OR) > 0) +

					  (getCounter(METRIC_TYPE_OPERATOR_BITWISE_NOT) > 0) +
					  (getCounter(METRIC_TYPE_OPERATOR_BITWISE_AND) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_BITWISE_OR) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_BITWISE_XOR) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_SHIFT_LEFT) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_SHIFT_RIGHT) > 0) +

					  (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_ADDITION_ASSIGN) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_SUBTRACTION_ASSIGN) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_MULTIPLICATION_ASSIGN) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_DIVISION_ASSIGN) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_MODULO_ASSIGN) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_BITWISE_AND_ASSIGN) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_BITWISE_OR_ASSIGN) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_BITWISE_XOR_ASSIGN) > 0) +
					  (getCounter(METRIC_TYPE_OPERATOR_SHIFT_LEFT_ASSIGN) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_SHIFT_RIGHT_ASSIGN) > 0) +

				      (getCounter(METRIC_TYPE_OPERATOR_ARRAY_SUBSCRIPT) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_DEREFERENCE) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ADDRESS_OF) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_MEMBER_ACCESS_POINTER) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_MEMBER_ACCESS_DIRECT) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_PTR_TO_MEMBER_INDIRECT) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_PTR_TO_MEMBER_DIRECT) > 0) +

					  (getCounter(METRIC_TYPE_FUNCTION_CALLS ) > 0) +
					  (getCounter(METRIC_TYPE_OPERATOR_COMMA) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_TERNARY ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_SIZE_OF ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ALIGN_OF ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_CAST ) > 0);
		break;
		case METRIC_TYPE_MODIFIED_CYCLOMATIC:
			if(( m_type == METRIC_UNIT_FUNCTION ) ||
			   ( m_type == METRIC_UNIT_METHOD ))
			{
				ret_val = getCounter( METRIC_TYPE_FORLOOP ) +
						  getCounter( METRIC_TYPE_IF ) +
						  getCounter( METRIC_TYPE_WHILELOOP ) +
						  getCounter( METRIC_TYPE_SWITCH ) +
						  getCounter( METRIC_TYPE_OPERATOR_LOGICAL_AND ) +
						  getCounter( METRIC_TYPE_OPERATOR_LOGICAL_OR ) +
						  getCounter( METRIC_TYPE_OPERATOR_TERNARY ) + 1;
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
						  getCounter( METRIC_TYPE_OPERATOR_LOGICAL_AND ) +
						  getCounter( METRIC_TYPE_OPERATOR_LOGICAL_OR ) +
						  getCounter( METRIC_TYPE_OPERATOR_TERNARY ) + 1;
			}
			break;
		case METRIC_TYPE_VOCF:
			break;
		default:
			ret_val  = m_counters[ p_metricType ];
			if( m_metricIsCumulative[ p_metricType ] )
			{
				for( SubUnitMap_t::const_iterator unitIt = m_subUnits.begin();
					 unitIt != m_subUnits.end();
					 ++unitIt )
				{
					ret_val += (*unitIt).second->getCounter( p_metricType );
				}
			}
			break;
	}

	return ret_val;
}

void MetricUnit::dump( std::ostream& out, const bool p_output[ METRIC_UNIT_MAX ], const MetricDumpFormat_e p_fmt, const MetricOptions* const p_options ) const
{
	std::string sep;

	if( p_output[ m_type ] ) 
	{
		switch( p_fmt ) {
		case METRIC_DUMP_FORMAT_TREE:
		case METRIC_DUMP_FORMAT_SPARSE_TREE:
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
				out << "Name" << sep;
				unsigned loop;
				for( loop = 0;
					 loop < METRIC_TYPE_MAX;

					 loop++ )
				{
					if( SHOULD_INCLUDE_METRIC( p_options, m_metricShortNames[ loop ] ) )
					{
						out << m_metricNames[loop] << sep;
					}
				}
				out << std::endl;
			}
		}

		if( IS_OUTPUT_TREE_FORM ( p_fmt )) {
			out << m_namePrefix[m_type];
		} 

		out << m_name << sep;

		unsigned loop;
		for( loop = 0;
			 loop < METRIC_TYPE_MAX;
			 loop++ )
		{
			/* Filter out metrics which only apply at file/method level */
			if( SHOULD_INCLUDE_METRIC( p_options, m_metricShortNames[ loop ] ) && m_metricApplies[ m_type ][ loop ])
			{
				counter_t val = getCounter((MetricType_e) loop );

				if(( p_fmt != METRIC_DUMP_FORMAT_SPARSE_TREE ) ||
				   ( val != 0 )) 
				{
					if( IS_OUTPUT_TREE_FORM( p_fmt )) {
						out << m_dumpPrefix[ m_type ] << m_metricNames[loop] << ": ";
					} 
					out << val << sep;
				}
			} else if( !IS_OUTPUT_TREE_FORM( p_fmt ))
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
		(*unitIt).second->dump( out, p_output, p_fmt, p_options );
	}
}

MetricUnit* MetricUnit::getSubUnit( const std::string& p_name, const MetricUnitType_e p_type )
{
	MetricUnit* ret_val = NULL;
	SubUnitMap_t::iterator name_it = m_subUnits.find( p_name );
	if( name_it == m_subUnits.end() )
	{
		ret_val = new MetricUnit( this, p_name, p_type );

		if(( p_type == METRIC_UNIT_FUNCTION ) ||
		   ( p_type == METRIC_UNIT_METHOD ))
		{
			/* By default, every function/method has 1 return point */
			ret_val->increment( METRIC_TYPE_RETURNPOINTS );
		}
		else if( p_type == METRIC_UNIT_FILE )
		{
			this->increment( METRIC_TYPE_FILES );
		}
		m_subUnits[ p_name ] = ret_val;
	} else {
		ret_val = (*name_it).second;
	}
	return ret_val;
}

bool MetricUnit::hasBeenProcessed( void ) const
{
	return m_processed;
}

void MetricUnit::setProcessed( void )
{
	m_processed = true;
}

bool MetricUnit::isFnOrMethod( void ) const
{
	return(( m_type == METRIC_UNIT_FUNCTION ) ||
		   ( m_type == METRIC_UNIT_METHOD ));
}
