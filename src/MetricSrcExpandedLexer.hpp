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
#if !defined(METRIC_SRC_EXPANDED_LEXER_HPP)
#define METRIC_SRC_EXPANDED_LEXER_HPP

#include "MetricSrcLexer.hpp"

#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"

#include <cstdint>

class MetricSrcExpandedLexer : public MetricSrcLexer {
  protected:
    std::set<std::string> m_currentFnNumerics;
    std::set<std::string> m_currentFnStrings;
    std::set<std::string> m_currentFnCharConsts;
    std::set<std::string> m_currentFnIdentifiers;
    std::set<std::string> m_currentBodyIdentifiers;

    bool m_dumpNewline = false;

    static const std::map<clang::tok::TokenKind, MetricType_e> m_tokenKindToTypeMap;
    static const std::map<MetricType_e, MetricType_e> m_metricToBodyMetricMap;

    virtual void ProcessToken(clang::Token &p_token);
    virtual void CloseOutFnOrMtd(void);
    virtual void EnterFileScope(void);
    virtual MetricUnitProcessingType_e getLexType(void) const;
    void HandleOperand(clang::Token &p_token);

  public:
    MetricSrcExpandedLexer(clang::CompilerInstance &p_CI, MetricUnit *p_topUnit,
                           MetricOptions &p_options);
    virtual ~MetricSrcExpandedLexer(void);
};

#endif // !defined( METRIC_SRC_EXPANDED_LEXER_HPP )