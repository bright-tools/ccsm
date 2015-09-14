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
#if !defined( METRIC_SRC_UNEXPANDED_LEXER_HPP )
#define       METRIC_SRC_UNEXPANDED_LEXER_HPP

#include "MetricSrcLexer.hpp"

#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"

#include <cstdint>

class MetricSrcUnexpandedLexer : public MetricSrcLexer
{
	protected:
		typedef enum {
			SCC_Define,
			SCC_For,
			SCC_Struct,
			SCC_Union,
			SCC_MAX
		} SemiConlonContainers_e;

		static const SemiConlonContainers_e m_sccStartLineEnding;
		static const SemiConlonContainers_e m_sccEndLineEnding;

		static const SemiConlonContainers_e m_sccStartParen;
		static const SemiConlonContainers_e m_sccEndParen;

		static const SemiConlonContainers_e m_sccStartBrace;
		static const SemiConlonContainers_e m_sccEndBrace;

		clang::tok::TokenKind   m_lastToken;
		std::set<std::string>   m_currentFnNumerics;
		std::set<std::string>   m_currentFnStrings;
		std::set<std::string>   m_currentFnCharConsts;
		std::set<std::string>   m_currentFnIdentifiers;

		std::set<std::string>   m_currentFileNumerics;
		std::set<std::string>   m_currentFileStrings;
		std::set<std::string>   m_currentFileCharConsts;
		std::set<std::string>   m_currentFileIdentifiers;

		uint32_t				m_semiContainerDepth[ SCC_MAX ];
		bool				    m_semiContainerOpen[ SCC_MAX ];
		bool                    m_semiHasContent;
		bool					m_dumpNewline;

		static const std::map<clang::tok::TokenKind,MetricType_e> m_tokenKindToTypeMap;

		virtual void ProcessToken( clang::Token& p_token );
		void CloseOutFnOrMtd( void );
		void HandleBasicToken( clang::Token& p_token );
		void HandleSemiToken( clang::Token& p_token );
		virtual MetricUnitProcessingType_e getLexType(void) const;

	public:
		MetricSrcUnexpandedLexer(clang::CompilerInstance &p_CI, MetricUnit* p_topUnit, MetricOptions* p_options = NULL);
	    virtual ~MetricSrcUnexpandedLexer(void);

		virtual void LexSources(clang::CompilerInstance& p_ci, const TranslationUnitFunctionLocator* const p_fnLocator);
};

#endif     // !defined( METRIC_SRC_LEXER_HPP )