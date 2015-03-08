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

#include <sstream>
#include <iostream>

using namespace clang;
using namespace std;

MetricVisitor::MetricVisitor(clang::CompilerInstance &p_CI, MetricUnit* p_topUnit, MetricOptions* p_options, TranslationUnitFunctionLocator* p_fnLocator) : 
	                                                                                                           m_compilerInstance(p_CI), 
																											   m_astContext(&(p_CI.getASTContext())),
	                                                                                                           m_topUnit( p_topUnit ), 
	                                                                                                           m_currentUnit( NULL ), 
																											   m_options( p_options ),
																											   m_fnLocator( p_fnLocator )
{
}

MetricVisitor::~MetricVisitor(void)
{
}

void MetricVisitor::UpdateCurrentFileName( const clang::SourceLocation &loc )
{
	clang::SourceManager& sm =  m_astContext->getSourceManager();
	SourceLocation sLoc = loc;
	std::string declFn = sm.getFilename( sLoc ).str();

	while( m_options->isDefFile( declFn ) && sLoc.isValid() )
	{
		clang::FullSourceLoc sl = m_astContext->getFullLoc( sLoc );
		sLoc = sm.getIncludeLoc( sl.getFileID() );
		if( sLoc.isValid() )
		{
			declFn = sm.getFilename( sLoc ).str();
		}
	} 

	m_currentFileName = declFn;
}

bool MetricVisitor::VisitFunctionDecl(clang::FunctionDecl *func) {
    
#if defined( DEBUG_FN_TRACE_OUTOUT )
	std::cout << "VisitFunctionDecl - CONTEXT " << m_currentFileName << "::" << m_currentFunctionName << std::endl;
#endif

	/* Deal with common actions which may be applicable to a decl valid at translation-unit level */
	DeclCommon( func->getLexicalParent(), func );

	/* Function body attached? */
	if( func->doesThisDeclarationHaveABody() )
	{		
		SourceLocation funcLoc = func->getLocation();
		if( funcLoc.isMacroID() )
		{
			funcLoc = m_astContext->getSourceManager().getFileLoc( funcLoc );
		}

		UpdateCurrentFileName( funcLoc );
		
		m_currentFunctionName = func->getQualifiedNameAsString();

		if( m_fnLocator != NULL )
		{
			m_fnLocator->addFunctionLocation( m_astContext, m_currentFunctionName, func );
		}

		if( ShouldIncludeFile( m_currentFileName ) && 
			SHOULD_INCLUDE_FUNCTION( m_options, m_currentFunctionName ))
		{
			/* Ensure that there is a file-level sub-unit */
			MetricUnit* fileUnit = m_topUnit->getSubUnit(m_currentFileName, METRIC_UNIT_FILE);

			MetricUnitType_e type = METRIC_UNIT_FUNCTION;

			if( func->isCXXClassMember() )
			{
				type = METRIC_UNIT_METHOD;
			}
		
			m_currentUnit = fileUnit->getSubUnit(m_currentFunctionName, type);

			/* Obtain the buffer for the function body, then count the lines */ 
			const char * start = m_astContext->getSourceManager().getCharacterData( func->getBody()->getLocStart() );
			const char * end = m_astContext->getSourceManager().getCharacterData( func->getBody()->getLocEnd() );
			StringRef buffer( start, end-start );
			m_currentUnit->set( METRIC_TYPE_FUNCTION_LINE_COUNT, countNewlines( buffer ) );

			if( func->isInlineSpecified() )
			{
				IncrementMetric( fileUnit, METRIC_TYPE_INLINE_FUNCTIONS );
			}

			switch( func->getLinkageAndVisibility().getLinkage() )
			{
				case clang::Linkage::InternalLinkage:
					IncrementMetric( fileUnit, METRIC_TYPE_LOCAL_FUNCTIONS );
					break;
				default:
					/* Not interested at the moment */
					break;
			}
			IncrementMetric( fileUnit, METRIC_TYPE_FUNCTIONS );
		}
		else
		{
			/* Not including this function */
			m_currentUnit = NULL;
		}
	}
	else
	{
		/* No body to this function */

		if( m_currentUnit )
		{
			switch( func->getStorageClass() )
			{
				case clang::SC_None:
					/* No storage class specified - implicitly the function is extern */
					IncrementMetric( m_currentUnit,  METRIC_TYPE_EXTERN_IMPL_FUNCTIONS );
					break;
				case clang::SC_Extern:
					IncrementMetric( m_currentUnit, METRIC_TYPE_EXTERN_EXPL_FUNCTIONS );
					break;
				default:
					/* Not currently of interest */
					break;
			}
		}
	}
	return true;     
}     

