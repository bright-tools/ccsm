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

#include "MetricSrcLexer.hpp"
#include "MetricUtils.hpp"
#include "clang/Lex/Preprocessor.h"
#include "clang/AST/ASTContext.h"

#include <iostream>

const MetricSrcLexer::SemiConlonContainers_e MetricSrcLexer::m_sccStartLineEnding = SCC_Define;
const MetricSrcLexer::SemiConlonContainers_e MetricSrcLexer::m_sccEndLineEnding = SCC_Define;

const MetricSrcLexer::SemiConlonContainers_e MetricSrcLexer::m_sccStartParen = SCC_For;
const MetricSrcLexer::SemiConlonContainers_e MetricSrcLexer::m_sccEndParen = SCC_For;

const MetricSrcLexer::SemiConlonContainers_e MetricSrcLexer::m_sccStartBrace = SCC_Struct;
const MetricSrcLexer::SemiConlonContainers_e MetricSrcLexer::m_sccEndBrace = SCC_Union;


const static std::pair<std::string,MetricType_e> tokenToTypeMapData[] = {
	std::make_pair("bool", METRIC_TYPE_TOKEN_BOOL),
	std::make_pair("char", METRIC_TYPE_TOKEN_CHAR),
	std::make_pair("double", METRIC_TYPE_TOKEN_DOUBLE),
	std::make_pair("float", METRIC_TYPE_TOKEN_FLOAT),
	std::make_pair("int", METRIC_TYPE_TOKEN_INT),
	std::make_pair("long", METRIC_TYPE_TOKEN_LONG),
	std::make_pair("short", METRIC_TYPE_TOKEN_SHORT),
	std::make_pair("signed", METRIC_TYPE_TOKEN_SIGNED),
	std::make_pair("unsigned", METRIC_TYPE_TOKEN_UNSIGNED),
	std::make_pair("void", METRIC_TYPE_TOKEN_VOID),
	std::make_pair("const", METRIC_TYPE_TOKEN_CONST),
	std::make_pair("friend", METRIC_TYPE_TOKEN_FRIEND),
	std::make_pair("volatile", METRIC_TYPE_TOKEN_VOLATILE),
	std::make_pair("extern", METRIC_TYPE_TOKEN_EXTERN),
//	std::make_pair("inline", METRIC_TYPE_TOKEN_INLINE),
	std::make_pair("register", METRIC_TYPE_TOKEN_REGISTER),
	std::make_pair("static", METRIC_TYPE_TOKEN_STATIC),
	std::make_pair("typedef", METRIC_TYPE_TOKEN_TYPEDEF),
	std::make_pair("virtual", METRIC_TYPE_TOKEN_VIRTUAL),
	std::make_pair("mutable", METRIC_TYPE_TOKEN_MUTABLE),
	std::make_pair("auto", METRIC_TYPE_TOKEN_AUTO),
	std::make_pair("asm", METRIC_TYPE_TOKEN_ASM),
	std::make_pair("break", METRIC_TYPE_TOKEN_BREAK),
	std::make_pair("case", METRIC_TYPE_TOKEN_CASE),
	std::make_pair("class", METRIC_TYPE_TOKEN_CLASS),
	std::make_pair("continue", METRIC_TYPE_TOKEN_CONTINUE),
	std::make_pair("default", METRIC_TYPE_TOKEN_DEFAULT),
	std::make_pair("delete", METRIC_TYPE_TOKEN_DELETE),
	std::make_pair("do", METRIC_TYPE_TOKEN_DO),
	std::make_pair("else", METRIC_TYPE_TOKEN_ELSE),
	std::make_pair("enum", METRIC_TYPE_TOKEN_ENUM),
	std::make_pair("for", METRIC_TYPE_TOKEN_FOR),
	std::make_pair("goto", METRIC_TYPE_TOKEN_GOTO),
	std::make_pair("if", METRIC_TYPE_TOKEN_IF),
	std::make_pair("new", METRIC_TYPE_TOKEN_NEW),
	std::make_pair("operator", METRIC_TYPE_TOKEN_OPERATOR),
	std::make_pair("private", METRIC_TYPE_TOKEN_PRIVATE),
	std::make_pair("protected", METRIC_TYPE_TOKEN_PROTECTED),
	std::make_pair("public", METRIC_TYPE_TOKEN_PUBLIC),
	std::make_pair("return", METRIC_TYPE_TOKEN_RETURN),
	std::make_pair("sizeof", METRIC_TYPE_TOKEN_SIZEOF),
	std::make_pair("struct", METRIC_TYPE_TOKEN_STRUCT),
	std::make_pair("switch", METRIC_TYPE_TOKEN_SWITCH),
	std::make_pair("this", METRIC_TYPE_TOKEN_THIS),
	std::make_pair("union", METRIC_TYPE_TOKEN_UNION),
	std::make_pair("while", METRIC_TYPE_TOKEN_WHILE),
	std::make_pair("namespace", METRIC_TYPE_TOKEN_NAMESPACE),
	std::make_pair("using", METRIC_TYPE_TOKEN_USING),
	std::make_pair("try", METRIC_TYPE_TOKEN_TRY),
	std::make_pair("catch", METRIC_TYPE_TOKEN_CATCH),
	std::make_pair("throw", METRIC_TYPE_TOKEN_THROW),
	std::make_pair("typeid", METRIC_TYPE_TOKEN_TYPEID),
	std::make_pair("template", METRIC_TYPE_TOKEN_TEMPLATE),
	std::make_pair("explicit", METRIC_TYPE_TOKEN_EXPLICIT),
	std::make_pair("true", METRIC_TYPE_TOKEN_TRUE),
	std::make_pair("false", METRIC_TYPE_TOKEN_FALSE),
	std::make_pair("typename", METRIC_TYPE_TOKEN_TYPENAME)
};

