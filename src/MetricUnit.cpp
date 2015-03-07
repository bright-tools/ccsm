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

#include <iostream>
#include <iomanip>

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
#define METRIC( _enum, _short_name, _long_name, _applies_global, _applies_file, _applies_function, _applies_method, _cumulative, _multipass, _report_local, _scaling, _description  ) _short_name ,
#include "metrics.def"
#undef  METRIC
};

const std::string MetricUnit::m_metricNames[ METRIC_TYPE_MAX ] = {
#define METRIC( _enum, _short_name, _long_name, _applies_global, _applies_file, _applies_function, _applies_method, _cumulative, _multipass, _report_local, _scaling, _description  ) _long_name ,
#include "metrics.def"
#undef  METRIC
};

const bool MetricUnit::m_metricMultipass[ METRIC_TYPE_MAX ] = {
#define METRIC( _enum, _short_name, _long_name, _applies_global, _applies_file, _applies_function, _applies_method, _cumulative, _multipass, _report_local, _scaling, _description  ) _multipass ,
#include "metrics.def"
#undef  METRIC
};


const bool MetricUnit::m_metricIsCumulative[ METRIC_TYPE_MAX ] = {
#define METRIC( _enum, _short_name, _long_name, _applies_global, _applies_file, _applies_function, _applies_method, _cumulative, _multipass, _report_local, _scaling, _description  ) _cumulative ,
#include "metrics.def"
#undef  METRIC
};

const bool MetricUnit::m_metricReportLocal[ METRIC_TYPE_MAX ] = {
#define METRIC( _enum, _short_name, _long_name, _applies_global, _applies_file, _applies_function, _applies_method, _cumulative, _multipass, _report_local, _scaling, _description  ) _report_local ,
#include "metrics.def"
#undef  METRIC
};

const uint32_t MetricUnit::m_metricScaling[ METRIC_TYPE_MAX ] = {
#define METRIC( _enum, _short_name, _long_name, _applies_global, _applies_file, _applies_function, _applies_method, _cumulative, _multipass, _report_local, _scaling, _description  ) _scaling ,
#include "metrics.def"
#undef  METRIC
};


const bool MetricUnit::m_metricApplies[ METRIC_UNIT_MAX ][ METRIC_TYPE_MAX ] = {
	{
#define METRIC( _enum, _short_name, _long_name, _applies_global, _applies_file, _applies_function, _applies_method, _cumulative, _multipass, _report_local, _scaling, _description  ) _applies_global ,
#include "metrics.def"
#undef  METRIC
	},
	{
#define METRIC( _enum, _short_name, _long_name, _applies_global, _applies_file, _applies_function, _applies_method, _cumulative, _multipass, _report_local, _scaling, _description  ) _applies_file ,
#include "metrics.def"
#undef  METRIC
	},
	{
#define METRIC( _enum, _short_name, _long_name, _applies_global, _applies_file, _applies_function, _applies_method, _cumulative, _multipass, _report_local, _scaling, _description  ) _applies_function ,
#include "metrics.def"
#undef  METRIC
	},
	{
#define METRIC( _enum, _short_name, _long_name, _applies_global, _applies_file, _applies_function, _applies_method, _cumulative, _multipass, _report_local, _scaling, _description  ) _applies_method ,
#include "metrics.def"
#undef  METRIC
	}
};

const uint16_t MetricUnit::counter_t_Max = UINT16_MAX;


MetricUnit::MetricUnit( MetricUnit* const p_parent, const std::string& p_name, const MetricUnitType_e p_type ) : 
					m_name( p_name ), m_type( p_type ), m_parent( p_parent )
{
	uint16_t loop;

	for( loop = 0;
		 loop < METRIC_UNIT_PROCESS_MAX;
		 loop++ )
	{
		m_processed[ loop ] = false;
	}

	for( loop = 0;
		 loop < METRIC_TYPE_MAX;
		 loop++ )
	{
		m_counters[ loop ] = 0;
	}
}