bool MetricVisitor::VisitTypedefDecl( clang::TypedefDecl* p_typeDef )
{
#if defined( DEBUG_FN_TRACE_OUTOUT )
	std::cout << "VisitTypedefDecl - CONTEXT " << m_currentFileName << "::" << m_currentFunctionName << std::endl;
#endif

	/* Deal with common actions which may be applicable to a decl valid at translation-unit level */
	DeclCommon( p_typeDef->getLexicalDeclContext(), p_typeDef );

	if( m_currentUnit )
	{
		/* Is it a "top level" decl? */
		if( p_typeDef->isDefinedOutsideFunctionOrMethod() )
		{
			IncrementMetric( m_currentUnit, METRIC_TYPE_TYPEDEF_FILE );
		}
		else
		{
			IncrementMetric( m_currentUnit, METRIC_TYPE_TYPEDEF_FN );
		}
	}

	return true;
}

void MetricVisitor::CloseOutFnOrMtd( void )
{
	/* Check to see if we've just finished processing a function/method - if so, finish up any
		required counts */
	if( m_currentUnit )
	{
		if( m_currentUnit->isFnOrMethod() )
		{
			m_currentUnit->set( METRIC_TYPE_DIFFERENT_FUNCTIONS_CALLED, m_fnsCalled.size() );
			m_fnsCalled.clear();
		}
	}
}

void MetricVisitor::DeclCommon( const clang::DeclContext* p_declCtxt, const clang::Decl* p_decl )
{
#if defined( DEBUG_FN_TRACE_OUTOUT )
	std::cout << "DeclCommon - CONTEXT " << m_currentFileName << "::" << m_currentFunctionName << std::endl;
#endif

	// Check to see if the decl is top-level - may need to update m_currentUnit after exiting a function.
	if( p_declCtxt->getDeclKind() == clang::Decl::TranslationUnit )
	{
		CloseOutFnOrMtd();

		HandleLoc( p_decl->getLocation() );
	}
}

