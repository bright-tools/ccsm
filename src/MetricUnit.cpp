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
#include "MetricOptions.hpp"
#include "MetricUtils.hpp"

#include "llvm/Support/FileSystem.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/Path.h"

#include <iostream>
#include <sstream>
#include <iomanip>

const std::string MetricUnit::m_metricShortNames[ METRIC_TYPE_MAX ] = {
#define METRIC_ALIAS( _name, _alias )
#define METRIC( _enum, _short_name, _long_name, _applies_global, _applies_file, _applies_function, _applies_method, _cumulative, _multipass, _report_local, _scaling, _description  ) _short_name ,
#include "metrics.def"
#undef  METRIC
#undef  METRIC_ALIAS
};

const std::string MetricUnit::m_metricNames[ METRIC_TYPE_MAX ] = {
#define METRIC_ALIAS( _name, _alias )
#define METRIC( _enum, _short_name, _long_name, _applies_global, _applies_file, _applies_function, _applies_method, _cumulative, _multipass, _report_local, _scaling, _description  ) _long_name ,
#include "metrics.def"
#undef  METRIC
#undef  METRIC_ALIAS
};

const bool MetricUnit::m_metricMultipass[ METRIC_TYPE_MAX ] = {
#define METRIC_ALIAS( _name, _alias )
#define METRIC( _enum, _short_name, _long_name, _applies_global, _applies_file, _applies_function, _applies_method, _cumulative, _multipass, _report_local, _scaling, _description  ) _multipass ,
#include "metrics.def"
#undef  METRIC
#undef  METRIC_ALIAS
};

const bool MetricUnit::m_metricIsCumulative[ METRIC_TYPE_MAX ] = {
#define METRIC_ALIAS( _name, _alias )
#define METRIC( _enum, _short_name, _long_name, _applies_global, _applies_file, _applies_function, _applies_method, _cumulative, _multipass, _report_local, _scaling, _description  ) _cumulative ,
#include "metrics.def"
#undef  METRIC
#undef  METRIC_ALIAS
};

const bool MetricUnit::m_metricReportLocal[ METRIC_TYPE_MAX ] = {
#define METRIC_ALIAS( _name, _alias )
#define METRIC( _enum, _short_name, _long_name, _applies_global, _applies_file, _applies_function, _applies_method, _cumulative, _multipass, _report_local, _scaling, _description  ) _report_local ,
#include "metrics.def"
#undef  METRIC
#undef  METRIC_ALIAS
};

const uint32_t MetricUnit::m_metricScaling[ METRIC_TYPE_MAX ] = {
#define METRIC_ALIAS( _name, _alias )
#define METRIC( _enum, _short_name, _long_name, _applies_global, _applies_file, _applies_function, _applies_method, _cumulative, _multipass, _report_local, _scaling, _description  ) _scaling ,
#include "metrics.def"
#undef  METRIC
#undef  METRIC_ALIAS
};

const bool MetricUnit::m_metricApplies[ METRIC_UNIT_MAX ][ METRIC_TYPE_MAX ] = {
	{
#define METRIC_ALIAS( _name, _alias )
#define METRIC( _enum, _short_name, _long_name, _applies_global, _applies_file, _applies_function, _applies_method, _cumulative, _multipass, _report_local, _scaling, _description  ) _applies_global ,
#include "metrics.def"
#undef  METRIC
#undef  METRIC_ALIAS
	},
	{
#define METRIC_ALIAS( _name, _alias )
#define METRIC( _enum, _short_name, _long_name, _applies_global, _applies_file, _applies_function, _applies_method, _cumulative, _multipass, _report_local, _scaling, _description  ) _applies_file ,
#include "metrics.def"
#undef  METRIC
#undef  METRIC_ALIAS
	},
	{
#define METRIC_ALIAS( _name, _alias )
#define METRIC( _enum, _short_name, _long_name, _applies_global, _applies_file, _applies_function, _applies_method, _cumulative, _multipass, _report_local, _scaling, _description  ) _applies_function ,
#include "metrics.def"
#undef  METRIC
#undef  METRIC_ALIAS
	},
	{
#define METRIC_ALIAS( _name, _alias )
#define METRIC( _enum, _short_name, _long_name, _applies_global, _applies_file, _applies_function, _applies_method, _cumulative, _multipass, _report_local, _scaling, _description  ) _applies_method ,
#include "metrics.def"
#undef  METRIC
#undef  METRIC_ALIAS
	}
};

