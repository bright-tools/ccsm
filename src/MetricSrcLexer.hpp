/*
   @file
   @brief TODO

   @author John Bailey
   @copyright Copyright 2014 John Bailey

   @section LICENSE

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
#if !defined( METRIC_SRC_LEXER_HPP )
#define       METRIC_SRC_LEXER_HPP

#include "MetricUnit.hpp"
#include "MetricOptions.hpp"
#include "MetricUtils.hpp"

#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"

class MetricSrcLexer
{
	protected:
		clang::CompilerInstance &m_compilerInstance;
		MetricUnit*		        m_topUnit;
		MetricOptions*          m_options;
		MetricUnit*             m_currentUnit;
		std::string             m_currentFileName;
		std::set<std::string>   m_currentFnNumerics;
		std::set<std::string>   m_currentFnStrings;
		std::set<std::string>   m_currentFnCharConsts;
		std::set<std::string>   m_currentFnIdentifiers;

		std::set<std::string>   m_currentFileNumerics;
		std::set<std::string>   m_currentFileStrings;
		std::set<std::string>   m_currentFileCharConsts;
		std::set<std::string>   m_currentFileIdentifiers;

		std::string             m_currentFunctionName;

		static const std::map<std::string,MetricType_e> m_tokenToTypeMap;
		static const std::map<clang::tok::TokenKind,MetricType_e> m_tokenKindToTypeMap;

		void CountToken( clang::Token& p_token );
		std::string FindFunction( clang::SourceManager& p_sm, clang::SourceLocation& p_loc, const SrcStartToFunctionMap_t* const p_fnMap );
		void CloseOutFnOrMtd( void );

	public:
		MetricSrcLexer(clang::CompilerInstance &p_CI, MetricUnit* p_topUnit,MetricOptions* p_options = NULL);
	    virtual ~MetricSrcLexer(void);

		void LexSources( clang::SourceManager& p_sm, const SrcStartToFunctionMap_t* const p_fnMap );
};

#endif     // !defined( METRIC_SRC_LEXER_HPP )