void MetricVisitor::HandleLoc( SourceLocation& p_loc )
{
	if( p_loc.isMacroID() )
	{
		p_loc = m_astContext->getSourceManager().getFileLoc( p_loc );
    }

	m_currentFunctionName = "";
	UpdateCurrentFileName( p_loc );

	if( ShouldIncludeFile( m_currentFileName ))
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
	std::cout << "VisitVarDecl : Processing " << p_varDec->getNameAsString() << std::endl;
#endif

	/* Deal with common actions which may be applicable to a decl valid at translation-unit level */
	DeclCommon( p_varDec->getLexicalDeclContext(), p_varDec );

	if( m_currentUnit )
	{
		/* Check it's not something like a function parameter */
		if( p_varDec->getKind() == clang::Decl::Var )
		{
			clang::StorageClass sc = p_varDec->getStorageClass();

			/* Check to see if this variable is file scope */
			if( p_varDec->isFileVarDecl() )
			{
#if defined( DEBUG_FN_TRACE_OUTOUT )
				std::cout << "VisitVarDecl : Processing file-scope var " << p_varDec->getNameAsString() << std::endl;
#endif
				if ( sc == clang::SC_Extern )
				{
					IncrementMetric( m_currentUnit, METRIC_TYPE_VARIABLE_FILE_EXTERN );
				}
				else
				{
					IncrementMetric( m_currentUnit, METRIC_TYPE_VARIABLE_FILE_LOCAL );
					switch( sc )
					{
						case clang::SC_Static:
							IncrementMetric( m_currentUnit, METRIC_TYPE_VARIABLE_FILE_STATIC );
							break;
						default:
							/* Not currently interested */
							break;
					}
				}
			}
			else
			{
				if ( sc == clang::SC_Extern )
				{
					IncrementMetric( m_currentUnit, METRIC_TYPE_VARIABLE_FN_EXTERN );
				}
				else
				{
					IncrementMetric( m_currentUnit, METRIC_TYPE_VARIABLE_FN_LOCAL );
					switch( sc )
					{
						case clang::SC_Static:
							IncrementMetric( m_currentUnit, METRIC_TYPE_VARIABLE_FN_STATIC );
							break;
						case clang::SC_Register:
							IncrementMetric( m_currentUnit, METRIC_TYPE_VARIABLE_FN_REGISTER );
							break;
						case clang::SC_Auto:
							IncrementMetric( m_currentUnit, METRIC_TYPE_VARIABLE_FN_AUTO );
							break;
						default:
							/* Not currently of interest */
							break;
					}
				}
			}
		} 
		else if( p_varDec->getKind() == clang::Decl::ParmVar )
		{
			IncrementMetric( m_currentUnit, METRIC_TYPE_FUNCTION_PARAMETERS );
		}
		else
		{
		}
	}

	return true;	
}

// TODO: Lots of repetition here - template it?
bool MetricVisitor::VisitForStmt(clang::ForStmt *p_forSt) 
{
#if defined( DEBUG_FN_TRACE_OUTOUT )
	std::cout << "VisitForStmt - CONTEXT " << m_currentFileName << "::" << m_currentFunctionName << std::endl;
#endif

	if( m_currentUnit )
	{
		m_currentUnit->set( METRIC_TYPE_NESTING_LEVEL, getControlDepth( p_forSt, m_astContext ));
		IncrementMetric( m_currentUnit, METRIC_TYPE_LOOPS );
		IncrementMetric( m_currentUnit, METRIC_TYPE_FORLOOP );
	}
    return true;
}

bool MetricVisitor::VisitGotoStmt(clang::GotoStmt *p_gotoSt) 
{
	if( m_currentUnit )
	{
		IncrementMetric( m_currentUnit, METRIC_TYPE_GOTO );
	}

    return true;
}

bool MetricVisitor::VisitLabelStmt(clang::LabelStmt *p_LabelSt) 
{
	if( m_currentUnit )
	{
		IncrementMetric( m_currentUnit, METRIC_TYPE_LABEL );
	}
    return true;
}

bool MetricVisitor::VisitWhileStmt(clang::WhileStmt *p_whileSt) 
{
	if( m_currentUnit )
	{
		m_currentUnit->set( METRIC_TYPE_NESTING_LEVEL, getControlDepth( p_whileSt, m_astContext ));
		IncrementMetric( m_currentUnit, METRIC_TYPE_LOOPS );
		IncrementMetric( m_currentUnit, METRIC_TYPE_WHILELOOP );
	}
    return true;
}

bool MetricVisitor::VisitReturnStmt(clang::ReturnStmt *p_returnSt) 
{
	if( m_currentUnit )
	{
		IncrementMetric( m_currentUnit, METRIC_TYPE_RETURN );

		if( !isLastExecutableStmtInFn( p_returnSt, m_astContext ) )
		{
			IncrementMetric( m_currentUnit, METRIC_TYPE_RETURNPOINTS );
		}
	}
    return true;
}