const uint32_t MetricUnit::counter_t_Max = UINT32_MAX;


MetricUnit::MetricUnit( MetricUnit* const p_parent, const std::string& p_name, const MetricUnitType_e p_type ) : 
		m_name(p_name), m_type(p_type), 
		m_parent(p_parent), m_hasExternalLinkage( false )
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

void MetricUnit::addUnresolvedFn(const std::string& p_fnName)
{
	m_unresolvedFnCalls.insert(p_fnName);
}

const std::set<std::string>& MetricUnit::getUnresolvedFns() const
{
	return m_unresolvedFnCalls;
}

void MetricUnit::setExternalLinkage(const bool p_isExternal)
{
	m_hasExternalLinkage = p_isExternal;
}

MetricUnit::FunctionMap_t MetricUnit::getAllFunctionMap(void)
{
	FunctionMap_t retVal;

	for (SubUnitMap_t::const_iterator fileIt = m_subUnits.begin();
		fileIt != m_subUnits.end();
		++fileIt)
	{
		if ((*fileIt).second->GetType() == METRIC_UNIT_FILE)
		{
			for (SubUnitMap_t::const_iterator funcIt = (*fileIt).second->m_subUnits.begin();
				funcIt != (*fileIt).second->m_subUnits.end();
				++funcIt)
			{
				retVal[(*funcIt).first] = (*funcIt).second;
			}
		}
	}


	return retVal;
}

bool MetricUnit::hasExternalLinkage(void) const
{
	return m_hasExternalLinkage;
}


