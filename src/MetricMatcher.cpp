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
#include "clang/Lex/Preprocessor.h"

#include <sstream>
#include <iostream>

using namespace clang;
using namespace std;

//#define DEBUG_FN_TRACE_OUTOUT 1

MetricVisitor::MetricVisitor(clang::CompilerInstance &p_CI, MetricUnit* p_topUnit, MetricOptions* p_options) : m_compilerInstance(p_CI), 
																											   m_astContext(&(p_CI.getASTContext())),
	                                                                                                           m_topUnit( p_topUnit ), 
	                                                                                                           m_currentUnit( NULL ), m_options( p_options )
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

		m_currentFileName = m_astContext->getSourceManager().getFilename( func->getLocation() ).str();
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
			if( func->isInlineSpecified() )
			{
				fileUnit->increment( METRIC_TYPE_INLINE_FUNCTIONS );
			}

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

bool MetricVisitor::VisitTypedefDecl( clang::TypedefDecl* p_typeDef )
{
	if( p_typeDef->isDefinedOutsideFunctionOrMethod() )
	{
		SourceLocation loc = p_typeDef->getLocation();
		HandleLoc( loc );

		if( m_currentUnit )
		{
			m_currentUnit->increment( METRIC_TYPE_TYPEDEF_FILE );
		}
	}
	else
	{
		if( m_currentUnit )
		{
			m_currentUnit->increment( METRIC_TYPE_TYPEDEF_FN );
		}
	}

	return true;
}

void MetricVisitor::HandleLoc( SourceLocation& p_loc )
{
	if( p_loc.isMacroID() )
	{
		p_loc = m_astContext->getSourceManager().getFileLoc( p_loc );
    }

	m_currentFileName = m_astContext->getSourceManager().getFilename( p_loc ).str();
	m_currentFunctionName = "";

	if( SHOULD_INCLUDE_FILE( m_options, m_currentFileName ))
	{
		m_currentUnit = m_topUnit->getSubUnit(m_currentFileName, METRIC_UNIT_FILE);
	}
    else
	{
		m_currentUnit = NULL;
	}
}


bool MetricVisitor::VisitVarDecl(clang::VarDecl *p_varDec) {

#if defined( DEBUG_FN_TRACE_OUTOUT )
	std::cout << "VisitVarDecl : CONTEXT " << m_currentFileName << "::" << m_currentFunctionName << std::endl;
#endif

	/* Check it's not something like a function parameter */
	if( p_varDec->getKind() == clang::Decl::Var )
	{
		clang::StorageClass sc = p_varDec->getStorageClass();

		/* Check to see if this variable is file scope */
		if( p_varDec->isFileVarDecl() )
		{
			SourceLocation loc = p_varDec->getLocation();
			HandleLoc( loc );

			if( m_currentUnit )
			{
#if defined( DEBUG_FN_TRACE_OUTOUT )
			std::cout << "VisitVarDecl : Processing file-scope var " << p_varDec->getNameAsString() << std::endl;
#endif
				if ( sc == clang::SC_Extern )
				{
					m_currentUnit->increment( METRIC_TYPE_VARIABLE_FILE_EXTERN );
				}
				else
				{
					m_currentUnit->increment( METRIC_TYPE_VARIABLE_FILE_LOCAL );
					switch( sc )
					{
						case clang::SC_Static:
							m_currentUnit->increment( METRIC_TYPE_VARIABLE_FILE_STATIC );
							break;
					}
				}
			}

		}
		else
		{
			if( m_currentUnit )
			{
#if defined( DEBUG_FN_TRACE_OUTOUT )
		    	std::cout << "VisitVarDecl : Processing function-scope var " << p_varDec->getNameAsString() << std::endl;
#endif
				if ( sc == clang::SC_Extern )
				{
					m_currentUnit->increment( METRIC_TYPE_VARIABLE_FN_EXTERN );
				}
				else
				{
					m_currentUnit->increment( METRIC_TYPE_VARIABLE_FN_LOCAL );
					switch( sc )
					{
						case clang::SC_Static:
							m_currentUnit->increment( METRIC_TYPE_VARIABLE_FN_STATIC );
							break;
						case clang::SC_Register:
							m_currentUnit->increment( METRIC_TYPE_VARIABLE_FN_REGISTER );
							break;
						case clang::SC_Auto:
							m_currentUnit->increment( METRIC_TYPE_VARIABLE_FN_AUTO );
							break;
					}
				}
			}
		}
#if 0
        std::cout << "VisitVarDecl : Storage class " << p_varDec->getStorageClass() << std::endl;
        std::cout << "VisitVarDecl : Has Local Storage: " << p_varDec->hasLocalStorage() << std::endl;
        std::cout << "VisitVarDecl : Has Static Storage: " << p_varDec->isStaticLocal() << std::endl;
        std::cout << "VisitVarDecl : Is Local Var Decl: " << p_varDec->isLocalVarDecl() << std::endl;
        std::cout << "VisitVarDecl : Is File Var Decl: " << p_varDec->isFileVarDecl() << std::endl;
#endif
	} 
	else if( p_varDec->getKind() == clang::Decl::ParmVar )
	{
			if( m_currentUnit )
			{
				m_currentUnit->increment( METRIC_TYPE_FUNCTION_PARAMETERS );
			}
	}
	else
	{
	}

	return true;	
}