bool MetricVisitor::VisitCallExpr(clang::CallExpr *p_callExpr)
{
#if defined( DEBUG_FN_TRACE_OUTOUT )
	std::cout << "VisitCallExpr : CONTEXT " << m_currentFileName << "::" << m_currentFunctionName << std::endl;
#endif

	if( m_currentUnit )
	{
		clang::FunctionDecl* p_calleeFn = p_callExpr->getCalleeDecl()->getAsFunction();

		/* Check the function could be determined - probably not in the case of a function pointer */
		if( p_calleeFn != NULL )
		{
			std::string calleeName = p_calleeFn->getQualifiedNameAsString();

#if defined( DEBUG_FN_TRACE_OUTOUT )
			std::cout << "VisitCallExpr : Function call to " << calleeName << std::endl;
#endif

			/* Not registered this function as being called yet? */
			// TODO: Does this work for C++ namespacing?
			if( m_fnsCalled.find( calleeName ) == m_fnsCalled.end() ) {

				// Update the set containing the names of all the functions called
				m_fnsCalled.insert( calleeName );

#if defined( DEBUG_FN_TRACE_OUTOUT )
				std::cout << "VisitCallExpr : Not yet registered as called" << std::endl;
#endif

				/* TODO: This look-up doesn't work when the callee is in a different TU :-| */
				Stmt* calleeBody = p_callExpr->getDirectCallee()->getBody();

				if( calleeBody ) {

					SourceLocation calleeBodyLocation = calleeBody->getLocStart();
					std::string name = m_astContext->getSourceManager().getFilename(calleeBodyLocation);

#if defined( DEBUG_FN_TRACE_OUTOUT )
					std::cout << "VisitCallExpr : Found function body in " << name << std::endl;
#endif

					if( ShouldIncludeFile( name ))
					{
						if( SHOULD_INCLUDE_FUNCTION( m_options, calleeName ))
						{
							MetricUnit* fileUnit = m_topUnit->getSubUnit( name, METRIC_UNIT_FILE );
							MetricUnit* targFn = fileUnit->getSubUnit( calleeName, METRIC_UNIT_FUNCTION );
							IncrementMetric( targFn, METRIC_TYPE_CALLED_BY, fileUnit );
						}
						else
						{
#if defined( DEBUG_FN_TRACE_OUTOUT )
							std::cout << "VisitCallExpr : Function is in list to be ignored" << std::endl;
#endif
						}
					}
					else
					{
#if defined( DEBUG_FN_TRACE_OUTOUT )
						std::cout << "VisitCallExpr : Function in ignored file" << std::endl;
#endif
					}
				}
				else
				{
#if defined( DEBUG_FN_TRACE_OUTOUT )
					std::cout << "VisitCallExpr : Function has no body" << std::endl;
#endif
				}
			}
			else
			{
#if defined( DEBUG_FN_TRACE_OUTOUT )
				std::cout << "VisitCallExpr : Already registered as called" << std::endl;
#endif
			}

			if( p_calleeFn->getBody() != NULL )
			{
				IncrementMetric( m_currentUnit, METRIC_TYPE_LOCAL_FUNCTION_CALLS );
			}
		}
		else
		{
#if defined( DEBUG_FN_TRACE_OUTOUT )
			std::cout << "VisitCallExpr : Not able to determin name of called function" << std::endl;
#endif
			// TODO: Number of functions called isn't incremented, which might be misleading ... should document this behaviour at least
		}
		IncrementMetric( m_currentUnit, METRIC_TYPE_FUNCTION_CALLS );
	}

    return true;
}

bool MetricVisitor::VisitSwitchStmt(clang::SwitchStmt *p_switchSt) 
{
	if( m_currentUnit )
	{
		m_currentUnit->set( METRIC_TYPE_NESTING_LEVEL, getControlDepth( p_switchSt, m_astContext ));
		IncrementMetric( m_currentUnit, METRIC_TYPE_SWITCH );
		IncrementMetric( m_currentUnit, METRIC_TYPE_DECISIONS );
	}
    return true;
}

