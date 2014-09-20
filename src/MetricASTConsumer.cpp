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

#include "MetricASTConsumer.hpp"
#include "clang/Basic/SourceManager.h"

MetricASTConsumer::MetricASTConsumer(clang::CompilerInstance &CI, MetricUnit* p_topUnit, MetricOptions* p_options ) : visitor(new MetricVisitor(CI, p_topUnit, p_options)), 
																													  lexer(new MetricSrcLexer(CI, p_topUnit, p_options)),
																													  m_topUnit( p_topUnit )
{ 
}

MetricASTConsumer::~MetricASTConsumer(void) 
{
};

#include <iostream>

void MetricASTConsumer::HandleTranslationUnit(clang::ASTContext &Context) {
	clang::SourceManager& SM = Context.getSourceManager();

	/* we can use ASTContext to get the TranslationUnitDecl, which is
		a single Decl that collectively represents the entire source file */
	visitor->TraverseDecl(Context.getTranslationUnitDecl());
	lexer->LexSources(SM, visitor->getFunctionMap() );

	/* Flag that all of the source files included in this AST tree have been processed.
	   TODO: This breaks the "def file idiom", as the first time the file is include it will be flagged as having been processed.  
	         See issue #47 */
	for( clang::SourceManager::fileinfo_iterator it = SM.fileinfo_begin();
		 it != SM.fileinfo_end();
		 it++ )
	{
		std::string fileName = it->first->getName();
		MetricUnit* fileUnit = m_topUnit->getSubUnit(fileName, METRIC_UNIT_FILE);
		fileUnit->setProcessed();
	}
}

void MetricASTConsumer::dump( std::ostream& out, const bool p_output[ METRIC_UNIT_MAX ], const MetricDumpFormat_e p_fmt )
{
	visitor->dump( out, p_output, p_fmt );
}
