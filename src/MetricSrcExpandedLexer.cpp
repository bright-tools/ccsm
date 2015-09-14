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

#include "MetricSrcExpandedLexer.hpp"
#include "MetricUtils.hpp"
#include "MetricPPIncludeHandler.hpp"
#include "clang/Lex/Preprocessor.h"
#include "clang/AST/ASTContext.h"

#include <iostream>

const static std::pair<clang::tok::TokenKind,MetricType_e> tokenKindToTypeMapData[] = {
//	std::make_pair(clang::tok::exclaim,      METRIC_TYPE_TOKEN_NOT),
//	std::make_pair(clang::tok::exclaimequal, METRIC_TYPE_TOKEN_NOT),
//	std::make_pair(clang::tok::percent,      METRIC_TYPE_TOKEN_MODULO),
//	std::make_pair(clang::tok::percentequal, METRIC_TYPE_TOKEN_MODULO_ASSIGN),
//	std::make_pair(clang::tok::amp,          METRIC_TYPE_TOKEN_AMP),
//	std::make_pair(clang::tok::ampamp,       METRIC_TYPE_TOKEN_AMPAMP),
//	std::make_pair(clang::tok::pipepipe,     METRIC_TYPE_TOKEN_PIPEPIPE),
//	std::make_pair(clang::tok::ampequal,     METRIC_TYPE_TOKEN_AND_ASSIGN),
//	std::make_pair(clang::tok::l_paren,      METRIC_TYPE_TOKEN_LPAREN),
//	std::make_pair(clang::tok::r_paren,      METRIC_TYPE_TOKEN_RPAREN),
//	std::make_pair(clang::tok::star,         METRIC_TYPE_TOKEN_ASTERISK),
//	std::make_pair(clang::tok::starequal,    METRIC_TYPE_TOKEN_ASTERISK_ASSIGN),
//	std::make_pair(clang::tok::plus,         METRIC_TYPE_TOKEN_PLUS),
//	std::make_pair(clang::tok::plusplus,     METRIC_TYPE_TOKEN_PLUSPLUS),
//	std::make_pair(clang::tok::plusequal,    METRIC_TYPE_TOKEN_PLUS_ASSIGN),
//	std::make_pair(clang::tok::comma,        METRIC_TYPE_TOKEN_COMMA),
//	std::make_pair(clang::tok::minus,        METRIC_TYPE_TOKEN_MINUS),
//	std::make_pair(clang::tok::minusminus,   METRIC_TYPE_TOKEN_MINUSMINUS),
//	std::make_pair(clang::tok::minusequal,   METRIC_TYPE_TOKEN_MINUS_ASSIGN),
//	std::make_pair(clang::tok::arrow,        METRIC_TYPE_TOKEN_MEMBER_POINTER),
//	std::make_pair(clang::tok::period,       METRIC_TYPE_TOKEN_MEMBER_REF),
//	std::make_pair(clang::tok::ellipsis,     METRIC_TYPE_TOKEN_ELLIPSIS),
//	std::make_pair(clang::tok::slash,        METRIC_TYPE_TOKEN_SLASH),
//	std::make_pair(clang::tok::slashequal,   METRIC_TYPE_TOKEN_SLASH_ASSIGN),
//	std::make_pair(clang::tok::colon,        METRIC_TYPE_TOKEN_COLON),
//	std::make_pair(clang::tok::kw_inline,    METRIC_TYPE_TOKEN_INLINE),
//	std::make_pair(clang::tok::kw_typedef,   METRIC_TYPE_TOKEN_TYPEDEF),
	std::make_pair(clang::tok::kw_auto,      METRIC_TYPE_AUTO),
	std::make_pair(clang::tok::kw_for,       METRIC_TYPE_FORLOOP),
	std::make_pair(clang::tok::kw_extern,    METRIC_TYPE_EXTERN),
	std::make_pair(clang::tok::kw_register,  METRIC_TYPE_REGISTER),
	std::make_pair(clang::tok::kw_if,        METRIC_TYPE_IF),
	std::make_pair(clang::tok::kw_else,      METRIC_TYPE_ELSE),
//	std::make_pair(clang::tok::kw_bool,      METRIC_TYPE_TOKEN_BOOL),
	std::make_pair(clang::tok::kw_char,      METRIC_TYPE_CHAR),
	std::make_pair(clang::tok::kw_double,    METRIC_TYPE_DOUBLE),
	std::make_pair(clang::tok::kw_float,     METRIC_TYPE_FLOAT),
	std::make_pair(clang::tok::kw_int,       METRIC_TYPE_INT),
	std::make_pair(clang::tok::kw_long,      METRIC_TYPE_LONG),
	std::make_pair(clang::tok::kw_short,     METRIC_TYPE_SHORT),
	std::make_pair(clang::tok::kw_signed,    METRIC_TYPE_SIGNED),
	std::make_pair(clang::tok::kw_unsigned,  METRIC_TYPE_UNSIGNED),
//	std::make_pair(clang::tok::kw_void,      METRIC_TYPE_TOKEN_VOID),
	std::make_pair(clang::tok::kw_const,     METRIC_TYPE_CONST),
//	std::make_pair(clang::tok::kw_friend,    METRIC_TYPE_TOKEN_FRIEND),
	std::make_pair(clang::tok::kw_volatile,  METRIC_TYPE_VOLATILE),
	std::make_pair(clang::tok::kw_static,    METRIC_TYPE_STATIC),
	std::make_pair(clang::tok::kw_typedef,   METRIC_TYPE_TYPEDEF),
//	std::make_pair(clang::tok::kw_virtual,   METRIC_TYPE_TOKEN_VIRTUAL),
//	std::make_pair(clang::tok::kw_mutable,   METRIC_TYPE_TOKEN_MUTABLE),
//	std::make_pair(clang::tok::kw_auto,      METRIC_TYPE_TOKEN_AUTO),
//	std::make_pair(clang::tok::kw_asm,       METRIC_TYPE_TOKEN_ASM),
	std::make_pair(clang::tok::kw_break,     METRIC_TYPE_BREAK),
	std::make_pair(clang::tok::kw_case,      METRIC_TYPE_CASE),
//	std::make_pair(clang::tok::kw_class,     METRIC_TYPE_TOKEN_CLASS),
	std::make_pair(clang::tok::kw_continue,  METRIC_TYPE_CONTINUE),
	std::make_pair(clang::tok::kw_default,   METRIC_TYPE_DEFAULT),
//	std::make_pair(clang::tok::kw_delete,    METRIC_TYPE_TOKEN_DELETE),
	std::make_pair(clang::tok::kw_do,        METRIC_TYPE_DO),
	std::make_pair(clang::tok::kw_enum,      METRIC_TYPE_ENUM),
	std::make_pair(clang::tok::kw_goto,      METRIC_TYPE_GOTO),
//	std::make_pair(clang::tok::kw_new,       METRIC_TYPE_TOKEN_NEW),
//	std::make_pair(clang::tok::kw_operator,  METRIC_TYPE_TOKEN_OPERATOR),
//	std::make_pair(clang::tok::kw_private,   METRIC_TYPE_TOKEN_PRIVATE),
//	std::make_pair(clang::tok::kw_protected, METRIC_TYPE_TOKEN_PROTECTED),
//	std::make_pair(clang::tok::kw_public,    METRIC_TYPE_TOKEN_PUBLIC),
	std::make_pair(clang::tok::kw_return,    METRIC_TYPE_RETURN),
//	std::make_pair(clang::tok::kw_sizeof,    METRIC_TYPE_TOKEN_SIZEOF),
	std::make_pair(clang::tok::kw_struct,    METRIC_TYPE_STRUCT),
	std::make_pair(clang::tok::kw_switch,    METRIC_TYPE_SWITCH),
//	std::make_pair(clang::tok::kw_this,      METRIC_TYPE_TOKEN_THIS),
	std::make_pair(clang::tok::kw_union,     METRIC_TYPE_UNION),
	std::make_pair(clang::tok::kw_while,            METRIC_TYPE_WHILE),
//	std::make_pair(clang::tok::kw_namespace,        METRIC_TYPE_TOKEN_NAMESPACE),
//	std::make_pair(clang::tok::kw_using,            METRIC_TYPE_TOKEN_USING),
//	std::make_pair(clang::tok::kw_try,              METRIC_TYPE_TOKEN_TRY),
//	std::make_pair(clang::tok::kw_catch,            METRIC_TYPE_TOKEN_CATCH),
//	std::make_pair(clang::tok::kw_throw,            METRIC_TYPE_TOKEN_THROW),
//	std::make_pair(clang::tok::kw_typeid,           METRIC_TYPE_TOKEN_TYPEID),
//	std::make_pair(clang::tok::kw_template,         METRIC_TYPE_TOKEN_TEMPLATE),
//	std::make_pair(clang::tok::kw_explicit,         METRIC_TYPE_TOKEN_EXPLICIT),
//	std::make_pair(clang::tok::kw_true,             METRIC_TYPE_TOKEN_TRUE),
//	std::make_pair(clang::tok::kw_false,            METRIC_TYPE_TOKEN_FALSE),
//	std::make_pair(clang::tok::kw_typename,         METRIC_TYPE_TOKEN_TYPENAME),
//	std::make_pair(clang::tok::coloncolon,          METRIC_TYPE_TOKEN_COLONCOLON),
//	std::make_pair(clang::tok::less,                METRIC_TYPE_TOKEN_LESS),
//	std::make_pair(clang::tok::lessless,            METRIC_TYPE_TOKEN_LESSLESS),
//	std::make_pair(clang::tok::lesslessequal,       METRIC_TYPE_TOKEN_LESSLESS_ASSIGN),
//	std::make_pair(clang::tok::lessequal,           METRIC_TYPE_TOKEN_LESS_EQUAL),
//	std::make_pair(clang::tok::equal,               METRIC_TYPE_TOKEN_ASSIGN),
//	std::make_pair(clang::tok::equalequal,          METRIC_TYPE_TOKEN_COMPARISON),
//	std::make_pair(clang::tok::greater,             METRIC_TYPE_TOKEN_MORE),
//	std::make_pair(clang::tok::greaterequal,        METRIC_TYPE_TOKEN_MORE_EQUAL),
//	std::make_pair(clang::tok::greatergreater,      METRIC_TYPE_TOKEN_MOREMORE),
//	std::make_pair(clang::tok::greatergreaterequal, METRIC_TYPE_TOKEN_MOREMORE_ASSIGN),
//	std::make_pair(clang::tok::question,            METRIC_TYPE_TOKEN_QUESTION),
//	std::make_pair(clang::tok::l_square,            METRIC_TYPE_TOKEN_LSQUARE),
//	std::make_pair(clang::tok::r_square,            METRIC_TYPE_TOKEN_RSQUARE),
//	std::make_pair(clang::tok::caret,               METRIC_TYPE_TOKEN_CARET),
//	std::make_pair(clang::tok::caretequal,          METRIC_TYPE_TOKEN_CARET_ASSIGN),
//	std::make_pair(clang::tok::l_brace,             METRIC_TYPE_TOKEN_LBRACE),
//	std::make_pair(clang::tok::r_brace,             METRIC_TYPE_TOKEN_RBRACE),
//	std::make_pair(clang::tok::pipe,                METRIC_TYPE_TOKEN_PIPE),
//	std::make_pair(clang::tok::pipeequal,           METRIC_TYPE_TOKEN_PIPE_ASSIGN),
//	std::make_pair(clang::tok::tilde,               METRIC_TYPE_TOKEN_TILDE)
};