bool MetricVisitor::VisitConditionalOperator(clang::ConditionalOperator *p_condOp) 
{
#if defined( DEBUG_FN_TRACE_OUTOUT )
	std::cout << "VisitConditionalOperator : " << m_currentFileName << "::" << m_currentFunctionName << std::endl;
#endif
	if( m_currentUnit )
	{
		IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_TERNARY );
	}
    return true;
}

bool MetricVisitor::VisitDefaultStmt(clang::DefaultStmt *p_defaultSt) 
{
	if( m_currentUnit )
	{
		IncrementMetric( m_currentUnit, METRIC_TYPE_DEFAULT );
	}
    return true;
}

bool MetricVisitor::VisitCaseStmt(clang::CaseStmt *p_caseSt) 
{
	if( m_currentUnit )
	{
		IncrementMetric( m_currentUnit, METRIC_TYPE_CASE );
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
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_SIZE_OF );
				break;
			case clang::UETT_AlignOf:
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_ALIGN_OF );
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
		IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_CAST );
	}
	return true;
}

bool MetricVisitor::VisitMemberExpr( clang::MemberExpr* p_memberExpr )
{
	if( m_currentUnit )
	{
		if( p_memberExpr->isArrow() )
		{
			IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_MEMBER_ACCESS_POINTER );
		} 
		else
		{
			IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_MEMBER_ACCESS_DIRECT );
		}
	}
	return true;
}

bool MetricVisitor::VisitArraySubscriptExpr (clang::ArraySubscriptExpr *p_subs)
{
	if( m_currentUnit )
	{
		IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_ARRAY_SUBSCRIPT );
	}
	return true;
}

bool MetricVisitor::VisitUnaryOperator(clang::UnaryOperator *p_uOp) 
{
	if( m_currentUnit )
	{
		switch( p_uOp->getOpcode() )
		{
			case clang::UO_PostInc:
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_ARITHMETIC_INCREMENT_POST );
				break;
			case clang::UO_PostDec:
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_ARITHMETIC_DECREMENT_POST );
				break;
			case clang::UO_PreInc:
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_ARITHMETIC_INCREMENT_PRE );
				break;
			case clang::UO_PreDec:
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_ARITHMETIC_DECREMENT_PRE);
				break;
			case clang::UO_AddrOf:
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_ADDRESS_OF );
				break;
			case clang::UO_Deref:
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_DEREFERENCE );
				break;
			case clang::UO_Plus:
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_ARITHMETIC_UNARY_PLUS );
				break; 	
			case clang::UO_Minus:
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_ARITHMETIC_UNARY_MINUS );
				break; 	
			case clang::UO_Not:
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_BITWISE_NOT );
				break; 	
			case clang::UO_LNot:
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_LOGICAL_NOT );
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

