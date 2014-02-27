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
#if !defined( METRIC_MATCHER_HPP )
#define       METRIC_MATCHER_HPP

#include "clang/Tooling/Tooling.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/ASTContext.h"
#include "clang/Frontend/CompilerInstance.h"

#include <ostream>
#include <iostream>
#include <string>

#include "MetricUnit.hpp"

class MetricVisitor : public clang::RecursiveASTVisitor<MetricVisitor>
{
protected:
	clang::ASTContext *astContext;
	MetricUnit*		  m_topUnit;
	std::string       m_currentFileName;
	std::string       m_currentFunctionName;
	MetricUnit*       m_currentUnit;

public:
    explicit MetricVisitor(clang::ASTContext* p_Context, MetricUnit* p_topUnit) : astContext(p_Context), m_topUnit( p_topUnit ), m_currentUnit( NULL )
    {
    }

	virtual bool VisitFunctionDecl(clang::FunctionDecl *func) {
     //   numFunctions++;
		m_currentFileName = astContext->getSourceManager().getFilename( func->getLocation() ).str();
        m_currentFunctionName = func->getNameInfo().getName().getAsString();

		m_currentUnit = m_topUnit->getSubUnit(m_currentFileName)->getSubUnit(m_currentFunctionName); 
		return true;     
	}     
	
	virtual bool VisitForStmt(clang::ForStmt *p_forSt) {
		if( m_currentUnit )
		{
			m_currentUnit->increment( METRIC_TYPE_FORLOOP );
		}
		return true;
	}

	virtual bool VisitIfStmt(clang::IfStmt *p_ifSt) {
		if( m_currentUnit )
		{
			m_currentUnit->increment( METRIC_TYPE_IF );

			if( p_ifSt->getElse() )
			{
				// TODO: This means that "else if" statements get counted as both an IF and an ELSE, which may not be what everyone wants
				m_currentUnit->increment( METRIC_TYPE_ELSE );
			}
		} else {
			/* TODO */
		}
        return true;
    }

	void dump( std::ostream& out )
	{
		m_topUnit->dump( out );
	}
};

class MetricASTConsumer : public clang::ASTConsumer
{
protected:
	MetricVisitor *visitor;

public:
    explicit MetricASTConsumer(clang::ASTContext *CI, MetricUnit* p_topUnit) : visitor(new MetricVisitor(CI, p_topUnit)) 
        { }

	virtual ~MetricASTConsumer(void) {};

	virtual void HandleTranslationUnit(clang::ASTContext &Context) {
		/* we can use ASTContext to get the TranslationUnitDecl, which is
		   a single Decl that collectively represents the entire source file */
		visitor->TraverseDecl(Context.getTranslationUnitDecl());
	}

	void dump( std::ostream& out )
	{
		visitor->dump( out );
	}
};




#endif
