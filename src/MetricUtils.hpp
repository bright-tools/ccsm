/*
   @file
   @brief Functions to support AST operations of any complexity that
          aren't supported out-of-the-box by clang.

   @author John Bailey
   @copyright Copyright 2013 John Bailey

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

#if !defined(METRIC_UTILS_HPP)
#define METRIC_UTILS_HPP

#include "MetricUnit.hpp"
#include "clang/AST/ASTContext.h"

extern unsigned getControlDepth(const clang::Stmt *const p_stmt, clang::ASTContext *p_context);
extern size_t countNewlines(clang::StringRef &p_buffer);
std::string makeRelative(const std::string &p_path);
std::string makeRelative(const std::string &p_path, const std::string &p_cwd);

extern const SourceFileAndLine_t InvalidSourceAndLine;

SourceFileAndLine_t getFileAndLine(const clang::SourceManager &p_sourceManager,
                                   const clang::SourceLocation *p_sourceLoc);

#endif