// TODO: Lots of repetition here - template it?
bool MetricVisitor::VisitForStmt(clang::ForStmt *p_forSt) {
	if( m_currentUnit )
	{
		m_currentUnit->set( METRIC_TYPE_NESTING_LEVEL, getControlDepth( p_forSt, m_astContext ));
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
		m_currentUnit->set( METRIC_TYPE_NESTING_LEVEL, getControlDepth( p_whileSt, m_astContext ));
		m_currentUnit->increment( METRIC_TYPE_LOOPS );
		m_currentUnit->increment( METRIC_TYPE_WHILELOOP );
	}
    return true;
}

bool MetricVisitor::VisitReturnStmt(clang::ReturnStmt *p_returnSt) {
	if( m_currentUnit )
	{
		m_currentUnit->increment( METRIC_TYPE_RETURN );

		if( !isLastExecutableStmtInFn( p_returnSt, m_astContext ) )
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
		m_currentUnit->set( METRIC_TYPE_NESTING_LEVEL, getControlDepth( p_switchSt, m_astContext ));
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
		m_currentUnit->increment( METRIC_TYPE_OPERATOR_TERNARY );
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

bool MetricVisitor::VisitUnaryExprOrTypeTraitExpr( clang::UnaryExprOrTypeTraitExpr* p_unaryExpr )
{
	if( m_currentUnit )
	{
		switch( p_unaryExpr->getKind() )
		{
			case clang::UETT_SizeOf:
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_SIZE_OF );
				break;
			case clang::UETT_AlignOf:
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_ALIGN_OF );
				break;
			case clang::UETT_VecStep:
				/* TODO */
				break;
			default:
#if defined( DEBUG_FN_TRACE_OUTOUT )
				std::cout << "VisitUnaryExprOrTypeTraitExpr - Unhandled operator" << std::endl;
#endif
				break;
		}
	}
	return true;
}

bool MetricVisitor::VisitExplicitCastExpr(clang::ExplicitCastExpr *p_castExpr)
{
	if( m_currentUnit )
	{
		m_currentUnit->increment( METRIC_TYPE_OPERATOR_CAST );
	}
	return true;
}

bool MetricVisitor::VisitMemberExpr( clang::MemberExpr* p_memberExpr )
{
	if( m_currentUnit )
	{
		if( p_memberExpr->isArrow() )
		{
			m_currentUnit->increment( METRIC_TYPE_OPERATOR_MEMBER_ACCESS_POINTER );
		} 
		else
		{
			m_currentUnit->increment( METRIC_TYPE_OPERATOR_MEMBER_ACCESS_DIRECT );
		}
	}
	return true;
}

bool MetricVisitor::VisitArraySubscriptExpr (clang::ArraySubscriptExpr *p_subs)
{
	if( m_currentUnit )
	{
		m_currentUnit->increment( METRIC_TYPE_OPERATOR_ARRAY_SUBSCRIPT );
	}
	return true;
}

bool MetricVisitor::VisitUnaryOperator(clang::UnaryOperator *p_uOp) {
	if( m_currentUnit )
	{
		switch( p_uOp->getOpcode() )
		{
			case clang::UO_PostInc:
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_ARITHMETIC_INCREMENT_POST );
				break;
			case clang::UO_PostDec:
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_ARITHMETIC_DECREMENT_POST );
				break;
			case clang::UO_PreInc:
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_ARITHMETIC_INCREMENT_PRE );
				break;
			case clang::UO_PreDec:
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_ARITHMETIC_DECREMENT_PRE);
				break;
			case clang::UO_AddrOf:
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_ADDRESS_OF );
				break;
			case clang::UO_Deref:
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_DEREFERENCE );
				break;
			case clang::UO_Plus:
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_ARITHMETIC_UNARY_PLUS );
				break; 	
			case clang::UO_Minus:
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_ARITHMETIC_UNARY_MINUS );
				break; 	
			case clang::UO_Not:
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_BITWISE_NOT );
				break; 	
			case clang::UO_LNot:
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_LOGICAL_NOT );
				break; 	
			case clang::UO_Real:
				/* TODO */
				break; 	
			case clang::UO_Imag:
				/* TODO */
				break; 	
			case clang::UO_Extension:
				/* TODO */
				break;
			default:

#if defined( DEBUG_FN_TRACE_OUTOUT )
				std::cout << "VisitUnaryOperator - Unhandled operator" << std::endl;
#endif
				break;
		}
	}
    return true;
}

