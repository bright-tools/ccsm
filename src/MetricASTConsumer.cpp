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
#include "MetricSrcUnexpandedLexer.hpp"
#include "MetricSrcExpandedLexer.hpp"

#include "MetricASTConsumer.hpp"
#include "clang/Basic/SourceManager.h"

MetricASTConsumer::MetricASTConsumer(clang::CompilerInstance &CI, MetricUnit* p_topUnit, MetricOptions& p_options, GlobalFunctionLocator*  p_fnLocator ) :
																													  m_compilerInstance( CI ),
																													  m_options( p_options ),
																													  m_topUnit( p_topUnit ),
																													  m_fnLocator( p_fnLocator )
{ 
}

MetricASTConsumer::~MetricASTConsumer(void) 
{
}

#include <iostream>

void MetricASTConsumer::HandleTranslationUnit(clang::ASTContext &Context) 
{
	clang::SourceManager& SM = Context.getSourceManager();
	clang::TranslationUnitDecl* translationUnitDecl = Context.getTranslationUnitDecl();

#if 0
	std::cout << "HandleTranslationUnit::Processing " << SM.getFileEntryForID(SM.getMainFileID())->getName() << std::endl;
#endif

	TranslationUnitFunctionLocator* fnMap = m_fnLocator->getLocatorFor( SM.getFileEntryForID( SM.getMainFileID() )->getName() );

	MetricVisitor* visitor = new MetricVisitor(m_compilerInstance, m_topUnit, m_options, fnMap ); 

	/* we can use ASTContext to get the TranslationUnitDecl, which is
		a single Decl that collectively represents the entire source file */
	visitor->TraverseDecl( translationUnitDecl );

	/* Flag that all of the source files included in this AST tree have been processed. */
	for( clang::SourceManager::fileinfo_iterator it = SM.fileinfo_begin();
		 it != SM.fileinfo_end();
		 it++ )
	{
		std::string fileName = it->first->getName();

		if( m_options.ShouldIncludeFile( fileName ) &&
			!(m_options.isDefFile( fileName )))
		{
			MetricUnit* fileUnit = m_topUnit->getSubUnit(fileName, METRIC_UNIT_FILE);
			fileUnit->setProcessed( METRIC_UNIT_PROCESS_AST);
		}
	}

	delete( visitor );
}

MetricPPConsumer::MetricPPConsumer(MetricUnit* p_topUnit, MetricOptions& p_options, GlobalFunctionLocator*  p_fnLocator, const bool p_expanded)
	: clang::PreprocessorFrontendAction(),
	  m_options(p_options),
      m_topUnit( p_topUnit ),
      m_fnLocator( p_fnLocator ),
	  m_expanded( p_expanded )
{ 
}

MetricPPConsumer::~MetricPPConsumer(void) 
{
}

void MetricPPConsumer::ExecuteAction()
{
	MetricSrcLexer* srcLexer;
	if ( m_expanded )
	{
		srcLexer = new MetricSrcExpandedLexer(getCompilerInstance(), m_topUnit, m_options);
	}
	else
	{
		srcLexer = new MetricSrcUnexpandedLexer(getCompilerInstance(), m_topUnit, m_options);
	}
	clang::SourceManager& sm = getCompilerInstance().getSourceManager();
	std::string mainFileName = sm.getFileEntryForID( sm.getMainFileID() )->getName();
	
	srcLexer->LexSources( getCompilerInstance(), m_fnLocator->getLocatorFor( mainFileName ) );

	delete(srcLexer);
}