bool MetricVisitor::VisitBinaryOperator(clang::BinaryOperator *p_binOp) 
{
	if( m_currentUnit )
	{
		switch( p_binOp->getOpcode() )
		{
			case clang::BO_PtrMemD:
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_PTR_TO_MEMBER_DIRECT );
				break;
			case clang::BO_PtrMemI:	
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_PTR_TO_MEMBER_INDIRECT );
				break;
			case clang::BO_Mul:
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_ARITHMETIC_MULTIPLICATION );
				break;
			case clang::BO_Div:	
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_ARITHMETIC_DIVISION );
				break;
			case clang::BO_Rem: 	
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_ARITHMETIC_MODULO );
				break;
			case clang::BO_Add: 	
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_ARITHMETIC_ADDITION );
				break;
			case clang::BO_Sub: 	
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_ARITHMETIC_SUBTRACTION );
				break;
			case clang::BO_Shl:	
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_SHIFT_LEFT );
				break;
			case clang::BO_Shr: 	
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_SHIFT_RIGHT );
				break;
			case clang::BO_LT: 	
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_COMP_LESS_THAN );
				break;
			case clang::BO_GT: 	
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_COMP_GREATER_THAN );
				break;
			case clang::BO_LE: 	
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_COMP_LESS_THAN_EQUAL );
				break;
			case clang::BO_GE: 	
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_COMP_GREATER_THAN_EQUAL );
				break;
			case clang::BO_EQ: 	
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_COMP_EQUAL );
				break;
			case clang::BO_NE: 	
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_COMP_NOT_EQUAL );
				break;
			case clang::BO_And: 	
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_BITWISE_AND );
				break;
			case clang::BO_Xor: 	
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_BITWISE_XOR );
				break;
			case clang::BO_Or: 	
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_BITWISE_OR );
				break;
			case clang::BO_LAnd:
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_LOGICAL_AND );
				break;
			case clang::BO_LOr:
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_LOGICAL_OR );
				break;
			case clang::BO_Assign:
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_ARITHMETIC_ASSIGN );
				break;
			case clang::BO_MulAssign:
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_ARITHMETIC_MULTIPLICATION_ASSIGN );
				break;
			case clang::BO_DivAssign:	
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_ARITHMETIC_DIVISION_ASSIGN );
				break;
			case clang::BO_RemAssign: 	
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_ARITHMETIC_MODULO_ASSIGN );
				break;
			case clang::BO_AddAssign: 	
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_ARITHMETIC_ADDITION_ASSIGN );
				break;
			case clang::BO_SubAssign: 	
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_ARITHMETIC_SUBTRACTION_ASSIGN );
				break;
			case clang::BO_ShlAssign: 	
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_SHIFT_LEFT_ASSIGN );
				break;
			case clang::BO_ShrAssign: 	
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_SHIFT_RIGHT_ASSIGN );
				break;
			case clang::BO_AndAssign:	
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_BITWISE_AND_ASSIGN );
				break;
			case clang::BO_XorAssign: 	
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_BITWISE_XOR_ASSIGN );
				break;
			case clang::BO_OrAssign: 	
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_BITWISE_OR_ASSIGN );
				break;
			case clang::BO_Comma:
				IncrementMetric( m_currentUnit, METRIC_TYPE_OPERATOR_COMMA );
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


bool MetricVisitor::VisitStmt(clang::Stmt *p_statement) 
{
	if( m_currentUnit )
	{
		IncrementMetric( m_currentUnit, METRIC_TYPE_STATEMENTS );
	}
	return true;
}


bool MetricVisitor::VisitIfStmt(clang::IfStmt *p_ifSt) 
{
#if defined( DEBUG_FN_TRACE_OUTOUT )
	std::cout << "VisitIfStmt : CONTEXT " << m_currentFileName << "::" << m_currentFunctionName << std::endl;
#endif

	if( m_currentUnit )
	{
#if defined( DEBUG_FN_TRACE_OUTOUT )
	std::cout << "VisitIfStmt - Recorded" << std::endl;
#endif
		m_currentUnit->set( METRIC_TYPE_NESTING_LEVEL, getControlDepth( p_ifSt, m_astContext ));
		IncrementMetric( m_currentUnit, METRIC_TYPE_IF );
		IncrementMetric( m_currentUnit, METRIC_TYPE_DECISIONS );

		if( p_ifSt->getElse() )
		{
			// TODO: This means that "else if" statements get counted as both an IF and an ELSE, which may not be what everyone wants
			IncrementMetric( m_currentUnit, METRIC_TYPE_ELSE );
		}
	} else {
		/* TODO */
	}

    return true;
}

#if 0
void MetricVisitor::dump( std::ostream& out, const bool p_output[ METRIC_UNIT_MAX ], const MetricDumpFormat_e p_fmt )
{
	m_topUnit->dump( out, p_output, p_fmt, m_options );
}
#endif

bool MetricVisitor::TraverseStmt(clang::Stmt *p_stmt)
{
	if( p_stmt )
	{
		clang::Stmt::StmtClass stmtKind = p_stmt->getStmtClass();

		switch( stmtKind )
		{
			default:
#if defined( DEBUG_FN_TRACE_OUTOUT )
				std::cout << "TraverseStmt - Unhandled stmt type " << stmtKind << "(" << p_stmt->getStmtClassName() <<  ")\r\n";
#endif
				break;
		}
	}

	return clang::RecursiveASTVisitor<MetricVisitor>::TraverseStmt( p_stmt );
}