void MetricUnit::increment( const MetricType_e p_metricType, const counter_t p_inc )
{
	/* Check for over-flow */
	if(( counter_t_Max - p_inc ) > m_counters[ p_metricType ] )
	{
		m_counters[ p_metricType ]+= p_inc;
	} 
	else
	{
		/* Protect against spamming out the same message over and over again */
		if( m_counters[ p_metricType ] != counter_t_Max )
		{
			std::cerr << "Warning: Overflow of metric '" << p_metricType << "' within " << m_name <<  std::endl;
		}
		m_counters[ p_metricType ] = counter_t_Max;
	}
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

MetricUnit::counter_t MetricUnit::getCounter( const MetricType_e p_metricType, const bool p_recurse ) const
{
	counter_t ret_val = 0;

	switch( p_metricType ) 
	{
	    case METRIC_TYPE_OPERATOR_TYPES:
			/* TODO: This doesn't deal with C++ only operators */
			ret_val = (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_ASSIGN, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_ADDITION, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_SUBTRACTION, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_UNARY_PLUS, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_UNARY_MINUS, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_MULTIPLICATION, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_DIVISION, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_MODULO, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_INCREMENT_PRE, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_INCREMENT_POST, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_DECREMENT_PRE, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_DECREMENT_POST, p_recurse ) > 0) +

				      (getCounter(METRIC_TYPE_OPERATOR_COMP_EQUAL, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_COMP_NOT_EQUAL, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_COMP_GREATER_THAN, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_COMP_LESS_THAN, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_COMP_GREATER_THAN_EQUAL, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_COMP_LESS_THAN_EQUAL, p_recurse ) > 0) +

					  (getCounter(METRIC_TYPE_OPERATOR_LOGICAL_NOT, p_recurse ) > 0) +
					  (getCounter(METRIC_TYPE_OPERATOR_LOGICAL_AND, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_LOGICAL_OR, p_recurse ) > 0) +

					  (getCounter(METRIC_TYPE_OPERATOR_BITWISE_NOT, p_recurse ) > 0) +
					  (getCounter(METRIC_TYPE_OPERATOR_BITWISE_AND, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_BITWISE_OR, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_BITWISE_XOR, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_SHIFT_LEFT, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_SHIFT_RIGHT, p_recurse ) > 0) +

					  (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_ADDITION_ASSIGN, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_SUBTRACTION_ASSIGN, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_MULTIPLICATION_ASSIGN, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_DIVISION_ASSIGN, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ARITHMETIC_MODULO_ASSIGN, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_BITWISE_AND_ASSIGN, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_BITWISE_OR_ASSIGN, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_BITWISE_XOR_ASSIGN, p_recurse ) > 0) +
					  (getCounter(METRIC_TYPE_OPERATOR_SHIFT_LEFT_ASSIGN, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_SHIFT_RIGHT_ASSIGN, p_recurse ) > 0) +

				      (getCounter(METRIC_TYPE_OPERATOR_ARRAY_SUBSCRIPT, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_DEREFERENCE, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ADDRESS_OF, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_MEMBER_ACCESS_POINTER, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_MEMBER_ACCESS_DIRECT, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_PTR_TO_MEMBER_INDIRECT, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_PTR_TO_MEMBER_DIRECT, p_recurse ) > 0) +

					  (getCounter(METRIC_TYPE_FUNCTION_CALLS, p_recurse ) > 0) +
					  (getCounter(METRIC_TYPE_OPERATOR_COMMA, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_TERNARY, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_SIZE_OF, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_ALIGN_OF, p_recurse ) > 0) +
				      (getCounter(METRIC_TYPE_OPERATOR_CAST, p_recurse ) > 0);
			break;
		case METRIC_TYPE_HIS_COMMENT_DENSITY:
			{
				counter_t stmtCount = getCounter( METRIC_TYPE_HIS_STATEMENT, p_recurse );
				/* Div-by-zero protection */
				if( stmtCount > 0 )
				{
					ret_val = ((float)getCounter( METRIC_TYPE_COMMENT_COUNT, p_recurse ) / stmtCount) * 1000;
					/* Clip at 1.0 */
					if( ret_val > 1000 )
					{
						ret_val = 1000;
					}
				}
				else
				{
					ret_val = 0;
				}
			}
			break;
		case METRIC_TYPE_TOK_MODIFIED_CYCLOMATIC:
			if( isFnOrMethod() )
			{
				ret_val = getCounter( METRIC_TYPE_TOKEN_FOR, p_recurse ) +
						  getCounter( METRIC_TYPE_TOKEN_IF, p_recurse ) +
						  getCounter( METRIC_TYPE_TOKEN_WHILE, p_recurse ) +
						  getCounter( METRIC_TYPE_TOKEN_SWITCH, p_recurse ) +
						  getCounter( METRIC_TYPE_TOKEN_AMPAMP, p_recurse ) +
						  getCounter( METRIC_TYPE_TOKEN_PIPEPIPE, p_recurse ) +
						  getCounter( METRIC_TYPE_TOKEN_QUESTION, p_recurse ) + 1;
			}
			break;
		case METRIC_TYPE_MODIFIED_CYCLOMATIC:
			if( isFnOrMethod() )
			{
				ret_val = getCounter( METRIC_TYPE_FORLOOP, p_recurse ) +
						  getCounter( METRIC_TYPE_IF, p_recurse ) +
						  getCounter( METRIC_TYPE_WHILELOOP, p_recurse ) +
						  getCounter( METRIC_TYPE_SWITCH, p_recurse ) +
						  getCounter( METRIC_TYPE_OPERATOR_LOGICAL_AND, p_recurse ) +
						  getCounter( METRIC_TYPE_OPERATOR_LOGICAL_OR, p_recurse ) +
						  getCounter( METRIC_TYPE_OPERATOR_TERNARY, p_recurse ) + 1;
			}
			break;
		case METRIC_TYPE_TOK_CYCLOMATIC:
			if( isFnOrMethod() )
			{
				ret_val = getCounter( METRIC_TYPE_TOKEN_FOR, p_recurse ) +
						  getCounter( METRIC_TYPE_TOKEN_IF, p_recurse ) +
						  getCounter( METRIC_TYPE_TOKEN_WHILE, p_recurse ) +
						  getCounter( METRIC_TYPE_TOKEN_CASE, p_recurse ) +
						  getCounter( METRIC_TYPE_TOKEN_AMPAMP, p_recurse ) +
						  getCounter( METRIC_TYPE_TOKEN_PIPEPIPE, p_recurse ) +
						  getCounter( METRIC_TYPE_TOKEN_QUESTION, p_recurse ) + 1;
			}
			break;
		case METRIC_TYPE_CYCLOMATIC:
			if( isFnOrMethod() )
			{
				// TODO: What about the 'DEFAULT'?
				ret_val = getCounter( METRIC_TYPE_FORLOOP, p_recurse ) +
						  getCounter( METRIC_TYPE_IF, p_recurse ) +
						  getCounter( METRIC_TYPE_WHILELOOP, p_recurse ) +
						  getCounter( METRIC_TYPE_CASE, p_recurse ) +
						  getCounter( METRIC_TYPE_OPERATOR_LOGICAL_AND, p_recurse ) +
						  getCounter( METRIC_TYPE_OPERATOR_LOGICAL_OR, p_recurse ) +
						  getCounter( METRIC_TYPE_OPERATOR_TERNARY, p_recurse ) + 1;
			}
			break;
		case METRIC_TYPE_VOCF:
			break;
		default:
			ret_val  = m_counters[ p_metricType ];
			if( p_recurse )
			{
				for( SubUnitMap_t::const_iterator unitIt = m_subUnits.begin();
					 unitIt != m_subUnits.end();
					 ++unitIt )
				{
					ret_val += (*unitIt).second->getCounter( p_metricType, p_recurse );
				}
			}
			break;
	}

	return ret_val;
}