void MetricUnit::setMax(const MetricType_e p_metricType, const MetricUnit::counter_t p_val)
{
	if (p_val > m_counters[p_metricType])
	{
		m_counters[p_metricType] = p_val;
	}
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

/* TODO: This doesn't deal with C++ only operators */
const std::set<MetricType_e> m_halsteadOperatorMetrics = {
	METRIC_TYPE_OPERATOR_ARITHMETIC_ASSIGN,
	METRIC_TYPE_OPERATOR_ARITHMETIC_ADDITION,
	METRIC_TYPE_OPERATOR_ARITHMETIC_SUBTRACTION,
	METRIC_TYPE_OPERATOR_ARITHMETIC_UNARY_PLUS,
	METRIC_TYPE_OPERATOR_ARITHMETIC_UNARY_MINUS,
	METRIC_TYPE_OPERATOR_ARITHMETIC_MULTIPLICATION,
	METRIC_TYPE_OPERATOR_ARITHMETIC_DIVISION,
	METRIC_TYPE_OPERATOR_ARITHMETIC_MODULO,
	METRIC_TYPE_OPERATOR_ARITHMETIC_INCREMENT_PRE,
	METRIC_TYPE_OPERATOR_ARITHMETIC_INCREMENT_POST,
	METRIC_TYPE_OPERATOR_ARITHMETIC_DECREMENT_PRE,
	METRIC_TYPE_OPERATOR_ARITHMETIC_DECREMENT_POST,

	METRIC_TYPE_OPERATOR_COMP_EQUAL,
	METRIC_TYPE_OPERATOR_COMP_NOT_EQUAL,
	METRIC_TYPE_OPERATOR_COMP_GREATER_THAN,
	METRIC_TYPE_OPERATOR_COMP_LESS_THAN,
	METRIC_TYPE_OPERATOR_COMP_GREATER_THAN_EQUAL,
	METRIC_TYPE_OPERATOR_COMP_LESS_THAN_EQUAL,

	METRIC_TYPE_OPERATOR_LOGICAL_NOT,
	METRIC_TYPE_OPERATOR_LOGICAL_AND,
	METRIC_TYPE_OPERATOR_LOGICAL_OR,

	METRIC_TYPE_OPERATOR_BITWISE_NOT,
	METRIC_TYPE_OPERATOR_BITWISE_AND,
	METRIC_TYPE_OPERATOR_BITWISE_OR,
	METRIC_TYPE_OPERATOR_BITWISE_XOR,
	METRIC_TYPE_OPERATOR_SHIFT_LEFT,
	METRIC_TYPE_OPERATOR_SHIFT_RIGHT,

	METRIC_TYPE_OPERATOR_ARITHMETIC_ADDITION_ASSIGN,
	METRIC_TYPE_OPERATOR_ARITHMETIC_SUBTRACTION_ASSIGN,
	METRIC_TYPE_OPERATOR_ARITHMETIC_MULTIPLICATION_ASSIGN,
	METRIC_TYPE_OPERATOR_ARITHMETIC_DIVISION_ASSIGN,
	METRIC_TYPE_OPERATOR_ARITHMETIC_MODULO_ASSIGN,
	METRIC_TYPE_OPERATOR_BITWISE_AND_ASSIGN,
	METRIC_TYPE_OPERATOR_BITWISE_OR_ASSIGN,
	METRIC_TYPE_OPERATOR_BITWISE_XOR_ASSIGN,
	METRIC_TYPE_OPERATOR_SHIFT_LEFT_ASSIGN,
	METRIC_TYPE_OPERATOR_SHIFT_RIGHT_ASSIGN,

	METRIC_TYPE_OPERATOR_ARRAY_SUBSCRIPT,
	METRIC_TYPE_OPERATOR_DEREFERENCE,
	METRIC_TYPE_OPERATOR_ADDRESS_OF,
	METRIC_TYPE_OPERATOR_MEMBER_ACCESS_POINTER,
	METRIC_TYPE_OPERATOR_MEMBER_ACCESS_DIRECT,
	METRIC_TYPE_OPERATOR_PTR_TO_MEMBER_INDIRECT,
	METRIC_TYPE_OPERATOR_PTR_TO_MEMBER_DIRECT,

	METRIC_TYPE_AUTO,
	METRIC_TYPE_EXTERN,
//	METRIC_TYPE_INLINE,
	METRIC_TYPE_REGISTER,
	METRIC_TYPE_STATIC,
	METRIC_TYPE_TYPEDEF,
//	METRIC_TYPE_VIRTUAL,
//  METRIC_TYPE_MUTABLE,

	METRIC_TYPE_BODY_CONST,
	// TODO: should be BODY_VOLATILE
	// 	METRIC_TYPE_FRIEND
	METRIC_TYPE_VOLATILE,

//	METRIC_TYPE_ASM,
	METRIC_TYPE_BREAK,
	METRIC_TYPE_CASE,
	//METRIC_TYPE_CLASS,
	METRIC_TYPE_DEFAULT,
	METRIC_TYPE_DO,
//	METRIC_TYPE_DELETE,
	METRIC_TYPE_ELSE,
	METRIC_TYPE_BODY_ENUM,
	METRIC_TYPE_FORLOOP,
	METRIC_TYPE_GOTO,
	METRIC_TYPE_IF,
	//METRIC_TYPE_NEW,
	//METRIC_TYPE_OPERATOR,
	//METRIC_TYPE_PRIVATE,
	//METRIC_TYPE_PROTECTED,
	//METRIC_TYPE_PUBLIC,
	METRIC_TYPE_RETURN,
	METRIC_TYPE_OPERATOR_SIZE_OF,
	METRIC_TYPE_STRUCT,
	METRIC_TYPE_SWITCH,
	//METRIC_TYPE_THIS,
	METRIC_TYPE_UNION,
	METRIC_TYPE_WHILE,
	//METRIC_TYPE_NAMESPACE,
	//METRIC_TYPE_USING,
	//METRIC_TYPE_TRY,
	//METRIC_TYPE_CATCH,
	//METRIC_TYPE_THROW,
	//METRIC_TYPE_CONST_CAST,
	//METRIC_TYPE_STATIC_CAST,
	//METRIC_TYPE_DYNAMIC_CAST,
	//METRIC_TYPE_REINTERPRET_CAST,
	//METRIC_TYPE_TYPEID,
	//METRIC_TYPE_TEMPLATE,
	//METRIC_TYPE_EXPLICIT,
	//METRIC_TYPE_TRUE,
	//METRIC_TYPE_FALSE,
	//METRIC_TYPE_TYPENAME,









	// TODO: This is an operator?
	// METRIC_TYPE_FUNCTION_CALLS,
	METRIC_TYPE_OPERATOR_COMMA,
	METRIC_TYPE_OPERATOR_TERNARY,
	METRIC_TYPE_OPERATOR_ALIGN_OF,
	METRIC_TYPE_OPERATOR_CAST
};

/* TODO: This doesn't deal with C++ only operators */
const std::set<MetricType_e> m_operatorMetrics = {
	METRIC_TYPE_OPERATOR_ARITHMETIC_ASSIGN,
	METRIC_TYPE_OPERATOR_ARITHMETIC_ADDITION,
	METRIC_TYPE_OPERATOR_ARITHMETIC_SUBTRACTION,
	METRIC_TYPE_OPERATOR_ARITHMETIC_UNARY_PLUS,
	METRIC_TYPE_OPERATOR_ARITHMETIC_UNARY_MINUS,
	METRIC_TYPE_OPERATOR_ARITHMETIC_MULTIPLICATION,
	METRIC_TYPE_OPERATOR_ARITHMETIC_DIVISION,
	METRIC_TYPE_OPERATOR_ARITHMETIC_MODULO,
	METRIC_TYPE_OPERATOR_ARITHMETIC_INCREMENT_PRE,
	METRIC_TYPE_OPERATOR_ARITHMETIC_INCREMENT_POST,
	METRIC_TYPE_OPERATOR_ARITHMETIC_DECREMENT_PRE,
	METRIC_TYPE_OPERATOR_ARITHMETIC_DECREMENT_POST,

	METRIC_TYPE_OPERATOR_COMP_EQUAL,
	METRIC_TYPE_OPERATOR_COMP_NOT_EQUAL,
	METRIC_TYPE_OPERATOR_COMP_GREATER_THAN,
	METRIC_TYPE_OPERATOR_COMP_LESS_THAN,
	METRIC_TYPE_OPERATOR_COMP_GREATER_THAN_EQUAL,
	METRIC_TYPE_OPERATOR_COMP_LESS_THAN_EQUAL,

	METRIC_TYPE_OPERATOR_LOGICAL_NOT,
	METRIC_TYPE_OPERATOR_LOGICAL_AND,
	METRIC_TYPE_OPERATOR_LOGICAL_OR,

	METRIC_TYPE_OPERATOR_BITWISE_NOT,
	METRIC_TYPE_OPERATOR_BITWISE_AND,
	METRIC_TYPE_OPERATOR_BITWISE_OR,
	METRIC_TYPE_OPERATOR_BITWISE_XOR,
	METRIC_TYPE_OPERATOR_SHIFT_LEFT,
	METRIC_TYPE_OPERATOR_SHIFT_RIGHT,

	METRIC_TYPE_OPERATOR_ARITHMETIC_ADDITION_ASSIGN,
	METRIC_TYPE_OPERATOR_ARITHMETIC_SUBTRACTION_ASSIGN,
	METRIC_TYPE_OPERATOR_ARITHMETIC_MULTIPLICATION_ASSIGN,
	METRIC_TYPE_OPERATOR_ARITHMETIC_DIVISION_ASSIGN,
	METRIC_TYPE_OPERATOR_ARITHMETIC_MODULO_ASSIGN,
	METRIC_TYPE_OPERATOR_BITWISE_AND_ASSIGN,
	METRIC_TYPE_OPERATOR_BITWISE_OR_ASSIGN,
	METRIC_TYPE_OPERATOR_BITWISE_XOR_ASSIGN,
	METRIC_TYPE_OPERATOR_SHIFT_LEFT_ASSIGN,
	METRIC_TYPE_OPERATOR_SHIFT_RIGHT_ASSIGN,

	METRIC_TYPE_OPERATOR_ARRAY_SUBSCRIPT,
	METRIC_TYPE_OPERATOR_DEREFERENCE,
	METRIC_TYPE_OPERATOR_ADDRESS_OF,
	METRIC_TYPE_OPERATOR_MEMBER_ACCESS_POINTER,
	METRIC_TYPE_OPERATOR_MEMBER_ACCESS_DIRECT,
	METRIC_TYPE_OPERATOR_PTR_TO_MEMBER_INDIRECT,
	METRIC_TYPE_OPERATOR_PTR_TO_MEMBER_DIRECT,

	// TODO: This is an operator?
	METRIC_TYPE_FUNCTION_CALLS,
	METRIC_TYPE_OPERATOR_COMMA,
	METRIC_TYPE_OPERATOR_TERNARY,
	METRIC_TYPE_OPERATOR_SIZE_OF,
	METRIC_TYPE_OPERATOR_ALIGN_OF,
	METRIC_TYPE_OPERATOR_CAST
};

/* TODO: This doesn't deal with C99/C++ only keywoards */
const std::set<MetricType_e> m_keywordMetrics = {
	METRIC_TYPE_VARIABLE_FN_AUTO,
	METRIC_TYPE_BREAK,
	METRIC_TYPE_CASE,
	METRIC_TYPE_CHAR,
	METRIC_TYPE_CONST,
	METRIC_TYPE_CONTINUE,
	METRIC_TYPE_DEFAULT,
	METRIC_TYPE_DO,
	METRIC_TYPE_DOUBLE,
	METRIC_TYPE_ELSE,
	METRIC_TYPE_ENUM,
	METRIC_TYPE_VARIABLE_FN_EXTERN,
	METRIC_TYPE_VARIABLE_FILE_EXTERN,
	METRIC_TYPE_FLOAT,
	METRIC_TYPE_FORLOOP,
	METRIC_TYPE_GOTO,
	METRIC_TYPE_IF,
//	METRIC_TYPE_INLINE, // TODO C99
	METRIC_TYPE_INT,
	METRIC_TYPE_LONG,
	METRIC_TYPE_VARIABLE_FN_REGISTER,
//	METRIC_TYPE_RESTRICT, // TODO C99
	METRIC_TYPE_RETURN,
	METRIC_TYPE_SHORT,
	METRIC_TYPE_SIGNED,
	METRIC_TYPE_OPERATOR_SIZE_OF,
	METRIC_TYPE_STATIC,
	METRIC_TYPE_STRUCT,
	METRIC_TYPE_SWITCH,
	METRIC_TYPE_TYPEDEF,
	METRIC_TYPE_UNION,
	METRIC_TYPE_UNSIGNED,
	METRIC_TYPE_VOID,
	METRIC_TYPE_VOLATILE,
	METRIC_TYPE_WHILE
	// TODO - no _Bool, _Complex or _Imaginary
};

/* TODO: This doesn't deal with C99/C++ only keywoards */
const std::set<MetricType_e> m_halsteadOperandMetrics = {
	METRIC_TYPE_BODY_CHAR,
	METRIC_TYPE_BODY_DOUBLE,
	METRIC_TYPE_BODY_FLOAT,
	METRIC_TYPE_BODY_INT,
	METRIC_TYPE_BODY_LONG,
	METRIC_TYPE_BODY_SHORT,
	METRIC_TYPE_BODY_SIGNED,
	METRIC_TYPE_BODY_UNSIGNED,
	METRIC_TYPE_BODY_VOID
	// TODO
	// TODO - no _Bool, _Complex or _Imaginary
};

MetricUnit::counter_t MetricUnit::getCounter( const MetricType_e p_metricType, const bool p_recurse ) const
{
	counter_t ret_val = 0;

	switch( p_metricType ) 
	{
		case METRIC_TYPE_HIS_CALLING:
			ret_val = getCounter(METRIC_TYPE_CALLED_BY_LOCAL, p_recurse) +
				      getCounter(METRIC_TYPE_CALLED_BY_EXTERN, p_recurse);
			break;
		case METRIC_TYPE_OPERATOR_COUNT:
			for (std::set<MetricType_e>::const_iterator it = m_operatorMetrics.begin();
				it != m_operatorMetrics.end();
				it++)
			{
				ret_val += getCounter(*it, p_recurse);
			}
			break;
		case METRIC_TYPE_KEYWORD_CNT:
			for (std::set<MetricType_e>::const_iterator it = m_keywordMetrics.begin();
				it != m_keywordMetrics.end();
				it++)
			{
				ret_val += getCounter(*it, p_recurse);
			}
			break;
		case METRIC_TYPE_KEYWORD_TYPES:
			for (std::set<MetricType_e>::const_iterator it = m_keywordMetrics.begin();
				it != m_keywordMetrics.end();
				it++)
			{
				ret_val += (getCounter(*it, p_recurse) > 0);
			}
			break;
		case METRIC_TYPE_OPERAND_HALSTEAD_COUNT:
			for (std::set<MetricType_e>::const_iterator it = m_halsteadOperandMetrics.begin();
				it != m_halsteadOperandMetrics.end();
				it++)
			{
				ret_val += getCounter(*it, p_recurse);
			}
			ret_val += getCounter(METRIC_TYPE_NUMERIC_CONSTANTS, p_recurse);
			ret_val += getCounter(METRIC_TYPE_CHAR_CONSTS, p_recurse);
			ret_val += getCounter(METRIC_TYPE_STRING_LITERALS, p_recurse);
			ret_val += getCounter(METRIC_TYPE_BODY_UNRESERVED_IDENTIFIERS, p_recurse);
			break;
		case METRIC_TYPE_OPERAND_HALSTEAD_UNIQUE:
			for (std::set<MetricType_e>::const_iterator it = m_halsteadOperandMetrics.begin();
				it != m_halsteadOperandMetrics.end();
				it++)
			{
				ret_val += (getCounter(*it, p_recurse) > 0);
			}
			ret_val += getCounter(METRIC_TYPE_NUMERIC_CONSTANTS_UNIQ, p_recurse);
			ret_val += getCounter(METRIC_TYPE_CHAR_CONSTS_UNIQ, p_recurse);
			ret_val += getCounter(METRIC_TYPE_STRING_LITERALS_UNIQ, p_recurse);
			ret_val += getCounter(METRIC_TYPE_BODY_UNRESERVED_IDENTIFIERS_UNIQ, p_recurse);
			break;
		case METRIC_TYPE_OPERATOR_HALSTEAD_UNIQUE:
			for (std::set<MetricType_e>::const_iterator it = m_halsteadOperatorMetrics.begin();
				it != m_halsteadOperatorMetrics.end();
				it++)
			{
				ret_val += (getCounter(*it, p_recurse) > 0);
			}
			break;
		case METRIC_TYPE_OPERATOR_HALSTEAD_COUNT:
			for (std::set<MetricType_e>::const_iterator it = m_halsteadOperatorMetrics.begin();
				it != m_halsteadOperatorMetrics.end();
				it++)
			{
				ret_val += getCounter(*it, p_recurse);
			}
			break;
		case METRIC_TYPE_OPERATOR_TYPES:
			for (std::set<MetricType_e>::const_iterator it = m_operatorMetrics.begin();
				it != m_operatorMetrics.end();
				it++)
			{
				ret_val += (getCounter(*it, p_recurse) > 0);
			}
			break;
		case METRIC_TYPE_HIS_COMMENT_DENSITY:
			{
				counter_t stmtCount = getCounter( METRIC_TYPE_STATEMENTS, p_recurse );
				/* Div-by-zero protection */
				if( stmtCount > 0 )
				{
					ret_val = (getCounter(METRIC_TYPE_COMMENT_COUNT, p_recurse) * m_metricScaling[METRIC_TYPE_HIS_COMMENT_DENSITY]) / stmtCount;
					/* Clip at 1.0 */
					if (ret_val > m_metricScaling[METRIC_TYPE_HIS_COMMENT_DENSITY])
					{
						ret_val = m_metricScaling[METRIC_TYPE_HIS_COMMENT_DENSITY];
					}
				}
				else
				{
					ret_val = counter_t_Max;
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
						  getCounter( METRIC_TYPE_WHILE, p_recurse ) +
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
						  getCounter( METRIC_TYPE_WHILE, p_recurse ) +
						  getCounter( METRIC_TYPE_CASE, p_recurse ) +
						  getCounter( METRIC_TYPE_OPERATOR_LOGICAL_AND, p_recurse ) +
						  getCounter( METRIC_TYPE_OPERATOR_LOGICAL_OR, p_recurse ) +
						  getCounter( METRIC_TYPE_OPERATOR_TERNARY, p_recurse ) + 1;
			}
			break;
		case METRIC_TYPE_NUMERIC_CONSTANTS_UNIQ:
			return getSupplementary(METRIC_TYPE_NUMERIC_CONSTANTS, p_recurse).size();
			break;
		case METRIC_TYPE_TOKEN_NUMERIC_CONSTANTS_UNIQ:
			return getSupplementary(METRIC_TYPE_TOKEN_NUMERIC_CONSTANTS, p_recurse).size();
			break;
		case METRIC_TYPE_STRING_LITERALS_UNIQ:
			return getSupplementary(METRIC_TYPE_STRING_LITERALS, p_recurse).size();
			break;
		case METRIC_TYPE_TOKEN_STRING_LITERALS_UNIQ:
			return getSupplementary(METRIC_TYPE_TOKEN_STRING_LITERALS, p_recurse).size();
			break;
		case METRIC_TYPE_CHAR_CONSTS_UNIQ:
			return getSupplementary(METRIC_TYPE_CHAR_CONSTS, p_recurse).size();
			break;
		case METRIC_TYPE_TOKEN_CHAR_CONSTS_UNIQ:
			return getSupplementary(METRIC_TYPE_TOKEN_CHAR_CONSTS, p_recurse).size();
			break;
		case METRIC_TYPE_UNRESERVED_IDENTIFIERS_UNIQ:
			return getSupplementary(METRIC_TYPE_UNRESERVED_IDENTIFIERS, p_recurse).size();
			break;
		case METRIC_TYPE_BODY_UNRESERVED_IDENTIFIERS_UNIQ:
			return getSupplementary(METRIC_TYPE_BODY_UNRESERVED_IDENTIFIERS, p_recurse).size();
			break;
		case METRIC_TYPE_TOKEN_UNRESERVED_IDENTIFIERS_UNIQ:
			return getSupplementary(METRIC_TYPE_TOKEN_UNRESERVED_IDENTIFIERS, p_recurse).size();
			break;
		case METRIC_TYPE_VOCF:
			ret_val = ((getCounter(METRIC_TYPE_OPERATOR_HALSTEAD_COUNT, p_recurse) + getCounter(METRIC_TYPE_OPERAND_HALSTEAD_COUNT, p_recurse)) * m_metricScaling[METRIC_TYPE_VOCF]) / 
				      (getCounter(METRIC_TYPE_OPERATOR_HALSTEAD_UNIQUE, p_recurse) + getCounter(METRIC_TYPE_OPERAND_HALSTEAD_UNIQUE, p_recurse));
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

MetricUnit* MetricUnit::getSubUnit( const std::string& p_name, const MetricUnitType_e p_type, const bool p_create )
{
	MetricUnit* ret_val = NULL;
	std::string name = p_name;

	/* For files, get the absolute path so that we can disambiguate between files which have the same filename */
	if (p_type == METRIC_UNIT_FILE)
	{
		/* Filename limit on Windows is 32767 characters - see 
		    https://msdn.microsoft.com/en-gb/library/windows/desktop/aa365247(v=vs.85).aspx */
		llvm::SmallString<INT16_MAX> NativeAbsNameBuf(p_name);
		llvm::sys::fs::make_absolute(NativeAbsNameBuf);
		llvm::sys::path::native(NativeAbsNameBuf);
		name = NativeAbsNameBuf.c_str();
	}

	SubUnitMap_t::iterator name_it = m_subUnits.find( name );
	if( name_it == m_subUnits.end() )
	{
		if( p_create )
		{
			ret_val = new MetricUnit( this, name, p_type );

			if( p_type == METRIC_UNIT_FILE )
			{
				this->increment( METRIC_TYPE_FILES );

				ret_val->m_alias = makeRelative(name);
			}
			m_subUnits[ name ] = ret_val;
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

std::string MetricUnit::getMetricShortName(const MetricType_e p_type)
{
	return m_metricShortNames[p_type];
}

MetricType_e MetricUnit::getMetricByShortName(const std::string p_shortName)
{
	MetricType_e ret_val = METRIC_TYPE_MAX;
	uint16_t loop;

	for (loop = 0;
		loop < METRIC_TYPE_MAX;
		loop++)
	{
		if (m_metricShortNames[loop] == p_shortName)
		{
			ret_val = static_cast<MetricType_e>(loop);
			break;
		}
	}

	return ret_val;
}

uint32_t MetricUnit::getMetricScaling(const MetricType_e p_type)
{
	return m_metricScaling[p_type];
}

bool MetricUnit::isMetricCumulative(const MetricType_e p_type)
{
	return m_metricIsCumulative[p_type];
}

std::string MetricUnit::getUnitName(const MetricOptions& p_options) const
{
	std::string ret_val;

	if ((this->m_type == METRIC_UNIT_FILE) &&
		(!p_options.getUseAbsoluteFileNames()))
	{
		ret_val = m_alias;
	}
	else
	{
		if ((this->m_type == METRIC_UNIT_FUNCTION) &&
			(p_options.getUsePrefix()))
		{
			ret_val = getParent()->getUnitName(p_options) + "::";
		}

		ret_val += m_name;
	}

	return ret_val;
}

bool MetricUnit::doesMetricApplyForUnit(const MetricType_e p_MetricType, const MetricUnitType_e p_unitType)
{
	return m_metricApplies[p_unitType][p_MetricType];
}

bool MetricUnit::isMetricLocalAndCumulative(const MetricType_e p_type)
{
	return m_metricReportLocal[p_type];
}

const MetricUnit::SubUnitMap_t* MetricUnit::getSubUnits(void) const
{
	return &m_subUnits;
}

const MetricUnit* MetricUnit::getParent() const
{
	return m_parent;
}

float MetricUnit::getScaledMetric(const MetricType_e p_type, MetricUnit::counter_t p_val)
{
	return (float(p_val) / float(getMetricScaling(p_type)));
}

std::string MetricUnit::getScaledMetricString(const MetricType_e p_type, MetricUnit::counter_t p_val)
{
	std::ostringstream strStream;

	const uint32_t scaling = getMetricScaling(p_type);

	if (scaling == 1)
	{
		strStream << p_val;
	}
	else
	{
		if (p_val == counter_t_Max)
		{
			strStream << "Inf";
		}
		else
		{
			strStream << std::setprecision(6) << MetricUnit::getScaledMetric(p_type, p_val);
		}
	}

	return strStream.str();
}

const std::set<std::string> MetricUnit::getSupplementary(MetricType_e p_metric, const bool p_recurse) const
{
	std::set<std::string> ret_val;
	
	std::map<MetricType_e,std::set<std::string>>::const_iterator find = m_supplementary.find(p_metric);
	if (find != m_supplementary.end())
	{
		ret_val.insert(find->second.begin(), find->second.end());
	}
	if (p_recurse)
	{
		for (SubUnitMap_t::const_iterator unitIt = m_subUnits.begin();
			unitIt != m_subUnits.end();
			++unitIt)
		{
			std::set<std::string> sub = (*unitIt).second->getSupplementary(p_metric, p_recurse);

			ret_val.insert(sub.begin(), sub.end());
		}
	}
	return ret_val;
}

void MetricUnit::setSupplementary(MetricType_e p_metric, const std::set<std::string> p_val)
{
	m_supplementary[p_metric] = p_val;
}
