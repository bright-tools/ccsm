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
#if !defined(METRIC_AST_CONSUMER_HPP)
#define METRIC_AST_CONSUMER_HPP

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"

#include <ostream>
#include <string>
#include <vector>

#include "FunctionLocator.hpp"
#include "MetricMatcher.hpp"
#include "MetricOptions.hpp"

class MetricASTConsumer : public clang::ASTConsumer {
  protected:
    clang::CompilerInstance &m_compilerInstance;
    MetricOptions &m_options;
    MetricUnit *m_topUnit = NULL;
    GlobalFunctionLocator *m_fnLocator = NULL;

  public:
    explicit MetricASTConsumer(clang::CompilerInstance &CI,
                               MetricUnit *p_topUnit, MetricOptions &p_options,
                               GlobalFunctionLocator *m_fnLocator = NULL);

    virtual ~MetricASTConsumer(void);
    virtual void HandleTranslationUnit(clang::ASTContext &Context);
};

class MetricPPConsumer : public clang::PreprocessorFrontendAction {
  protected:
    MetricOptions &m_options;
    MetricUnit *m_topUnit = NULL;
    GlobalFunctionLocator *m_fnLocator = NULL;
    bool m_expanded;

  public:
    explicit MetricPPConsumer(MetricUnit *p_topUnit, MetricOptions &p_options,
                              GlobalFunctionLocator *m_fnLocator = NULL,
                              const bool p_expanded = true);

    virtual ~MetricPPConsumer(void);
    virtual void ExecuteAction();
};

#endif
