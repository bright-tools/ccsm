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
#include "clang/Lex/Preprocessor.h"
#include "clang/AST/ASTContext.h"

#include <iostream>

MetricSrcLexer::MetricSrcLexer(clang::CompilerInstance &p_CI, MetricUnit* p_topUnit, MetricOptions* p_options) : m_compilerInstance(p_CI), 
	                                                                                                             m_topUnit( p_topUnit ), 
	                                                                                                             m_options( p_options ),
																												 m_currentUnit( NULL )
{
}

MetricSrcLexer::~MetricSrcLexer(void)
{
}

void MetricSrcLexer::CountToken( clang::Token& p_token )
{
	std::map<std::string,MetricType_e> tokenToTypeMap;

	tokenToTypeMap["auto"] = METRIC_TYPE_TOKEN_AUTO;
	tokenToTypeMap["const"] = METRIC_TYPE_TOKEN_CONST;
	tokenToTypeMap["inline"] = METRIC_TYPE_TOKEN_INLINE;
	tokenToTypeMap["extern"] = METRIC_TYPE_TOKEN_EXTERN;
	tokenToTypeMap["register"] = METRIC_TYPE_TOKEN_REGISTER;
	tokenToTypeMap["static"] = METRIC_TYPE_TOKEN_STATIC;
	tokenToTypeMap["typedef"] = METRIC_TYPE_TOKEN_TYPEDEF;
	tokenToTypeMap["virtual"] = METRIC_TYPE_TOKEN_VIRTUAL;
	tokenToTypeMap["mutable"] = METRIC_TYPE_TOKEN_MUTABLE;

	switch( p_token.getKind() )
	{
		case clang::tok::raw_identifier:
			{
			std::string x = clang::StringRef(p_token.getRawIdentifierData(), p_token.getLength()).str();
			std::map<std::string,MetricType_e>::const_iterator typeLookup = tokenToTypeMap.find( x );
			if( typeLookup != tokenToTypeMap.end() )
			{
				m_currentUnit->increment( (*typeLookup).second );
			}
			}
			break;
		default:
			/* TODO */
			break;
	}
}

void MetricSrcLexer::CountLines( clang::StringRef& p_buffer )
{
	size_t pos = 0;
	size_t cnt = 0;
	while(( pos = p_buffer.find( '\n', pos )) && ( pos != p_buffer.npos))
	{
		cnt++;
		pos++;
	}
	m_currentUnit->set( METRIC_TYPE_LINE_COUNT, cnt );
}

void MetricSrcLexer::LexSources( clang::SourceManager& p_sm )
{
	for( clang::SourceManager::fileinfo_iterator it = p_sm.fileinfo_begin();
		it != p_sm.fileinfo_end();
		it++ )
	{
		bool Invalid = false;
		clang::FileID fid = p_sm.translateFile( it->first );
		clang::StringRef Buffer = p_sm.getBufferData(fid, &Invalid);
		clang::Token result;
		std::string fileName = it->first->getName();

		if (Invalid)
		{
		   /* TODO: Something more useful */
			return;
		}
 
		if( SHOULD_INCLUDE_FILE( m_options, fileName ))
		{
			m_currentFileName = fileName;
			m_currentUnit = m_topUnit->getSubUnit(fileName, METRIC_UNIT_FILE);

			CountLines( Buffer );

			// Create a lexer starting at the beginning of this token.
			clang::Lexer TheLexer(p_sm.getLocForStartOfFile(fid), m_compilerInstance.getASTContext().getLangOpts(),
						   Buffer.begin(), Buffer.begin(), Buffer.end());

			TheLexer.SetCommentRetentionState(true);
		
			do
			{

				TheLexer.LexFromRawLexer(result);

				CountToken( result );

			} while (result.isNot(clang::tok::eof));
		}
	} 
}