bool MetricVisitor::VisitBinaryOperator(clang::BinaryOperator *p_binOp) {
	if( m_currentUnit )
	{
		switch( p_binOp->getOpcode() )
		{
			case clang::BO_PtrMemD:
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_PTR_TO_MEMBER_DIRECT );
				break;
			case clang::BO_PtrMemI:	
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_PTR_TO_MEMBER_INDIRECT );
				break;
			case clang::BO_Mul:
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_ARITHMETIC_MULTIPLICATION );
				break;
			case clang::BO_Div:	
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_ARITHMETIC_DIVISION );
				break;
			case clang::BO_Rem: 	
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_ARITHMETIC_MODULO );
				break;
			case clang::BO_Add: 	
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_ARITHMETIC_ADDITION );
				break;
			case clang::BO_Sub: 	
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_ARITHMETIC_SUBTRACTION );
				break;
			case clang::BO_Shl:	
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_SHIFT_LEFT );
				break;
			case clang::BO_Shr: 	
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_SHIFT_RIGHT );
				break;
			case clang::BO_LT: 	
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_COMP_LESS_THAN );
				break;
			case clang::BO_GT: 	
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_COMP_GREATER_THAN );
				break;
			case clang::BO_LE: 	
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_COMP_LESS_THAN_EQUAL );
				break;
			case clang::BO_GE: 	
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_COMP_GREATER_THAN_EQUAL );
				break;
			case clang::BO_EQ: 	
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_COMP_EQUAL );
				break;
			case clang::BO_NE: 	
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_COMP_NOT_EQUAL );
				break;
			case clang::BO_And: 	
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_BITWISE_AND );
				break;
			case clang::BO_Xor: 	
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_BITWISE_XOR );
				break;
			case clang::BO_Or: 	
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_BITWISE_OR );
				break;
			case clang::BO_LAnd:
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_LOGICAL_AND );
				break;
			case clang::BO_LOr:
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_LOGICAL_OR );
				break;
			case clang::BO_Assign:
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_ARITHMETIC_ASSIGN );
				break;
			case clang::BO_MulAssign:
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_ARITHMETIC_MULTIPLICATION_ASSIGN );
				break;
			case clang::BO_DivAssign:	
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_ARITHMETIC_DIVISION_ASSIGN );
				break;
			case clang::BO_RemAssign: 	
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_ARITHMETIC_MODULO_ASSIGN );
				break;
			case clang::BO_AddAssign: 	
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_ARITHMETIC_ADDITION_ASSIGN );
				break;
			case clang::BO_SubAssign: 	
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_ARITHMETIC_SUBTRACTION_ASSIGN );
				break;
			case clang::BO_ShlAssign: 	
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_SHIFT_LEFT_ASSIGN );
				break;
			case clang::BO_ShrAssign: 	
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_SHIFT_RIGHT_ASSIGN );
				break;
			case clang::BO_AndAssign:	
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_BITWISE_AND_ASSIGN );
				break;
			case clang::BO_XorAssign: 	
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_BITWISE_XOR_ASSIGN );
				break;
			case clang::BO_OrAssign: 	
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_BITWISE_OR_ASSIGN );
				break;
			case clang::BO_Comma:
				m_currentUnit->increment( METRIC_TYPE_OPERATOR_COMMA );
				break;
			default:
#if defined( DEBUG_FN_TRACE_OUTOUT )
				std::cout << "VisitBinaryOperator - Unhandled operator" << std::endl;
#endif
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
		m_currentUnit->set( METRIC_TYPE_NESTING_LEVEL, getControlDepth( p_ifSt, m_astContext ));
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
	m_topUnit->dump( out, p_output, p_fmt, m_options );
}

void MetricVisitor::LexTokens( void )
{
	clang::SourceManager& SM = m_astContext->getSourceManager();
		
   const llvm::MemoryBuffer *FromFile = SM.getBuffer(SM.getMainFileID());
   clang::Lexer RawLex(SM.getMainFileID(), FromFile, SM, m_astContext->getLangOpts());
  
   clang::Token RawTok;
   do {
     RawLex.LexFromRawLexer(RawTok);
     // If we have an identifier with no identifier info for our raw token, look
     // up the indentifier info.  This is important for equality comparison of
     // identifier tokens.
     if (RawTok.is(tok::raw_identifier))
	 {
       m_compilerInstance.getPreprocessor().LookUpIdentifierInfo(RawTok);
	 }
 
	 m_compilerInstance.getPreprocessor().DumpToken(RawTok);
   } while (RawTok.isNot(tok::eof));
}

bool MetricVisitor::TraverseDecl(clang::Decl *p_decl)
{
	bool ret_val;

	if( p_decl->getKind() == clang::Decl::TranslationUnit )
	{
		clang::SourceManager& SM = m_astContext->getSourceManager();

		for( clang::SourceManager::fileinfo_iterator x = SM.fileinfo_begin() ;
			x != SM.fileinfo_end() ;
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

