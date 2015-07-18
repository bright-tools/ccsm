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

#include "MetricSrcUnexpandedLexer.hpp"
#include "MetricUtils.hpp"
#include "MetricPPIncludeHandler.hpp"
#include "clang/Lex/Preprocessor.h"
#include "clang/AST/ASTContext.h"

#include <iostream>

const MetricSrcUnexpandedLexer::SemiConlonContainers_e MetricSrcUnexpandedLexer::m_sccStartLineEnding = SCC_Define;
const MetricSrcUnexpandedLexer::SemiConlonContainers_e MetricSrcUnexpandedLexer::m_sccEndLineEnding = SCC_Define;

const MetricSrcUnexpandedLexer::SemiConlonContainers_e MetricSrcUnexpandedLexer::m_sccStartParen = SCC_For;
const MetricSrcUnexpandedLexer::SemiConlonContainers_e MetricSrcUnexpandedLexer::m_sccEndParen = SCC_For;

const MetricSrcUnexpandedLexer::SemiConlonContainers_e MetricSrcUnexpandedLexer::m_sccStartBrace = SCC_Struct;
const MetricSrcUnexpandedLexer::SemiConlonContainers_e MetricSrcUnexpandedLexer::m_sccEndBrace = SCC_Union;

const static std::set<clang::tok::TokenKind> statementImplyingTokens[2] = {
	{
		clang::tok::kw_for
	},
	{
		//clang::tok::identifier,
		clang::tok::exclaim,
		clang::tok::exclaimequal,
		clang::tok::percent,
		clang::tok::percentequal,
		clang::tok::amp,
		clang::tok::ampamp,
		clang::tok::pipepipe,
		clang::tok::ampequal,
		//clang::tok::l_paren, 
		//clang::tok::r_paren, 
		clang::tok::star,
		clang::tok::starequal,
		clang::tok::plus,
		clang::tok::plusplus,
		clang::tok::plusequal,
		//clang::tok::comma, 
		clang::tok::minus,
		clang::tok::minusminus,
		clang::tok::minusequal,
		clang::tok::arrow,
		clang::tok::period,
		clang::tok::ellipsis,
		clang::tok::slash,
		clang::tok::slashequal,
		//clang::tok::colon, 
		//clang::tok::kw_inline, 
		//clang::tok::kw_typedef, 
		//clang::tok::kw_auto, 
		//clang::tok::kw_extern, 
		//clang::tok::kw_register, 
		clang::tok::kw_if,
		clang::tok::kw_else,
		//clang::tok::kw_bool, 
		//clang::tok::kw_char, 
		//clang::tok::kw_double, 
		//clang::tok::kw_float, 
		//clang::tok::kw_int, 
		//clang::tok::kw_long, 
		//clang::tok::kw_short, 
		//clang::tok::kw_signed, 
		//clang::tok::kw_unsigned, 
		//clang::tok::kw_void, 
		//clang::tok::kw_const, 
		//clang::tok::kw_friend, 
		//clang::tok::kw_volatile, 
		//clang::tok::kw_static, 
		//clang::tok::kw_typedef, 
		//clang::tok::kw_virtual, 
		//clang::tok::kw_mutable, 
		//clang::tok::kw_auto, 
		//clang::tok::kw_asm, 
		clang::tok::kw_break,
		//clang::tok::kw_case, 
		//clang::tok::kw_class, 
		clang::tok::kw_continue,
		clang::tok::kw_default,
		//clang::tok::kw_delete, 
		clang::tok::kw_do,
		//clang::tok::kw_enum, 
		clang::tok::kw_goto,
		//clang::tok::kw_new, 
		//clang::tok::kw_operator, 
		//clang::tok::kw_private, 
		//clang::tok::kw_protected, 
		//clang::tok::kw_public, 
		clang::tok::kw_return,
		//clang::tok::kw_sizeof, 
		//clang::tok::kw_struct, 
		clang::tok::kw_switch,
		//clang::tok::kw_this, 
		//clang::tok::kw_union, 
		clang::tok::kw_while,
		//clang::tok::kw_namespace, 
		//clang::tok::kw_using, 
		//clang::tok::kw_try, 
		//clang::tok::kw_catch, 
		//clang::tok::kw_throw, 
		//clang::tok::kw_typeid, 
		//clang::tok::kw_template, 
		//clang::tok::kw_explicit, 
		//clang::tok::kw_true, 
		//clang::tok::kw_false, 
		//clang::tok::kw_typename, 
		//clang::tok::coloncolon, 
		clang::tok::less,
		clang::tok::lessless,
		clang::tok::lesslessequal,
		clang::tok::lessequal,
		clang::tok::equal,
		clang::tok::equalequal,
		clang::tok::greater,
		clang::tok::greaterequal,
		clang::tok::greatergreater,
		clang::tok::greatergreaterequal,
		clang::tok::question,
		clang::tok::l_square,
		clang::tok::r_square,
		clang::tok::caret,
		clang::tok::caretequal,
		//clang::tok::l_brace, 
		//clang::tok::r_brace, 
		clang::tok::pipe,
		clang::tok::pipeequal,
		clang::tok::tilde
	}
};