const std::map<std::string,MetricType_e> MetricSrcLexer::m_tokenToTypeMap( tokenToTypeMapData,
    tokenToTypeMapData + sizeof tokenToTypeMapData / sizeof tokenToTypeMapData[0]);

const static std::pair<clang::tok::TokenKind,MetricType_e> tokenKindToTypeMapData[] = {
	std::make_pair(clang::tok::exclaim, METRIC_TYPE_TOKEN_NOT),
	std::make_pair(clang::tok::exclaimequal, METRIC_TYPE_TOKEN_NOT),
	std::make_pair(clang::tok::percent, METRIC_TYPE_TOKEN_MODULO),
	std::make_pair(clang::tok::percentequal, METRIC_TYPE_TOKEN_MODULO_ASSIGN),
	std::make_pair(clang::tok::amp, METRIC_TYPE_TOKEN_AMP),
	std::make_pair(clang::tok::ampamp, METRIC_TYPE_TOKEN_AMPAMP),
	std::make_pair(clang::tok::pipepipe, METRIC_TYPE_TOKEN_PIPEPIPE),
	std::make_pair(clang::tok::ampequal, METRIC_TYPE_TOKEN_AND_ASSIGN),
	std::make_pair(clang::tok::l_paren, METRIC_TYPE_TOKEN_LPAREN),
	std::make_pair(clang::tok::r_paren, METRIC_TYPE_TOKEN_RPAREN),
	std::make_pair(clang::tok::star, METRIC_TYPE_TOKEN_ASTERISK),
	std::make_pair(clang::tok::starequal, METRIC_TYPE_TOKEN_ASTERISK_ASSIGN),
	std::make_pair(clang::tok::plus, METRIC_TYPE_TOKEN_PLUS),
	std::make_pair(clang::tok::plusplus, METRIC_TYPE_TOKEN_PLUSPLUS),
	std::make_pair(clang::tok::plusequal, METRIC_TYPE_TOKEN_PLUS_ASSIGN),
	std::make_pair(clang::tok::comma, METRIC_TYPE_TOKEN_COMMA),
	std::make_pair(clang::tok::minus, METRIC_TYPE_TOKEN_MINUS),
	std::make_pair(clang::tok::minusminus, METRIC_TYPE_TOKEN_MINUSMINUS),
	std::make_pair(clang::tok::minusequal, METRIC_TYPE_TOKEN_MINUS_ASSIGN),
	std::make_pair(clang::tok::arrow, METRIC_TYPE_TOKEN_MEMBER_POINTER),
	std::make_pair(clang::tok::period, METRIC_TYPE_TOKEN_MEMBER_REF),
	std::make_pair(clang::tok::ellipsis, METRIC_TYPE_TOKEN_ELLIPSIS),
	std::make_pair(clang::tok::slash, METRIC_TYPE_TOKEN_SLASH),
	std::make_pair(clang::tok::slashequal, METRIC_TYPE_TOKEN_SLASH_ASSIGN),
	std::make_pair(clang::tok::colon, METRIC_TYPE_TOKEN_COLON),
	std::make_pair(clang::tok::kw_inline, METRIC_TYPE_TOKEN_INLINE),
	std::make_pair(clang::tok::kw_typedef, METRIC_TYPE_TOKEN_TYPEDEF),
	std::make_pair(clang::tok::kw_auto, METRIC_TYPE_TOKEN_AUTO),
	std::make_pair(clang::tok::kw_extern, METRIC_TYPE_TOKEN_EXTERN),
	std::make_pair(clang::tok::kw_register, METRIC_TYPE_TOKEN_REGISTER),
	std::make_pair(clang::tok::coloncolon, METRIC_TYPE_TOKEN_COLONCOLON),
	std::make_pair(clang::tok::less, METRIC_TYPE_TOKEN_LESS),
	std::make_pair(clang::tok::lessless, METRIC_TYPE_TOKEN_LESSLESS),
	std::make_pair(clang::tok::lesslessequal, METRIC_TYPE_TOKEN_LESSLESS_ASSIGN),
	std::make_pair(clang::tok::lessequal, METRIC_TYPE_TOKEN_LESS_EQUAL),
	std::make_pair(clang::tok::equal, METRIC_TYPE_TOKEN_ASSIGN),
	std::make_pair(clang::tok::equalequal, METRIC_TYPE_TOKEN_COMPARISON),
	std::make_pair(clang::tok::greater, METRIC_TYPE_TOKEN_MORE),
	std::make_pair(clang::tok::greaterequal, METRIC_TYPE_TOKEN_MORE_EQUAL),
	std::make_pair(clang::tok::greatergreater, METRIC_TYPE_TOKEN_MOREMORE),
	std::make_pair(clang::tok::greatergreaterequal, METRIC_TYPE_TOKEN_MOREMORE_ASSIGN),
	std::make_pair(clang::tok::question, METRIC_TYPE_TOKEN_QUESTION),
	std::make_pair(clang::tok::l_square, METRIC_TYPE_TOKEN_LSQUARE),
	std::make_pair(clang::tok::r_square, METRIC_TYPE_TOKEN_RSQUARE),
	std::make_pair(clang::tok::caret, METRIC_TYPE_TOKEN_CARET),
	std::make_pair(clang::tok::caretequal, METRIC_TYPE_TOKEN_CARET_ASSIGN),
	std::make_pair(clang::tok::l_brace, METRIC_TYPE_TOKEN_LBRACE),
	std::make_pair(clang::tok::r_brace, METRIC_TYPE_TOKEN_RBRACE),
	std::make_pair(clang::tok::pipe, METRIC_TYPE_TOKEN_PIPE),
	std::make_pair(clang::tok::pipeequal, METRIC_TYPE_TOKEN_PIPE_ASSIGN),
	std::make_pair(clang::tok::tilde, METRIC_TYPE_TOKEN_TILDE)

};

