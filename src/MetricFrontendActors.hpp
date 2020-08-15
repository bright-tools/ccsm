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
#if !defined(METRIC_FRONTEND_ACTORS_HPP)
#define METRIC_FRONTEND_ACTORS_HPP

#include "FunctionLocator.hpp"
#include "MetricOptions.hpp"
#include "MetricUnit.hpp"

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"

extern clang::tooling::FrontendActionFactory *
newPPMetricFrontendActionFactory(MetricOptions &p_options, MetricUnit *p_topUnit,
                                 GlobalFunctionLocator *p_srcMap, const bool p_expanded);
extern clang::tooling::FrontendActionFactory *
newASTMetricFrontendActionFactory(MetricOptions &p_options, MetricUnit *p_topUnit,
                                  GlobalFunctionLocator *p_srcMap,
                                  std::set<std::string> *p_commentFileList);

#endif