const static std::pair<clang::tok::TokenKind,MetricType_e> tokenKindToTypeMapData[] = {
	std::make_pair(clang::tok::exclaim,      METRIC_TYPE_TOKEN_NOT),
	std::make_pair(clang::tok::exclaimequal, METRIC_TYPE_TOKEN_NOT),
	std::make_pair(clang::tok::percent,      METRIC_TYPE_TOKEN_MODULO),
	std::make_pair(clang::tok::percentequal, METRIC_TYPE_TOKEN_MODULO_ASSIGN),
	std::make_pair(clang::tok::amp,          METRIC_TYPE_TOKEN_AMP),
	std::make_pair(clang::tok::ampamp,       METRIC_TYPE_TOKEN_AMPAMP),
	std::make_pair(clang::tok::pipepipe,     METRIC_TYPE_TOKEN_PIPEPIPE),
	std::make_pair(clang::tok::ampequal,     METRIC_TYPE_TOKEN_AND_ASSIGN),
	std::make_pair(clang::tok::l_paren,      METRIC_TYPE_TOKEN_LPAREN),
	std::make_pair(clang::tok::r_paren,      METRIC_TYPE_TOKEN_RPAREN),
	std::make_pair(clang::tok::star,         METRIC_TYPE_TOKEN_ASTERISK),
	std::make_pair(clang::tok::starequal,    METRIC_TYPE_TOKEN_ASTERISK_ASSIGN),
	std::make_pair(clang::tok::plus,         METRIC_TYPE_TOKEN_PLUS),
	std::make_pair(clang::tok::plusplus,     METRIC_TYPE_TOKEN_PLUSPLUS),
	std::make_pair(clang::tok::plusequal,    METRIC_TYPE_TOKEN_PLUS_ASSIGN),
	std::make_pair(clang::tok::comma,        METRIC_TYPE_TOKEN_COMMA),
	std::make_pair(clang::tok::minus,        METRIC_TYPE_TOKEN_MINUS),
	std::make_pair(clang::tok::minusminus,   METRIC_TYPE_TOKEN_MINUSMINUS),
	std::make_pair(clang::tok::minusequal,   METRIC_TYPE_TOKEN_MINUS_ASSIGN),
	std::make_pair(clang::tok::arrow,        METRIC_TYPE_TOKEN_MEMBER_POINTER),
	std::make_pair(clang::tok::period,       METRIC_TYPE_TOKEN_MEMBER_REF),
	std::make_pair(clang::tok::ellipsis,     METRIC_TYPE_TOKEN_ELLIPSIS),
	std::make_pair(clang::tok::slash,        METRIC_TYPE_TOKEN_SLASH),
	std::make_pair(clang::tok::slashequal,   METRIC_TYPE_TOKEN_SLASH_ASSIGN),
	std::make_pair(clang::tok::colon,        METRIC_TYPE_TOKEN_COLON),
	std::make_pair(clang::tok::kw_inline,    METRIC_TYPE_TOKEN_INLINE),
	std::make_pair(clang::tok::kw_typedef,   METRIC_TYPE_TOKEN_TYPEDEF),
	std::make_pair(clang::tok::kw_auto,      METRIC_TYPE_TOKEN_AUTO),
	std::make_pair(clang::tok::kw_for,       METRIC_TYPE_TOKEN_FOR),
	std::make_pair(clang::tok::kw_extern,    METRIC_TYPE_TOKEN_EXTERN),
	std::make_pair(clang::tok::kw_register,  METRIC_TYPE_TOKEN_REGISTER),
	std::make_pair(clang::tok::kw_if,        METRIC_TYPE_TOKEN_IF),
	std::make_pair(clang::tok::kw_else,      METRIC_TYPE_TOKEN_ELSE),
	std::make_pair(clang::tok::kw_bool,      METRIC_TYPE_TOKEN_BOOL),
	std::make_pair(clang::tok::kw_char,      METRIC_TYPE_TOKEN_CHAR),
	std::make_pair(clang::tok::kw_double,    METRIC_TYPE_TOKEN_DOUBLE),
	std::make_pair(clang::tok::kw_float,     METRIC_TYPE_TOKEN_FLOAT),
	std::make_pair(clang::tok::kw_int,       METRIC_TYPE_TOKEN_INT),
	std::make_pair(clang::tok::kw_long,      METRIC_TYPE_TOKEN_LONG),
	std::make_pair(clang::tok::kw_short,     METRIC_TYPE_TOKEN_SHORT),
	std::make_pair(clang::tok::kw_signed,    METRIC_TYPE_TOKEN_SIGNED),
	std::make_pair(clang::tok::kw_unsigned,  METRIC_TYPE_TOKEN_UNSIGNED),
	std::make_pair(clang::tok::kw_void,      METRIC_TYPE_TOKEN_VOID),
	std::make_pair(clang::tok::kw_const,     METRIC_TYPE_TOKEN_CONST),
	std::make_pair(clang::tok::kw_friend,    METRIC_TYPE_TOKEN_FRIEND),
	std::make_pair(clang::tok::kw_volatile,  METRIC_TYPE_TOKEN_VOLATILE),
	std::make_pair(clang::tok::kw_static,    METRIC_TYPE_TOKEN_STATIC),
	std::make_pair(clang::tok::kw_typedef,   METRIC_TYPE_TOKEN_TYPEDEF),
	std::make_pair(clang::tok::kw_virtual,   METRIC_TYPE_TOKEN_VIRTUAL),
	std::make_pair(clang::tok::kw_mutable,   METRIC_TYPE_TOKEN_MUTABLE),
	std::make_pair(clang::tok::kw_auto,      METRIC_TYPE_TOKEN_AUTO),
	std::make_pair(clang::tok::kw_asm,       METRIC_TYPE_TOKEN_ASM),
	std::make_pair(clang::tok::kw_break,     METRIC_TYPE_TOKEN_BREAK),
	std::make_pair(clang::tok::kw_case,      METRIC_TYPE_TOKEN_CASE),
	std::make_pair(clang::tok::kw_class,     METRIC_TYPE_TOKEN_CLASS),
	std::make_pair(clang::tok::kw_continue,  METRIC_TYPE_TOKEN_CONTINUE),
	std::make_pair(clang::tok::kw_default,   METRIC_TYPE_TOKEN_DEFAULT),
	std::make_pair(clang::tok::kw_delete,    METRIC_TYPE_TOKEN_DELETE),
	std::make_pair(clang::tok::kw_do,        METRIC_TYPE_TOKEN_DO),
	std::make_pair(clang::tok::kw_enum,      METRIC_TYPE_TOKEN_ENUM),
	std::make_pair(clang::tok::kw_goto,      METRIC_TYPE_TOKEN_GOTO),
	std::make_pair(clang::tok::kw_new,       METRIC_TYPE_TOKEN_NEW),
	std::make_pair(clang::tok::kw_operator,  METRIC_TYPE_TOKEN_OPERATOR),
	std::make_pair(clang::tok::kw_private,   METRIC_TYPE_TOKEN_PRIVATE),
	std::make_pair(clang::tok::kw_protected, METRIC_TYPE_TOKEN_PROTECTED),
	std::make_pair(clang::tok::kw_public,    METRIC_TYPE_TOKEN_PUBLIC),
	std::make_pair(clang::tok::kw_return,    METRIC_TYPE_TOKEN_RETURN),
	std::make_pair(clang::tok::kw_sizeof,    METRIC_TYPE_TOKEN_SIZEOF),
	std::make_pair(clang::tok::kw_struct,    METRIC_TYPE_TOKEN_STRUCT),
	std::make_pair(clang::tok::kw_switch,    METRIC_TYPE_TOKEN_SWITCH),
	std::make_pair(clang::tok::kw_this,      METRIC_TYPE_TOKEN_THIS),
	std::make_pair(clang::tok::kw_union,     METRIC_TYPE_TOKEN_UNION),
	std::make_pair(clang::tok::kw_while,            METRIC_TYPE_TOKEN_WHILE),
	std::make_pair(clang::tok::kw_namespace,        METRIC_TYPE_TOKEN_NAMESPACE),
	std::make_pair(clang::tok::kw_using,            METRIC_TYPE_TOKEN_USING),
	std::make_pair(clang::tok::kw_try,              METRIC_TYPE_TOKEN_TRY),
	std::make_pair(clang::tok::kw_catch,            METRIC_TYPE_TOKEN_CATCH),
	std::make_pair(clang::tok::kw_throw,            METRIC_TYPE_TOKEN_THROW),
	std::make_pair(clang::tok::kw_typeid,           METRIC_TYPE_TOKEN_TYPEID),
	std::make_pair(clang::tok::kw_template,         METRIC_TYPE_TOKEN_TEMPLATE),
	std::make_pair(clang::tok::kw_explicit,         METRIC_TYPE_TOKEN_EXPLICIT),
	std::make_pair(clang::tok::kw_true,             METRIC_TYPE_TOKEN_TRUE),
	std::make_pair(clang::tok::kw_false,            METRIC_TYPE_TOKEN_FALSE),
	std::make_pair(clang::tok::kw_typename,         METRIC_TYPE_TOKEN_TYPENAME),
	std::make_pair(clang::tok::coloncolon,          METRIC_TYPE_TOKEN_COLONCOLON),
	std::make_pair(clang::tok::less,                METRIC_TYPE_TOKEN_LESS),
	std::make_pair(clang::tok::lessless,            METRIC_TYPE_TOKEN_LESSLESS),
	std::make_pair(clang::tok::lesslessequal,       METRIC_TYPE_TOKEN_LESSLESS_ASSIGN),
	std::make_pair(clang::tok::lessequal,           METRIC_TYPE_TOKEN_LESS_EQUAL),
	std::make_pair(clang::tok::equal,               METRIC_TYPE_TOKEN_ASSIGN),
	std::make_pair(clang::tok::equalequal,          METRIC_TYPE_TOKEN_COMPARISON),
	std::make_pair(clang::tok::greater,             METRIC_TYPE_TOKEN_MORE),
	std::make_pair(clang::tok::greaterequal,        METRIC_TYPE_TOKEN_MORE_EQUAL),
	std::make_pair(clang::tok::greatergreater,      METRIC_TYPE_TOKEN_MOREMORE),
	std::make_pair(clang::tok::greatergreaterequal, METRIC_TYPE_TOKEN_MOREMORE_ASSIGN),
	std::make_pair(clang::tok::question,            METRIC_TYPE_TOKEN_QUESTION),
	std::make_pair(clang::tok::l_square,            METRIC_TYPE_TOKEN_LSQUARE),
	std::make_pair(clang::tok::r_square,            METRIC_TYPE_TOKEN_RSQUARE),
	std::make_pair(clang::tok::caret,               METRIC_TYPE_TOKEN_CARET),
	std::make_pair(clang::tok::caretequal,          METRIC_TYPE_TOKEN_CARET_ASSIGN),
	std::make_pair(clang::tok::l_brace,             METRIC_TYPE_TOKEN_LBRACE),
	std::make_pair(clang::tok::r_brace,             METRIC_TYPE_TOKEN_RBRACE),
	std::make_pair(clang::tok::pipe,                METRIC_TYPE_TOKEN_PIPE),
	std::make_pair(clang::tok::pipeequal,           METRIC_TYPE_TOKEN_PIPE_ASSIGN),
	std::make_pair(clang::tok::tilde,               METRIC_TYPE_TOKEN_TILDE)
};