void MetricUnit::dumpMetric( std::ostream& out, const MetricType_e p_metric, const MetricDumpFormat_e p_fmt, const std::string& p_sep, const bool p_recurse ) const
{
	counter_t val = getCounter( p_metric, p_recurse );

	if(( p_fmt != METRIC_DUMP_FORMAT_SPARSE_TREE ) ||
		( val != 0 )) 
	{
		if( IS_OUTPUT_TREE_FORM( p_fmt )) {
			out << m_dumpPrefix[ m_type ] << m_metricNames[ p_metric ] << ": ";
		} 
		if( m_metricScaling[ p_metric ] == 1 )
		{
			out << val << p_sep;
		}
		else
		{
			out << std::setprecision(6) << ((float)(val)) / m_metricScaling[ p_metric ] << p_sep;
		}
	}
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
					const bool localAndCumulativeOutputs = m_metricIsCumulative[ loop ] && m_metricReportLocal[ loop ];

					if( SHOULD_INCLUDE_METRIC( p_options, m_metricShortNames[ loop ] ) )
					{
						out << m_metricNames[loop];

						if( localAndCumulativeOutputs )
						{
							out << "(local)" << sep << m_metricNames[loop] << "(cumulative)";
						
						}
						out << sep;
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
			/* TODO: duped above */
			const bool localAndCumulativeOutputs = m_metricIsCumulative[ loop ] && m_metricReportLocal[ loop ];

			/* Filter out metrics which only apply at file/method level */
			if( SHOULD_INCLUDE_METRIC( p_options, m_metricShortNames[ loop ] ) && m_metricApplies[ m_type ][ loop ])
			{
				if( localAndCumulativeOutputs )
				{
					dumpMetric( out, (MetricType_e) loop, p_fmt, sep, false );
				}
				dumpMetric( out, (MetricType_e) loop, p_fmt, sep, m_metricIsCumulative[ loop ] );
			} 
			else if( !IS_OUTPUT_TREE_FORM( p_fmt ))
			{
				out << sep;
				if( localAndCumulativeOutputs )
				{
					out << sep;
				}
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

MetricUnit* MetricUnit::getSubUnit( const std::string& p_name, const MetricUnitType_e p_type, const bool p_create )
{
	MetricUnit* ret_val = NULL;
	SubUnitMap_t::iterator name_it = m_subUnits.find( p_name );
	if( name_it == m_subUnits.end() )
	{
		if( p_create )
		{
			ret_val = new MetricUnit( this, p_name, p_type );

			if( ret_val->isFnOrMethod() )
			{
				/* By default, every function/method has 1 return point */
				ret_val->increment( METRIC_TYPE_RETURNPOINTS );
			}
			else if( p_type == METRIC_UNIT_FILE )
			{
				this->increment( METRIC_TYPE_FILES );
			}
			m_subUnits[ p_name ] = ret_val;
		}
	} else {
		ret_val = (*name_it).second;
	}
	return ret_val;
}

bool MetricUnit::hasBeenProcessed( const MetricUnitProcessingType_e p_type ) const
{
	return m_processed[ p_type ];
}

void MetricUnit::setProcessed( const MetricUnitProcessingType_e p_type )
{
	m_processed[ p_type ] = true;
}

bool MetricUnit::isFnOrMethod( void ) const
{
	return(( m_type == METRIC_UNIT_FUNCTION ) ||
		   ( m_type == METRIC_UNIT_METHOD ));
}

bool MetricUnit::isMultiPassAllowed( const MetricType_e p_type )
{
	return m_metricMultipass[ p_type ];
}

std::string MetricUnit::getMetricName( const MetricType_e p_type )
{
	return m_metricNames[ p_type ];
}
