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

#include <sstream>
#include <iostream>

#include "MetricMatcher.hpp"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Lexer.h"
#include "clang/AST/ASTContext.h"

using namespace clang;
using namespace std;

#if 0
MetricMatcher::MetricMatcher(void) : m_currentCU( NULL )
{
}

MetricMatcher::~MetricMatcher(void)
{
}

void MetricMatcher::run(const clang::ast_matchers::MatchFinder::MatchResult &Result)
{
	SourceLocation sl;
	if (const IfStmt *IS = Result.Nodes.getNodeAs<clang::IfStmt>("ifStmt"))
	{
		sl = IS->getLocStart();
	}
		
	if( sl.isValid() )
	{
		std::string fileName = Result.Context->getSourceManager().getFilename( sl ).str();
		std::string fileName = Result.Context->getSourceManager().getF
		std::cout << "Found Match in " << fn;
		MetricUnit* file = m_unit.getSubUnit( fileName );
		MetricUnit* func = file->getSubUnit( funcName );
		
	}
}

void MetricMatcher::dump( ostream& out )
{
}
#endif