const std::map<clang::tok::TokenKind, MetricType_e> MetricSrcUnexpandedLexer::m_tokenKindToTypeMap(tokenKindToTypeMapData,
    tokenKindToTypeMapData + sizeof tokenKindToTypeMapData / sizeof tokenKindToTypeMapData[0]);


MetricSrcUnexpandedLexer::MetricSrcUnexpandedLexer(clang::CompilerInstance &p_CI, MetricUnit* p_topUnit, MetricOptions* p_options) : MetricSrcLexer( p_CI, p_topUnit, p_options ),
																												 m_dumpNewline( false )
{
}

MetricSrcUnexpandedLexer::~MetricSrcUnexpandedLexer(void)
{
}

void MetricSrcUnexpandedLexer::HandleSemiToken(clang::Token& p_token)
{
	bool inScc = false;
	for( unsigned checkLoop = 0 ; checkLoop < SCC_MAX ; checkLoop++ )
	{
		if( m_semiContainerDepth[ checkLoop ] )
		{
			inScc = true;
			break;
		}
	}
	if( !inScc )
	{
		if (m_currentFunctionName.length() && m_semiHasContent)
		{
// TODO			m_currentUnit->increment(METRIC_TYPE_HIS_STATEMENT);
			m_semiHasContent = false;
			m_dumpNewline = true;
		}
		if( m_options->getDumpTokens() )
		{
			std::cout << ",statement-delimiter";
		}
	}
}