const std::map<clang::tok::TokenKind, MetricType_e> MetricSrcExpandedLexer::m_tokenKindToTypeMap(tokenKindToTypeMapData,
    tokenKindToTypeMapData + sizeof tokenKindToTypeMapData / sizeof tokenKindToTypeMapData[0]);


MetricSrcExpandedLexer::MetricSrcExpandedLexer(clang::CompilerInstance &p_CI, MetricUnit* p_topUnit, MetricOptions* p_options) : MetricSrcLexer( p_CI, p_topUnit, p_options ),
																												 m_dumpNewline( false )
{
}

MetricSrcExpandedLexer::~MetricSrcExpandedLexer(void)
{
}

MetricUnitProcessingType_e MetricSrcExpandedLexer::getLexType(void) const
{
	return METRIC_UNIT_PROCESS_LEX_EXPANDED;
}

void MetricSrcExpandedLexer::ProcessToken(clang::Token& p_token)
{		
	std::string tok_data;
	unsigned int tok_len = p_token.getLength();

	if( m_options->getDumpTokens() )
	{
		std::cout << "(" << p_token.getName();
	}

	switch( p_token.getKind() )
	{
		case clang::tok::numeric_constant:
			tok_data = clang::StringRef(p_token.getLiteralData(), tok_len).str();
			if( m_currentUnit->isFnOrMethod() )
			{
				m_currentFnNumerics.insert( tok_data );
			}
			else
			{
				m_currentFileNumerics.insert( tok_data );
			}
			m_currentUnit->increment( METRIC_TYPE_TOKEN_NUMERIC_CONSTANTS );
			break;
		case clang::tok::char_constant:
			tok_data = clang::StringRef(p_token.getLiteralData(), tok_len).str();
			if( m_currentUnit->isFnOrMethod() )
			{
				m_currentFnCharConsts.insert( tok_data );
			}
			else
			{
				m_currentFileCharConsts.insert( tok_data );
			}
			m_currentUnit->increment( METRIC_TYPE_TOKEN_CHAR_CONSTS );
			break;
		case clang::tok::string_literal:
			tok_data = clang::StringRef(p_token.getLiteralData(), tok_len).str();
			if( m_currentUnit->isFnOrMethod() )
			{
				m_currentFnStrings.insert( tok_data );
			}
			else
			{
				m_currentFileStrings.insert( tok_data );
			}
			m_currentUnit->increment( METRIC_TYPE_TOKEN_STRING_LITERALS );
			break;
		case clang::tok::comment:
			/* TODO */
			break;
		case clang::tok::eof:
			/* Not interested in registering end-of-file */
			break;
		default:
			std::map<clang::tok::TokenKind, MetricType_e>::const_iterator typeLookup = m_tokenKindToTypeMap.find(p_token.getKind());

			if (typeLookup != m_tokenKindToTypeMap.end())
			{
				m_currentUnit->increment((*typeLookup).second);
			}
			break;
	}

	if( m_options->getDumpTokens() )
	{
		std::cout << "," << p_token.getLocation().getRawEncoding();
		std::cout << "," << m_compilerInstance.getSourceManager().getFileLoc(p_token.getLocation()).getRawEncoding();
		if (tok_data.length())
		{
			std::cout << "," << tok_data;
		}
			std::cout << "," << (long)(p_token.getKind());
			std::cout << "," << (long)(p_token.getFlags());
		std::cout << ")";
		if (m_dumpNewline)
		{
			std::cout << std::endl << "  ";
			m_dumpNewline = false;
		}
	}

}

void MetricSrcExpandedLexer::CloseOutFnOrMtd(void)
{
	/* Have a current unit? */
	if( m_currentUnit != NULL )
	{
		/* Is it of the appropriate type? */
		if( m_currentUnit->isFnOrMethod() )
		{
			/* Close off accumulated metrics */

			m_currentUnit->set( METRIC_TYPE_TOKEN_NUMERIC_CONSTANTS_UNIQ, m_currentFnNumerics.size() );
			m_currentUnit->set( METRIC_TYPE_TOKEN_STRING_LITERALS_UNIQ, m_currentFnStrings.size() );
			m_currentUnit->set( METRIC_TYPE_TOKEN_UNRESERVED_IDENTIFIERS_UNIQ, m_currentFnIdentifiers.size() );
			m_currentUnit->set( METRIC_TYPE_TOKEN_CHAR_CONSTS_UNIQ, m_currentFnCharConsts.size() );
		}
	}
	m_currentFnNumerics.clear();
	m_currentFnStrings.clear();
	m_currentFnIdentifiers.clear();
	m_currentFnCharConsts.clear();
}