const std::map<clang::tok::TokenKind,MetricType_e> MetricSrcLexer::m_tokenKindToTypeMap( tokenKindToTypeMapData,
    tokenKindToTypeMapData + sizeof tokenKindToTypeMapData / sizeof tokenKindToTypeMapData[0]);


MetricSrcLexer::MetricSrcLexer(clang::CompilerInstance &p_CI, MetricUnit* p_topUnit, MetricOptions* p_options) : m_compilerInstance(p_CI), 
	                                                                                                             m_topUnit( p_topUnit ), 
	                                                                                                             m_options( p_options ),
																												 m_currentUnit( NULL )
{
}

MetricSrcLexer::~MetricSrcLexer(void)
{
}

void MetricSrcLexer::HandleSemiToken( clang::Token& p_token )
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
		m_currentUnit->increment( METRIC_TYPE_HIS_STATEMENT );
		if( m_options->getDumpTokens() )
		{
			std::cout << ",statement-delimiter";
		}

	}
}

void MetricSrcLexer::HandleBasicToken( clang::Token& p_token )
{
	const clang::tok::TokenKind tokenKind = p_token.getKind();
	MetricSrcLexer::SemiConlonContainers_e start;
	MetricSrcLexer::SemiConlonContainers_e end;
	bool checkSccStart = false;
	bool checkSccEnd = false;

	std::map<clang::tok::TokenKind,MetricType_e>::const_iterator typeLookup = m_tokenKindToTypeMap.find( tokenKind );

	if( typeLookup != m_tokenKindToTypeMap.end() )
	{
		m_currentUnit->increment( (*typeLookup).second );
	}
	else
	{
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

void MetricSrcLexer::CountToken( clang::Token& p_token )
{		
	std::string tok_data;
	unsigned int tok_len = p_token.getLength();

	if( m_options->getDumpTokens() )
	{
		std::cout << "(" << p_token.getName();
	}

	switch( p_token.getKind() )
	{
		case clang::tok::raw_identifier:
			{
					if( m_options->getDumpTokens() )
					{
						std::cout << "raw_t,";
					}
				tok_data = clang::StringRef(p_token.getRawIdentifierData(), tok_len).str();
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
	}

	m_lastToken = p_token.getKind();
}

std::string MetricSrcLexer::FindFunction( clang::SourceManager& p_sm, clang::SourceLocation& p_loc, const SrcStartToFunctionMap_t* const p_fnMap )
{
	std::string ret_val = "";
	unsigned fileIdHash = p_sm.getFileID( p_loc ).getHashValue();
	SrcStartToFunctionMap_t::const_iterator file_it = p_fnMap->find(fileIdHash);
	if( file_it != p_fnMap->end() )
	{
		StartEndPair_t::const_iterator func_it = file_it->second.begin();

		/* While we've not found a matching function and there are still functions to consider ... */
		while(( ret_val == "" ) && ( func_it != file_it->second.end()))
		{
			/* Does the location we're considering match the function start or end or is it within those bounds? */
			if(( p_loc == (*func_it).first ) || 
				( p_loc == (*func_it).second.first ) ||
				( (*func_it).first < p_loc ) &&
				( p_loc < (*func_it).second.first ))
			{
				ret_val = (*func_it).second.second;
				break;
			}
			/* Next function in the map */
			func_it++;
		}
	}
	return ret_val;
}

void MetricSrcLexer::CloseOutFnOrMtd( void )
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

void MetricSrcLexer::LexSources( clang::CompilerInstance& p_ci, const SrcStartToFunctionMap_t* const p_fnMap )
{
	clang::Preprocessor &PP = p_ci.getPreprocessor();
	clang::SourceManager &SM = p_ci.getSourceManager();

	// Start preprocessing the specified input file.
	clang::Token result;
	PP.EnterMainSourceFile();
	PP.SetMacroExpansionOnlyInDirectives();
	PP.SetCommentRetentionState(true,true);

	do {
		bool shouldLexToken = true;

		PP.Lex(result);

		std::string fileName = SM.getFilename( result.getLocation() ).str();

//		CloseOutFnOrMtd();
		// TODO: Need to differentiate between "non-function" and "whole-file" level counts - e.g. 
		//  unique numerical constants that aren't in functions and unique numerical constants across the whole file
		if( SHOULD_INCLUDE_FILE( m_options, fileName ))
		{
			/* TODO: Could optimise this by not doing the function look-up for every single token, but 
				determining whether or not the token's position has exceeded the range of the current function */
			std::string funcName = FindFunction( SM, result.getLocation(), p_fnMap );
			if( funcName != m_currentFunctionName )
			{
				if( m_options->getDumpTokens() )
				{
					std::cout << "[fn:" << funcName << "]";
				}
				CloseOutFnOrMtd();
			}
			if( funcName != "" ) 
			{
				if( SHOULD_INCLUDE_FUNCTION( m_options, funcName ))
				{
					m_currentUnit = m_topUnit->getSubUnit(fileName, METRIC_UNIT_FILE)->getSubUnit(funcName, METRIC_UNIT_FUNCTION);
				}
				else
				{
					shouldLexToken = false;
				}
			}
			else
			{
				m_currentUnit = m_topUnit->getSubUnit(fileName, METRIC_UNIT_FILE);
			}
			m_currentFunctionName = funcName;

			if( shouldLexToken )
			{
				CountToken( result );
			}
		}
	} while (result.isNot(clang::tok::eof));

	for( clang::SourceManager::fileinfo_iterator it = SM.fileinfo_begin();
		it != SM.fileinfo_end();
		it++ )
	{
		bool Invalid = false;
		clang::FileID fid = SM.translateFile( it->first );
		clang::StringRef Buffer = SM.getBufferData(fid, &Invalid);
#if 0
		const llvm::MemoryBuffer* mb = p_sm.getMemoryBufferForFile(it->first, &Invalid);
		clang::Token result;
#endif
		std::string fileName = it->first->getName();

		if (Invalid)
		{
		   /* TODO: Something more useful */
			return;
		}
 
		if( SHOULD_INCLUDE_FILE( m_options, fileName ))
		{
			MetricUnit* fileUnit = m_topUnit->getSubUnit(fileName, METRIC_UNIT_FILE);
			fileUnit->set( METRIC_TYPE_LINE_COUNT, countNewlines( Buffer ) );

#if 0

			m_currentFileNumerics.clear();
			m_currentFileStrings.clear();
			m_currentFileCharConsts.clear();
			m_currentFileIdentifiers.clear();

			m_currentFileName = fileName;
			m_currentUnit = fileUnit;

			for( unsigned initLoop = 0 ; initLoop < SCC_MAX ; initLoop++ )
			{
				m_semiContainerDepth[ initLoop ] = 0;
				m_semiContainerOpen[ initLoop ] = false;
			}


			clang::Preprocessor& pp = m_compilerInstance.getPreprocessor();

#if 0 
			  Preprocessor(IntrusiveRefCntPtr<PreprocessorOptions> PPOpts,
               DiagnosticsEngine &diags, LangOptions &opts,
               const TargetInfo *target,
               SourceManager &SM, HeaderSearch &Headers,
               ModuleLoader &TheModuleLoader,
               IdentifierInfoLookup *IILookup = 0,
               bool OwnsHeaderSearch = false,
               bool DelayInitialization = false,
               bool IncrProcessing = false);
			  clang::DiagnosticOptions diagnosticOptions;
clang::TextDiagnosticPrinter *pTextDiagnosticPrinter =
    new clang::TextDiagnosticPrinter(
        llvm::outs(),
        diagnosticOptions);
llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs> pDiagIDs;
			  clang::DiagnosticsEngine *pDiagnosticsEngine =
    new clang::DiagnosticsEngine(pDiagIDs, pTextDiagnosticPrinter);
			  clang::Preprocessor new_pp(pDiagnosticsEngine,
				  pp.getDiagnostics(),
				  pp.getTargetInfo(),
				  pp.getSourceManager(),
				  pp.getHeaderSearchInfo(),
				  pp.getModuleLoader() );
#endif

			// Create a lexer starting at the beginning of this token.
#if 0
			  clang::Lexer TheLexer(fid, 
				                  mb,
								  pp);
#endif
			clang::Lexer TheLexer( p_sm.getLocForStartOfFile(fid),
				                  m_compilerInstance.getASTContext().getLangOpts(),
					         	  Buffer.begin(), 
								  Buffer.begin(), 
								  Buffer.end());

			TheLexer.SetCommentRetentionState(true);
			m_lastToken = clang::tok::eof;

			do
			{
				bool shouldLexToken = true;

				TheLexer.LexFromRawLexer(result);
				//new_pp.Lex( result );

				/* TODO: Could optimise this by not doing the function look-up for every single token, but 
				   determining whether or not the token's position has exceeded the range of the current function */
				std::string funcName = FindFunction( p_sm, result.getLocation(), p_fnMap );
				if( funcName != m_currentFunctionName )
				{
					if( m_options->getDumpTokens() )
					{
						std::cout << "[fn:" << funcName << "]";
					}
					CloseOutFnOrMtd();
				}
				if( funcName != "" ) 
				{
					if( SHOULD_INCLUDE_FUNCTION( m_options, funcName ))
					{
						m_currentUnit = m_topUnit->getSubUnit(fileName, METRIC_UNIT_FILE)->getSubUnit(funcName, METRIC_UNIT_FUNCTION);
					}
					else
					{
						shouldLexToken = false;
					}
				}
				else
				{
					m_currentUnit = m_topUnit->getSubUnit(fileName, METRIC_UNIT_FILE);
				}
				m_currentFunctionName = funcName;

				if(( result.getKind() == 0 ) || result.needsCleaning() )
				{
					std::string x = TheLexer.getSpelling( result, p_sm, m_compilerInstance.getASTContext().getLangOpts(), NULL );

					std::cout << std::endl << x << "/" << x.length() << std::endl;
				}

				if( shouldLexToken )
				{
					CountToken( result );
				}


			} while (result.isNot(clang::tok::eof));

			CloseOutFnOrMtd();
			fileUnit->set( METRIC_TYPE_TOKEN_NUMERIC_CONSTANTS_UNIQ, m_currentFileNumerics.size() );
			fileUnit->set( METRIC_TYPE_TOKEN_STRING_LITERALS_UNIQ, m_currentFileStrings.size() );
			fileUnit->set( METRIC_TYPE_TOKEN_UNRESERVED_IDENTIFIERS_UNIQ, m_currentFileIdentifiers.size() );
			fileUnit->set( METRIC_TYPE_TOKEN_CHAR_CONSTS_UNIQ, m_currentFileCharConsts.size() );
// TODO: Need to differentiate between "non-function" and "whole-file" level counts - e.g. unique numerical constants that aren't in functions and unique numerical constants across the whole file
#endif
		}
	} 
}