MetricUnitProcessingType_e MetricSrcUnexpandedLexer::getLexType(void) const
{
	return METRIC_UNIT_PROCESS_LEX_UNEXPANDED;
}

void MetricSrcUnexpandedLexer::HandleBasicToken(clang::Token& p_token)
{
	const clang::tok::TokenKind tokenKind = p_token.getKind();
	MetricSrcUnexpandedLexer::SemiConlonContainers_e start;
	MetricSrcUnexpandedLexer::SemiConlonContainers_e end;
	bool checkSccStart = false;
	bool checkSccEnd = false;

	if (statementImplyingTokens[0].find(tokenKind) != statementImplyingTokens[0].end())
	{
// TODO		m_currentUnit->increment(METRIC_TYPE_HIS_STATEMENT);
		m_dumpNewline = true;
	}
	else if (statementImplyingTokens[1].find(tokenKind) != statementImplyingTokens[1].end())
	{
		m_semiHasContent = true;
	}

	std::map<clang::tok::TokenKind, MetricType_e>::const_iterator typeLookup = m_tokenKindToTypeMap.find(tokenKind);

	if( typeLookup != m_tokenKindToTypeMap.end() )
	{
		m_currentUnit->increment( (*typeLookup).second );
		switch( tokenKind )
		{
			case clang::tok::kw_for:
				m_semiContainerOpen[ SCC_For ] = true;
				break;
			case clang::tok::kw_struct:
				m_semiContainerOpen[ SCC_Struct ] = true;
				break;
			case clang::tok::kw_union:
				m_semiContainerOpen[ SCC_Union ] = true;
				break;
			default:
				break;
#if 0
					else if(( tok_data == "define" ) && 
						    ( m_lastToken == clang::tok::hash ))
					{
						m_semiContainerOpen[ SCC_Define ] = true;
					}
#endif
		}

	}
	else
	{
		if( p_token.isAnyIdentifier() )
		{
			std::string tok_data = p_token.getIdentifierInfo()->getName();
			if( m_options->getDumpTokens() )
			{
				std::cout << ",unreserved:"<<tok_data;
			}
			if( m_currentUnit->isFnOrMethod() )
			{
				m_currentFnIdentifiers.insert( tok_data );
			}
			else
			{
				m_currentFileIdentifiers.insert( tok_data );
			}
			m_currentUnit->increment( METRIC_TYPE_TOKEN_UNRESERVED_IDENTIFIERS );
		}
		else
		{
			/* TODO */
		}
		/* TODO */
	}

	switch( tokenKind )
	{
		case clang::tok::l_brace:
			checkSccStart = true;
			start = m_sccStartBrace;
			end = m_sccEndBrace;
			break;
		case clang::tok::l_paren:
			checkSccStart = true;
			start = m_sccStartParen;
			end = m_sccEndParen;
			break;
		case clang::tok::r_brace:
			checkSccEnd = true;
			start = m_sccStartBrace;
			end = m_sccEndBrace;
			break;
		case clang::tok::r_paren:
			checkSccEnd = true;
			start = m_sccStartParen;
			end = m_sccEndParen;
			break;
		default:
			break;
	}

	if( checkSccStart )
	{
		for( signed checkLoop = start ; checkLoop <= end; checkLoop++ )
		{
			if( m_semiContainerOpen[ checkLoop ] ||
				m_semiContainerDepth[ checkLoop ] )
			{
				m_semiContainerDepth[ checkLoop ]++;
				m_semiContainerOpen[ checkLoop ] = false;
			}
		}
	}
	else if( checkSccEnd )
	{
		for( signed checkLoop = start ; checkLoop <= end ; checkLoop++ )
		{
			if( m_semiContainerDepth[ checkLoop ] )
			{
				m_semiContainerDepth[ checkLoop ]--;
			}
		}
	}

}

