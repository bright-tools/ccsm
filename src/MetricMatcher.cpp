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

#include "MetricUtils.hpp"

#include "MetricMatcher.hpp"
#include "clang/Basic/SourceManager.h"
#include "clang/AST/ASTContext.h"

#include <sstream>
#include <iostream>

using namespace clang;
using namespace std;

//#define DEBUG_FN_TRACE_OUTOUT 1

MetricVisitor::MetricVisitor(clang::ASTContext* p_Context, MetricUnit* p_topUnit, MetricOptions* p_options) : astContext(p_Context), m_topUnit( p_topUnit ), m_currentUnit( NULL ), m_options( p_options )
{
}

MetricVisitor::~MetricVisitor(void)
{
}

bool MetricVisitor::VisitFunctionDecl(clang::FunctionDecl *func) {
    
#if defined( DEBUG_FN_TRACE_OUTOUT )
	std::cout << "VisitFunctionDecl" << std::endl;
#endif
	if( func->doesThisDeclarationHaveABody() )
	{
		m_fnsCalled.clear();

		m_currentFileName = astContext->getSourceManager().getFilename( func->getLocation() ).str();
		m_currentFunctionName = func->getQualifiedNameAsString();

#if defined( DEBUG_FN_TRACE_OUTOUT )
		std::cout << "VisitFunctionDecl - " << m_currentFileName << "::"<< m_currentFunctionName << std::endl;
#endif

		if( SHOULD_INCLUDE_FILE( m_options, m_currentFileName ) && 
			SHOULD_INCLUDE_FUNCTION( m_options, m_currentFunctionName ))
		{
#if defined( DEBUG_FN_TRACE_OUTOUT )
			std::cout << "VisitFunctionDecl - Should be included" << std::endl;
#endif
			MetricUnit* fileUnit = m_topUnit->getSubUnit(m_currentFileName, METRIC_UNIT_FILE);

			MetricUnitType_e type = METRIC_UNIT_FUNCTION;

			if( func->isCXXClassMember() )
			{
				type = METRIC_UNIT_METHOD;
			}
		
			m_currentUnit = fileUnit->getSubUnit(m_currentFunctionName, type);

			if( func->getLinkageAndVisibility().getLinkage() == InternalLinkage )
			{
				fileUnit->increment( METRIC_TYPE_LOCAL_FUNCTIONS );
			}
			fileUnit->increment( METRIC_TYPE_FUNCTIONS );
		}
		else
		{
			m_currentUnit = NULL;
		}
	}
	return true;     
}     

bool MetricVisitor::VisitVarDecl(clang::VarDecl *p_varDec) {

#if defined( DEBUG_FN_TRACE_OUTOUT )
	std::cout << "VisitVarDecl : CONTEXT " << m_currentFileName << "::" << m_currentFunctionName << std::endl;
#endif

	/* Check it's not something like a function parameter */
	if( p_varDec->getKind() == clang::Decl::Var )
	{
		/* Check to see if this variable does not have a parent function/method */
		if( !p_varDec->getParentFunctionOrMethod() )
		{
			/* Don't count external references */
			if ( !p_varDec->hasExternalStorage() )
			{

				SourceLocation loc = p_varDec->getLocation();
				if( loc.isMacroID() )
				{
					loc = astContext->getSourceManager().getFileLoc(loc);
				}

				/* File-scope variable */
				m_currentFileName = astContext->getSourceManager().getFilename( loc ).str();
				m_currentFunctionName = "";
				if( SHOULD_INCLUDE_FILE( m_options, m_currentFileName ))
				{
#if defined( DEBUG_FN_TRACE_OUTOUT )
				std::cout << "VisitVarDecl : Processing " << p_varDec->getNameAsString() << std::endl;
#endif
					m_currentUnit = m_topUnit->getSubUnit(m_currentFileName, METRIC_UNIT_FILE);
					m_currentUnit->increment( METRIC_TYPE_VARIABLE );
				}
				else
				{
					m_currentUnit = NULL;
				}
			}
		}

	} else if( p_varDec->getKind() == clang::Decl::ParmVar )
	{
			if( m_currentUnit )
			{
				m_currentUnit->increment( METRIC_TYPE_FUNCTION_PARAMETERS );
			}
	}

	return true;	
}

// TODO: Lots of repetition here - template it?
bool MetricVisitor::VisitForStmt(clang::ForStmt *p_forSt) {
	if( m_currentUnit )
	{
		m_currentUnit->set( METRIC_TYPE_NESTING_LEVEL, getControlDepth( p_forSt, astContext ));
		m_currentUnit->increment( METRIC_TYPE_LOOPS );
		m_currentUnit->increment( METRIC_TYPE_FORLOOP );
	}
    return true;
}

bool MetricVisitor::VisitGotoStmt(clang::GotoStmt *p_gotoSt) {
	if( m_currentUnit )
	{
		m_currentUnit->increment( METRIC_TYPE_GOTO );
	}

    return true;
}

bool MetricVisitor::VisitLabelStmt(clang::LabelStmt *p_LabelSt) {
	if( m_currentUnit )
	{
		m_currentUnit->increment( METRIC_TYPE_LABEL );
	}
    return true;
}

bool MetricVisitor::VisitWhileStmt(clang::WhileStmt *p_whileSt) {
	if( m_currentUnit )
	{
		m_currentUnit->set( METRIC_TYPE_NESTING_LEVEL, getControlDepth( p_whileSt, astContext ));
		m_currentUnit->increment( METRIC_TYPE_LOOPS );
		m_currentUnit->increment( METRIC_TYPE_WHILELOOP );
	}
    return true;
}