bool MetricVisitor::TraverseDecl(clang::Decl *p_decl)
{
	clang::Decl::Kind declKind = p_decl->getKind();

	switch( declKind )
	{
		case clang::Decl::Kind::TranslationUnit:
		case clang::Decl::Kind::AccessSpec:
		case clang::Decl::Kind::Block:
		case clang::Decl::Kind::Captured:
		case clang::Decl::Kind::ClassScopeFunctionSpecialization:
		case clang::Decl::Kind::ClassTemplate:
		case clang::Decl::Kind::ClassTemplatePartialSpecialization:
		case clang::Decl::Kind::CXXConstructor:
		case clang::Decl::Kind::CXXConversion:
		case clang::Decl::Kind::CXXDestructor:
		case clang::Decl::Kind::CXXMethod:
		case clang::Decl::Kind::CXXRecord:
		case clang::Decl::Kind::Empty:
		case clang::Decl::Kind::Enum:
		case clang::Decl::Kind::EnumConstant:
		case clang::Decl::Kind::Field:
		case clang::Decl::Kind::FileScopeAsm:
		/* TODO: first* ?? */
		case clang::Decl::Kind::Friend:
		case clang::Decl::Kind::FriendTemplate:
		case clang::Decl::Kind::Function:
		case clang::Decl::Kind::FunctionTemplate:
		case clang::Decl::Kind::ImplicitParam:
		case clang::Decl::Kind::Import:
		case clang::Decl::Kind::IndirectField:
		case clang::Decl::Kind::Label:
		/* TODO: last* ?? */
		/* TODO: the rest of the types */
			break;
		default:
#if defined( DEBUG_FN_TRACE_OUTOUT )
			std::cout << "TraverseDecl - Unhandled decl type " << declKind << "(" << p_decl->getDeclKindName() << ")\r\n";
#endif
			break;
	}

	if( declKind == clang::Decl::TranslationUnit )
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
			if( ShouldIncludeFile( name ))
			{
				MetricUnit* fileUnit = m_topUnit->getSubUnit(name, METRIC_UNIT_FILE);
				fileUnit->set( METRIC_TYPE_BYTE_COUNT, (*x).first->getSize() );
			}
		}
	}
	
	bool retVal = clang::RecursiveASTVisitor<MetricVisitor>::TraverseDecl( p_decl );

	if( declKind == clang::Decl::TranslationUnit )
	{
		CloseOutFnOrMtd();
	}

	return retVal;
}

void MetricVisitor::IncrementMetric( MetricUnit* const p_unit, const MetricType_e p_metricType )
{
	MetricUnit* fileUnit = m_topUnit->getSubUnit(m_currentFileName, METRIC_UNIT_FILE, false);

	if( fileUnit )
	{
		IncrementMetric( p_unit, p_metricType, fileUnit );
	}
}

void MetricVisitor::IncrementMetric( MetricUnit* const p_unit, const MetricType_e p_metricType, const MetricUnit* const p_file )
{
	if( p_file )
	{
		if(( ! p_file->hasBeenProcessed( METRIC_UNIT_PROCESS_AST ) ) || ( MetricUnit::isMultiPassAllowed( p_metricType )))
		{
			p_unit->increment( p_metricType );
		}
		else
		{
#if defined( DEBUG_FN_TRACE_OUTOUT )
			std::cout << "Not incrementing metric " << MetricUnit::getMetricName( p_metricType ) << std::endl;
#endif
		}
	}
}

bool MetricVisitor::ShouldIncludeFile( const std::string& p_file )
{
	bool ret_val = false;
	if( SHOULD_INCLUDE_FILE( m_options, p_file ) )
	{
		ret_val = true;
	}
	return ( ret_val );
}