void MetricSrcUnexpandedLexer::ProcessToken(clang::Token& p_token)
{		
	std::string tok_data;
	unsigned int tok_len = p_token.getLength();

	if( m_options->getDumpTokens() )
	{
		std::cout << "(" << p_token.getName();
	}

	switch( p_token.getKind() )
	{
#if 0
		case clang::tok::raw_identifier:
			{
					if( m_options->getDumpTokens() )
					{
						std::cout << "raw_t,";
					}
					tok_data = p_token.getRawIdentifier();
				std::map<std::string,MetricType_e>::const_iterator typeLookup = m_tokenToTypeMap.find( tok_data );
				if( typeLookup != m_tokenToTypeMap.end() )
				{
					if( m_options->getDumpTokens() )
					{
						std::cout << ",reserved";
					}
					m_currentUnit->increment( (*typeLookup).second );
					if( tok_data == "for" )
					{
						m_semiContainerOpen[ SCC_For ] = true;
					}
					else if( tok_data == "struct" )
					{
						m_semiContainerOpen[ SCC_Struct ] = true;
					}
					else if( tok_data == "union" )
					{
						m_semiContainerOpen[ SCC_Union ] = true;
					}
					else if(( tok_data == "define" ) && 
						    ( m_lastToken == clang::tok::hash ))
					{
						m_semiContainerOpen[ SCC_Define ] = true;
					}
				}
				else
				{
					if( m_options->getDumpTokens() )
					{
						std::cout << ",unreserved";
					}
					if( m_currentUnit->isFnOrMethod() )
					{
						m_currentFnIdentifiers.insert( tok_data );
					}
					else
					{
						m_currentFileIdentifiers.insert( tok_data );
					}
					m_currentUnit->increment( METRIC_TYPE_TOKEN_UNRESERVED_IDENTIFIERS );
				}
			}
			break;
#endif
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
		case clang::tok::semi:
			HandleSemiToken( p_token );
			break;
		case clang::tok::comment:
			/* TODO */
			break;
		case clang::tok::eof:
			/* Not interested in registering end-of-file */
			break;
		default:
			{
				HandleBasicToken( p_token );
			}
			break;
	}

	if( m_options->getDumpTokens() )
	{
		if( tok_data.length() )
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

	m_lastToken = p_token.getKind();
}

void MetricSrcUnexpandedLexer::CloseOutFnOrMtd(void)
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

void MetricSrcUnexpandedLexer::LexSources(clang::CompilerInstance& p_ci, const TranslationUnitFunctionLocator* const p_fnLocator)
{
	for( unsigned initLoop = 0 ; initLoop < SCC_MAX ; initLoop++ )
	{
		m_semiContainerDepth[ initLoop ] = 0;
		m_semiContainerOpen[ initLoop ] = false;
	}

	m_semiHasContent = false;

	clang::Preprocessor &PP = p_ci.getPreprocessor();
	PP.SetMacroExpansionOnlyInDirectives();

	MetricSrcLexer::LexSources(p_ci, p_fnLocator);
}