bool MetricVisitor::VisitReturnStmt(clang::ReturnStmt *p_returnSt) {
	if( m_currentUnit )
	{
		m_currentUnit->increment( METRIC_TYPE_RETURN );

		if( !isLastExecutableStmtInFn( p_returnSt, astContext ) )
		{
			m_currentUnit->increment( METRIC_TYPE_RETURNPOINTS );
		}
	}
    return true;
}

bool MetricVisitor::VisitCallExpr(clang::CallExpr *p_callExpr)
{
	if( m_currentUnit )
	{
		clang::FunctionDecl* p_calleeFn = p_callExpr->getCalleeDecl()->getAsFunction();

		// Update the set containing the names of all the functions called
		// TODO: Does this work for C++ namespacing?
		m_fnsCalled.insert( p_calleeFn->getQualifiedNameAsString() );

		if( p_calleeFn->getBody() != NULL )
		{
			m_currentUnit->increment( METRIC_TYPE_LOCAL_FUNCTION_CALLS );
		}
		m_currentUnit->increment( METRIC_TYPE_FUNCTION_CALLS );
	}
    return true;
}

bool MetricVisitor::VisitSwitchStmt(clang::SwitchStmt *p_switchSt) {
	if( m_currentUnit )
	{
		m_currentUnit->set( METRIC_TYPE_NESTING_LEVEL, getControlDepth( p_switchSt, astContext ));
		m_currentUnit->increment( METRIC_TYPE_SWITCH );
		m_currentUnit->increment( METRIC_TYPE_DECISIONS );
	}
    return true;
}

bool MetricVisitor::VisitConditionalOperator(clang::ConditionalOperator *p_condOp) {
#if defined( DEBUG_FN_TRACE_OUTOUT )
	std::cout << "VisitConditionalOperator : " << m_currentFileName << "::" << m_currentFunctionName << std::endl;
#endif
	if( m_currentUnit )
	{
		m_currentUnit->increment( METRIC_TYPE_TERNARY );
	}
    return true;
}

bool MetricVisitor::VisitDefaultStmt(clang::DefaultStmt *p_defaultSt) {
	if( m_currentUnit )
	{
		m_currentUnit->increment( METRIC_TYPE_DEFAULT );
	}
    return true;
}

bool MetricVisitor::VisitCaseStmt(clang::CaseStmt *p_caseSt) {
	if( m_currentUnit )
	{
		m_currentUnit->increment( METRIC_TYPE_CASE );
	}
    return true;
}

bool MetricVisitor::VisitBinaryOperator(clang::BinaryOperator *p_binOp) {
	if( m_currentUnit )
	{
		switch( p_binOp->getOpcode() )
		{
			case clang::BO_LAnd:
				m_currentUnit->increment( METRIC_TYPE_LOGICAL_AND );
				break;
			case clang::BO_LOr:
				m_currentUnit->increment( METRIC_TYPE_LOGICAL_OR );
				break;
			default:
				break;
		}
	}
    return true;
}


bool MetricVisitor::VisitStmt(clang::Stmt *p_statement) {

	if( m_currentUnit )
	{
		m_currentUnit->increment( METRIC_TYPE_STATEMENTS );
	}
	return true;
}


bool MetricVisitor::VisitIfStmt(clang::IfStmt *p_ifSt) {
#if defined( DEBUG_FN_TRACE_OUTOUT )
	std::cout << "VisitIfStmt : CONTEXT " << m_currentFileName << "::" << m_currentFunctionName << std::endl;
#endif

	if( m_currentUnit )
	{
#if defined( DEBUG_FN_TRACE_OUTOUT )
	std::cout << "VisitIfStmt - Recorded" << std::endl;
#endif
		m_currentUnit->set( METRIC_TYPE_NESTING_LEVEL, getControlDepth( p_ifSt, astContext ));
		m_currentUnit->increment( METRIC_TYPE_IF );
		m_currentUnit->increment( METRIC_TYPE_DECISIONS );

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

void MetricVisitor::dump( std::ostream& out, const bool p_output[ METRIC_UNIT_MAX ], const MetricDumpFormat_e p_fmt )
{
	m_topUnit->dump( out, p_output, p_fmt );
}

bool MetricVisitor::TraverseDecl(clang::Decl *p_decl)
{
	bool ret_val;

	if( p_decl->getKind() == clang::Decl::TranslationUnit )
	{
		for( clang::SourceManager::fileinfo_iterator x = astContext->getSourceManager().fileinfo_begin() ;
			x != astContext->getSourceManager().fileinfo_end() ;
			x++ )
		{
			std::string name = (*x).first->getName();
#if 0
			/* TODO: Cache not valid at this point, so NumLines not valid :-( */
			std::cout << "   " << (*x).second->NumLines << std::endl;
#endif
			if( SHOULD_INCLUDE_FILE( m_options, name ))
			{
				MetricUnit* fileUnit = m_topUnit->getSubUnit(name, METRIC_UNIT_FILE);
				fileUnit->set( METRIC_TYPE_BYTE_COUNT, (*x).first->getSize() );
			}
		}
	}
	
	ret_val = clang::RecursiveASTVisitor<MetricVisitor>::TraverseDecl( p_decl );
	
	if( p_decl->getKind() == clang::Decl::Function )
	{
		if( p_decl->hasBody() )
		{
			if( m_currentUnit )
			{
				m_currentUnit->set( METRIC_TYPE_DIFFERENT_FUNCTIONS_CALLED, m_fnsCalled.size() );
			}
		}
	}

	return ret_val;
}

