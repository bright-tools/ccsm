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

#if !defined( METRIC_UTILS_HPP )
#define       METRIC_UTILS_HPP

#include "clang/AST/ASTContext.h"

extern bool isLastExecutableStmtInFn(const clang::Stmt* const p_stmt, clang::ASTContext* p_context );
extern unsigned getControlDepth(const clang::Stmt* const p_stmt, clang::ASTContext* p_context );
extern size_t   countNewlines( clang::StringRef& p_buffer );

typedef std::pair< clang::SourceLocation, std::string > LocationNamePair_t;
typedef std::map< clang::SourceLocation, LocationNamePair_t> SrcStartToFunctionMap_t;

#endif
