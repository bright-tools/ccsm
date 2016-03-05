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
#include <llvm/Support/raw_os_ostream.h>

const std::pair<clang::Stmt::StmtClass, MetricType_e> keywordToMetricPairs[] = {
	std::make_pair(clang::Stmt::BreakStmtClass, METRIC_TYPE_BREAK),
};

const std::pair<clang::BinaryOperator::Opcode, MetricType_e> MetricVisitor::binaryOperatorToMetricPairs[] = {
	std::make_pair(clang::BO_PtrMemD, METRIC_TYPE_OPERATOR_PTR_TO_MEMBER_DIRECT),
	std::make_pair(clang::BO_PtrMemI, METRIC_TYPE_OPERATOR_PTR_TO_MEMBER_INDIRECT),
	std::make_pair(clang::BO_Mul, METRIC_TYPE_OPERATOR_ARITHMETIC_MULTIPLICATION),
	std::make_pair(clang::BO_Div, METRIC_TYPE_OPERATOR_ARITHMETIC_DIVISION),
	std::make_pair(clang::BO_Rem, METRIC_TYPE_OPERATOR_ARITHMETIC_MODULO),
	std::make_pair(clang::BO_Add, METRIC_TYPE_OPERATOR_ARITHMETIC_ADDITION),
	std::make_pair(clang::BO_Sub, METRIC_TYPE_OPERATOR_ARITHMETIC_SUBTRACTION),
	std::make_pair(clang::BO_Shl, METRIC_TYPE_OPERATOR_SHIFT_LEFT),
	std::make_pair(clang::BO_Shr, METRIC_TYPE_OPERATOR_SHIFT_RIGHT),
	std::make_pair(clang::BO_LT, METRIC_TYPE_OPERATOR_COMP_LESS_THAN),
	std::make_pair(clang::BO_GT, METRIC_TYPE_OPERATOR_COMP_GREATER_THAN),
	std::make_pair(clang::BO_LE, METRIC_TYPE_OPERATOR_COMP_LESS_THAN_EQUAL),
	std::make_pair(clang::BO_GE, METRIC_TYPE_OPERATOR_COMP_GREATER_THAN_EQUAL),
	std::make_pair(clang::BO_EQ, METRIC_TYPE_OPERATOR_COMP_EQUAL),
	std::make_pair(clang::BO_NE, METRIC_TYPE_OPERATOR_COMP_NOT_EQUAL),
	std::make_pair(clang::BO_And, METRIC_TYPE_OPERATOR_BITWISE_AND),
	std::make_pair(clang::BO_Xor, METRIC_TYPE_OPERATOR_BITWISE_XOR),
	std::make_pair(clang::BO_Or, METRIC_TYPE_OPERATOR_BITWISE_OR),
	std::make_pair(clang::BO_LAnd, METRIC_TYPE_OPERATOR_LOGICAL_AND),
	std::make_pair(clang::BO_LOr, METRIC_TYPE_OPERATOR_LOGICAL_OR),
	std::make_pair(clang::BO_Assign, METRIC_TYPE_OPERATOR_ARITHMETIC_ASSIGN),
	std::make_pair(clang::BO_MulAssign, METRIC_TYPE_OPERATOR_ARITHMETIC_MULTIPLICATION_ASSIGN),
	std::make_pair(clang::BO_DivAssign, METRIC_TYPE_OPERATOR_ARITHMETIC_DIVISION_ASSIGN),
	std::make_pair(clang::BO_RemAssign, METRIC_TYPE_OPERATOR_ARITHMETIC_MODULO_ASSIGN),
	std::make_pair(clang::BO_AddAssign, METRIC_TYPE_OPERATOR_ARITHMETIC_ADDITION_ASSIGN),
	std::make_pair(clang::BO_SubAssign, METRIC_TYPE_OPERATOR_ARITHMETIC_SUBTRACTION_ASSIGN),
	std::make_pair(clang::BO_ShlAssign, METRIC_TYPE_OPERATOR_SHIFT_LEFT_ASSIGN),
	std::make_pair(clang::BO_ShrAssign, METRIC_TYPE_OPERATOR_SHIFT_RIGHT_ASSIGN),
	std::make_pair(clang::BO_AndAssign, METRIC_TYPE_OPERATOR_BITWISE_AND_ASSIGN),
	std::make_pair(clang::BO_XorAssign, METRIC_TYPE_OPERATOR_BITWISE_XOR_ASSIGN),
	std::make_pair(clang::BO_OrAssign, METRIC_TYPE_OPERATOR_BITWISE_OR_ASSIGN),
	std::make_pair(clang::BO_Comma, METRIC_TYPE_OPERATOR_COMMA),
};

const std::map<clang::BinaryOperator::Opcode, MetricType_e> MetricVisitor::binaryOperatorToMetricMap(binaryOperatorToMetricPairs,
	binaryOperatorToMetricPairs + sizeof binaryOperatorToMetricPairs / sizeof binaryOperatorToMetricPairs[0]);


MetricVisitor::MetricVisitor(clang::CompilerInstance &p_CI, MetricUnit* p_topUnit, const MetricOptions& p_options, TranslationUnitFunctionLocator* p_fnLocator) : 
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

std::string MetricVisitor::getDefResolvedFileName( const clang::SourceLocation &loc )
{
	clang::SourceManager& sm =  m_astContext->getSourceManager();
	clang::SourceLocation sLoc = loc;

	if( sLoc.isMacroID() )
	{
		sLoc = m_astContext->getSourceManager().getFileLoc( sLoc );
	}

	std::string declFn = sm.getFilename( sLoc ).str();

#if defined( DEBUG_FN_TRACE_OUTOUT )
	std::cout << "getDefResolvedFileName - CONTEXT " << m_currentFileName << "::" << m_currentFunctionName << std::endl;
#endif

	while( m_options.isDefFile( declFn ) && sLoc.isValid() )
	{
		clang::FullSourceLoc sl = m_astContext->getFullLoc( sLoc );
		sLoc = sm.getIncludeLoc( sl.getFileID() );
		if( sLoc.isValid() )
		{
			declFn = sm.getFilename( sLoc ).str();
		}
	} 

	return declFn;
}

void MetricVisitor::UpdateCurrentFileName( const clang::SourceLocation &loc )
{
	m_currentFileName = getDefResolvedFileName( loc );
}

MetricVisitor::PathResults MetricVisitor::getSwitchPathCount(const clang::SwitchStmt* const p_stmt, uint16_t depth)
{
	MetricVisitor::PathResults pathSum = { 0, false };
	// If the switch is based on an enum and all enum values are covered then effectively there should be no need for
	//  a default
	// TODO: Not necessarily true - just because an enum is being used it does not stop other values
	//  being assigned to the variable
	bool hasDefault = p_stmt->isAllEnumCasesCovered();
	const clang::SwitchCase* switchCase = p_stmt->getSwitchCaseList();

#if defined( DEBUG_FN_TRACE_OUTOUT )
	std::string blanks(depth, ' ');
#endif

	if (switchCase)
	{
		bool firstSubst = true;
		do
		{
			const clang::Stmt* subStmt = switchCase->getSubStmt();

#if defined( DEBUG_FN_TRACE_OUTOUT )
			std::cout << blanks << "getSwitchPathCount - Case is " << subStmt->getStmtClassName() << std::endl;
#endif

			/* Check that it's not a fall-through */
			if ((subStmt->getStmtClass() != clang::Stmt::CaseStmtClass) &&
				(subStmt->getStmtClass() != clang::Stmt::DefaultStmtClass))
			{
				/* Check there actually is a substatement */
				if (subStmt != nullptr)
				{
					PathResults sub_count = getPathCount(subStmt, depth);
					pathSum.path_count += sub_count.path_count;
					pathSum.path_has_return = (pathSum.path_has_return || firstSubst) && sub_count.path_has_return;
					firstSubst = false;
				}
			}
			else
			{
#if defined( DEBUG_FN_TRACE_OUTOUT )
				std::cout << blanks << "getSwitchPathCount - Fallthrough" << std::endl;
#endif
			}

			/* This case a 'default' statement? */
			if (switchCase->getStmtClass() == clang::Stmt::DefaultStmtClass)
			{
				hasDefault = true;
			}

			switchCase = switchCase->getNextSwitchCase();

#if defined( DEBUG_FN_TRACE_OUTOUT )
			std::cout << blanks << "getSwitchPathCount - Updated: count - " << pathSum.path_count << " return - " << pathSum.path_has_return << std::endl;
#endif

		} while (switchCase != nullptr);
	}

	/* If there wasn't an explicit default in the set of cases, add an implicit one */
	if (!hasDefault)
	{
#if defined( DEBUG_FN_TRACE_OUTOUT )
		std::cout << blanks << "getSwitchPathCount - Adding implicit default" << std::endl;
#endif
		pathSum.path_count++;
		pathSum.path_has_return = false;
	}

#if defined( DEBUG_FN_TRACE_OUTOUT )
	std::cout << blanks << "getSwitchPathCount - Done: count - " << pathSum.path_count << " return - " << pathSum.path_has_return << std::endl;
#endif
	
	return pathSum;
}

MetricVisitor::PathResults MetricVisitor::getIfPathCount(const clang::IfStmt* const p_stmt, uint16_t depth)
{
	MetricVisitor::PathResults ret_val;
	MetricUnit::counter_t pathSum = 0;
	bool has_return = false;
	const clang::IfStmt* ifStmt = p_stmt;

#if defined( DEBUG_FN_TRACE_OUTOUT )
	std::string blanks(depth, ' ');
#endif

	if (ifStmt != nullptr)
	{
		has_return = true;

		/* Loops across 'if', 'else if' and 'else' constructs */
		while (ifStmt != nullptr)
		{
			MetricVisitor::PathResults thenCount = { 1, false };
			/* Default to one gives an count for an implicit else block in the case that there is not one in the code */
			MetricVisitor::PathResults elseCount = { 1, false };

			/* Get the path count for the 'then' block */
			thenCount = getPathCount(ifStmt->getThen(), depth);

			/* Is there an else block? */
			if (ifStmt->getElse() != nullptr)
			{
#if defined( DEBUG_FN_TRACE_OUTOUT )
				std::cout << blanks << "getIfPathCount - Else is " << ifStmt->getElse()->getStmtClassName() << std::endl;
#endif

				/* Is it an 'else if'? */
				if (ifStmt->getElse()->getStmtClass() == clang::Stmt::IfStmtClass)
				{
					/* Process the contents of the 'else if' next time round the while */
					ifStmt = static_cast<const clang::IfStmt*>(ifStmt->getElse());
					/* No need to count an implicit else */
					elseCount.path_count = 0;
				}
				else
				{
					/* Not an 'else if', just an 'else', so count that and no more while loop iteration required */
					elseCount = getPathCount(ifStmt->getElse(), depth);
					ifStmt = nullptr;
				}
			}
			else
			{
				ifStmt = nullptr;
			}

			pathSum += (thenCount.path_count + elseCount.path_count);
			has_return = has_return && (thenCount.path_has_return && ((elseCount.path_count == 0) || elseCount.path_has_return));
		}
	}
	ret_val.path_count = pathSum;
	ret_val.path_has_return = has_return;

#if defined( DEBUG_FN_TRACE_OUTOUT )
	std::cout << blanks << "getIfPathCount - Updated: count - " << ret_val.path_count << " return - " << ret_val.path_has_return << std::endl;
#endif

	return ret_val;
}

/* This function determines the number of unique paths through the specified code.  It also determines whether each and every
   path leads to a return keyword or not.
*/
MetricVisitor::PathResults MetricVisitor::getPathCount(const clang::Stmt* const p_stmt, uint16_t depth)
{
	PathResults ret_val;
	// TODO: Return this for null, too?
	ret_val.path_has_return = false;
	ret_val.path_count = 1;
	// TODO: Need to check that there are no GOTOs before doing the path count

#if defined( DEBUG_FN_TRACE_OUTOUT )
	std::string blanks(depth, ' ');
	std::cout << blanks << "getPathCount - CONTEXT " << m_currentFileName << "::" << m_currentFunctionName << std::endl;
	std::cout << blanks << "getPathCount - " << p_stmt->getStmtClassName() << std::endl;
#endif

	if (p_stmt != nullptr)
	{
		const uint16_t thisDepth = depth + 1;

		switch (p_stmt->getStmtClass())
		{
			case clang::Stmt::StmtClass::IfStmtClass:
				ret_val = getIfPathCount(static_cast<const clang::IfStmt*>(p_stmt), thisDepth);
				break;
			case clang::Stmt::StmtClass::SwitchStmtClass:
				ret_val = getSwitchPathCount(static_cast<const clang::SwitchStmt*>(p_stmt), thisDepth);
				break;
			case clang::Stmt::StmtClass::WhileStmtClass:
				ret_val = getPathCount((static_cast<const clang::WhileStmt*>(p_stmt)->getBody()), thisDepth);
				ret_val.path_count += 1;
				break;
			case clang::Stmt::StmtClass::ForStmtClass:
				ret_val = getPathCount((static_cast<const clang::ForStmt*>(p_stmt)->getBody()), thisDepth);
				ret_val.path_count += 1;
				break;
			case clang::Stmt::StmtClass::DoStmtClass:
				ret_val = getPathCount((static_cast<const clang::DoStmt*>(p_stmt)->getBody()), thisDepth);
				ret_val.path_count += 1;
				break;
			default:
				ret_val = getOtherPathCount(p_stmt, thisDepth);
				break;
		}

	}

#if defined( DEBUG_FN_TRACE_OUTOUT )
	std::cout << blanks << "getPathCount - path_count " << ret_val.path_count << " has_return " << ret_val.path_has_return << std::endl;
#endif

	return ret_val;
}

MetricVisitor::PathResults MetricVisitor::getOtherPathCount(const clang::Stmt* const p_stmt, uint16_t depth)
{
	PathResults ret_val;
	ret_val.path_has_return = false;
	ret_val.path_count = 1;
	// TODO: Need to check that there are no GOTOs before doing the path count

#if defined( DEBUG_FN_TRACE_OUTOUT )
	std::string blanks(depth, ' ');
#endif

	if (p_stmt->getStmtClass() == clang::Stmt::StmtClass::ReturnStmtClass)
	{
		ret_val.path_has_return = true;
		/* No need to process any following statements, as they're inaccessible 
		   TODO: unless there's a labelled statement that can receive a goto jump */
		IncrementMetric(m_currentUnit, METRIC_TYPE_RETURNPOINTS);
#if defined( DEBUG_FN_TRACE_OUTOUT )
		std::cout << blanks << "getOtherPathCount - Return point found" << std::endl;
#endif
	}
	else
	{
		bool skipAllSubsequent = false;
		bool pathMissingReturn = false;
		// Iterate all child statements
		for (clang::Stmt::const_child_iterator it = p_stmt->child_begin();
			it != p_stmt->child_end();
			it++)
		{
			clang::Stmt::StmtClass clss = (*it)->getStmtClass();
			PathResults sub_results;

#if defined( DEBUG_FN_TRACE_OUTOUT )
			std::cout << blanks << "getOtherPathCount - processing " << (*it)->getStmtClassName() << std::endl;
#endif
			if (skipAllSubsequent)
			{
				if (clss == clang::Stmt::LabelStmtClass)
				{
					skipAllSubsequent = false;
				}
#if defined( DEBUG_FN_TRACE_OUTOUT )
				std::cout << blanks << "getOtherPathCount - skipping " << std::endl;
#endif
			}
			else 
			{
				// For statement classes which have an effect on the path
				switch (clss)
				{
				case clang::Stmt::StmtClass::GotoStmtClass:
				case clang::Stmt::StmtClass::BreakStmtClass:
				case clang::Stmt::StmtClass::ContinueStmtClass:
					/* Subsequent statements are inaccessible unless there's a label 
					   TODO: what about if the label is within a sub-tree? */
					skipAllSubsequent = true;
					break;
				case clang::Stmt::StmtClass::ReturnStmtClass:
					ret_val.path_has_return = true;
					skipAllSubsequent = true;
					IncrementMetric(m_currentUnit, METRIC_TYPE_RETURNPOINTS);
					pathMissingReturn = false;

#if defined( DEBUG_FN_TRACE_OUTOUT )
					std::cout << blanks << "getOtherPathCount - Return point found" << std::endl;
#endif

					break;
				case clang::Stmt::StmtClass::IfStmtClass:
				case clang::Stmt::StmtClass::SwitchStmtClass:
				case clang::Stmt::StmtClass::CompoundStmtClass:
				case clang::Stmt::StmtClass::WhileStmtClass:
				case clang::Stmt::StmtClass::DoStmtClass:
				case clang::Stmt::StmtClass::ForStmtClass:
					sub_results = getPathCount(*it, depth);
					if (sub_results.path_has_return)
					{
#if defined( DEBUG_FN_TRACE_OUTOUT )
						std::cout << blanks << "getOtherPathCount - Path has a return statement" << std::endl;
#endif
						ret_val.path_has_return = true;
						/* All paths had a return statement ... from now on, code should not be reachable */
						skipAllSubsequent = true;
					}
					else
					{
#if defined( DEBUG_FN_TRACE_OUTOUT )
						std::cout << blanks << "getOtherPathCount - Path is missing a return statement" << std::endl;
#endif
						pathMissingReturn = true;
					}
					ret_val.path_count *= sub_results.path_count;
					break;
				default:
					break;
				}
#if defined( DEBUG_FN_TRACE_OUTOUT )
				std::cout << blanks << "getOtherPathCount - Updated: count - " << ret_val.path_count << " return - " << ret_val.path_has_return << std::endl;
#endif
			}
		}
		if (pathMissingReturn)
		{
			ret_val.path_has_return = false;
		}
	}
#if defined( DEBUG_FN_TRACE_OUTOUT )
	std::cout << blanks << "getOtherPathCount - Done: count - " << ret_val.path_count << " return - " << ret_val.path_has_return << std::endl;
#endif

	return ret_val;
}

bool MetricVisitor::VisitFunctionDecl(clang::FunctionDecl *func) {
    
#if defined( DEBUG_FN_TRACE_OUTOUT )
	std::cout << "VisitFunctionDecl - CONTEXT " << m_currentFileName << std::endl;
#endif

	/* Deal with common actions which may be applicable to a decl valid at translation-unit level */
	DeclCommon(func->getLexicalParent(), func);

	/* Function body attached? */
	if( func->doesThisDeclarationHaveABody() )
	{		
		clang::SourceLocation funcLoc = func->getLocation();

		UpdateCurrentFileName( funcLoc );
		
		m_currentFunctionName = func->getQualifiedNameAsString();

#if defined( DEBUG_FN_TRACE_OUTOUT )
		std::cout << "VisitFunctionDecl - CONTEXT Fn: " << m_currentFunctionName << std::endl;
#endif

		if( m_fnLocator != NULL )
		{
			m_fnLocator->addFunctionLocation( m_astContext, m_currentFunctionName, func );
		}

		if( ShouldIncludeFile( m_currentFileName ) && 
			m_options.ShouldIncludeFunction( m_currentFunctionName ))
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
			clang::StringRef buffer(start, end - start);
			m_currentUnit->set( METRIC_TYPE_FUNCTION_LINE_COUNT, countNewlines( buffer ) );

			if( func->isInlineSpecified() )
			{
				IncrementMetric( fileUnit, METRIC_TYPE_INLINE_FUNCTIONS );
			}

			PathResults pathResults = getPathCount(func->getBody());

			m_currentUnit->set(METRIC_TYPE_FUNCTION_PATHS, pathResults.path_count );

			if (!pathResults.path_has_return)
			{
				/* Add an implicit return point */
				m_currentUnit->increment(METRIC_TYPE_RETURNPOINTS, 1);
			}

			switch( func->getLinkageAndVisibility().getLinkage() )
			{
				case clang::Linkage::InternalLinkage:
					IncrementMetric( fileUnit, METRIC_TYPE_LOCAL_FUNCTIONS );
					break;
				case clang::Linkage::ExternalLinkage:
					m_currentUnit->setExternalLinkage();
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
		if( m_currentUnit )
		{
			switch( func->getStorageClass() )
			{
				case clang::SC_None:
					/* No storage class specified - implicitly the function is extern */
					IncrementMetric( m_currentUnit, METRIC_TYPE_EXTERN_IMPL_FUNCTIONS );
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

#undef DEBUG_FN_TRACE_OUTOUT

bool MetricVisitor::VisitEnumDecl(clang::EnumDecl* p_enumDecl)
{
	return true;
}

bool MetricVisitor::VisitRecordDecl(clang::RecordDecl* p_recordDecl)
{
	return true;
}

bool MetricVisitor::VisitTypedefNameDecl(const clang::TypedefNameDecl *p_typeDef)
{
#if defined( DEBUG_FN_TRACE_OUTOUT )
	std::cout << "VisitTypedefNameDecl - CONTEXT " << m_currentFileName << "::" << m_currentFunctionName << std::endl;
#endif

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

void MetricVisitor::DeclCommon(const clang::DeclContext* p_declCtxt, const clang::Decl* p_decl)
{
#if defined(DEBUG_FN_TRACE_OUTOUT)
	std::cout << "DeclCommon - CONTEXT " << m_currentFileName << "::" << m_currentFunctionName << std::endl;
#endif

	// Check to see if the decl is top-level - may need to update m_currentUnit after exiting a function.
	if (p_declCtxt->getDeclKind() == clang::Decl::TranslationUnit)
	{
		CloseOutFnOrMtd();

		HandleLoc(p_decl->getLocation());
	}
}

void MetricVisitor::HandleLoc( const clang::SourceLocation& p_loc)
{
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

bool MetricVisitor::VisitCastExpr(clang::CastExpr *p_castExp)
{
	return true;
}

bool MetricVisitor::VisitVarDecl(clang::VarDecl *p_varDec) {

#if defined( DEBUG_FN_TRACE_OUTOUT )
	std::cout << "VisitVarDecl : CONTEXT " << m_currentFileName << "::" << m_currentFunctionName << std::endl;
	std::cout << "VisitVarDecl : Processing " << p_varDec->getNameAsString() << std::endl;
#endif

	/* Deal with common actions which may be applicable to a decl valid at translation-unit level */
	DeclCommon(p_varDec->getLexicalDeclContext(), p_varDec);

	if( m_currentUnit )
	{
		/* Interested in the absolute qualifiers associated with the variable */
		const bool isVolatile = p_varDec->getType().isVolatileQualified();
		const bool isConst = p_varDec->getType().isConstQualified();

		/* Check it's not a function parameter */
		if (p_varDec->getKind() == clang::Decl::Var)
		{
			// TODO: This is the declared storage class rather than the computed storage class.  There may be a difference - which version should the metrics reflect?
			clang::StorageClass sc = p_varDec->getStorageClass();

			/* Check to see if this variable is file scope */
			if (p_varDec->isFileVarDecl())
			{
#if defined( DEBUG_FN_TRACE_OUTOUT )
				std::cout << "VisitVarDecl : Processing file-scope var " << p_varDec->getNameAsString() << std::endl;
#endif
				if (sc == clang::SC_Extern)
				{
					IncrementMetric(m_currentUnit, METRIC_TYPE_VARIABLE_FILE_EXTERN);
				}
				else
				{
					IncrementMetric(m_currentUnit, METRIC_TYPE_VARIABLE_FILE_LOCAL);
					if (isVolatile)
					{
						IncrementMetric(m_currentUnit, METRIC_TYPE_VARIABLE_FILE_VOLATILE);
					}
					if (isConst)
					{
						IncrementMetric(m_currentUnit, METRIC_TYPE_VARIABLE_FILE_CONST);
					}
					switch (sc)
					{
					case clang::SC_Static:
						IncrementMetric(m_currentUnit, METRIC_TYPE_VARIABLE_FILE_STATIC);
						break;
					default:
						/* Not currently interested */
						break;
					}
				}
			}
			else
			{
				if (sc == clang::SC_Extern)
				{
					IncrementMetric(m_currentUnit, METRIC_TYPE_VARIABLE_FN_EXTERN);
				}
				else
				{
					IncrementMetric(m_currentUnit, METRIC_TYPE_VARIABLE_FN_LOCAL);
					if (isVolatile)
					{
						IncrementMetric(m_currentUnit, METRIC_TYPE_VARIABLE_FN_VOLATILE);
					}
					if (isConst)
					{
						IncrementMetric(m_currentUnit, METRIC_TYPE_VARIABLE_FN_CONST);
					}
					switch (sc)
					{
					case clang::SC_Static:
						IncrementMetric(m_currentUnit, METRIC_TYPE_VARIABLE_FN_STATIC);
						break;
					case clang::SC_Register:
						IncrementMetric(m_currentUnit, METRIC_TYPE_VARIABLE_FN_REGISTER);
						break;
					case clang::SC_Auto:
						IncrementMetric(m_currentUnit, METRIC_TYPE_VARIABLE_FN_AUTO);
						break;
					default:
						/* Not currently of interest */
						break;
					}
				}
			}

			/* If the decl has an initialiser then it's a statement */
			if (p_varDec->getAnyInitializer() != NULL)
			{
				IncrementMetric(m_currentUnit, METRIC_TYPE_STATEMENTS);
				if (!p_varDec->getLocStart().isMacroID())
				{
					m_currentUnit->increment(METRIC_TYPE_TOKEN_STATEMENTS);
				}
			}

		} else if (p_varDec->getKind() == clang::Decl::ParmVar)
		{
			MetricUnit* owner = m_currentUnit;

			/* Is this a function parameter which should be attributed to file scope? */
			if (m_options.getPrototypesAreFileScope())
			{
				owner = m_topUnit->getSubUnit(m_currentFileName, METRIC_UNIT_FILE);
			}

			IncrementMetric(owner, METRIC_TYPE_FUNCTION_PARAMETERS);
		}
		else
		{
		}
	}

	return true;	
}

bool MetricVisitor::VisitCompoundStmt(clang::CompoundStmt *p_compoundSt)
{
	if (m_currentUnit)
	{
		CountStatements(p_compoundSt->children());
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
		m_currentUnit->setMax( METRIC_TYPE_NESTING_LEVEL, getControlDepth( p_forSt, m_astContext ));
		IncrementMetric(m_currentUnit, METRIC_TYPE_LOOPS);

		CountStatements(p_forSt->getBody());
	}
    return true;
}

bool MetricVisitor::VisitGotoStmt(clang::GotoStmt *p_gotoSt) 
{
	if( m_currentUnit )
	{
	}

    return true;
}

bool MetricVisitor::VisitLabelStmt(clang::LabelStmt *p_LabelSt) 
{
	if( m_currentUnit )
	{
		IncrementMetric(m_currentUnit, METRIC_TYPE_LABEL_NAME );
	}
    return true;
}

bool MetricVisitor::VisitWhileStmt(clang::WhileStmt *p_whileSt) 
{
#if defined( DEBUG_FN_TRACE_OUTOUT )
	std::cout << "VisitWhileStmt - CONTEXT " << m_currentFileName << "::" << m_currentFunctionName << std::endl;
#endif
	if (m_currentUnit)
	{
		m_currentUnit->setMax( METRIC_TYPE_NESTING_LEVEL, getControlDepth( p_whileSt, m_astContext ));
		IncrementMetric( m_currentUnit, METRIC_TYPE_LOOPS );

		CountStatements(p_whileSt->getBody());
	}
    return true;
}

bool MetricVisitor::VisitDoStmt(clang::DoStmt *p_doSt)
{
#if defined( DEBUG_FN_TRACE_OUTOUT )
	std::cout << "VisitDoStmt - CONTEXT " << m_currentFileName << "::" << m_currentFunctionName << std::endl;
#endif
	if (m_currentUnit)
	{
		m_currentUnit->setMax(METRIC_TYPE_NESTING_LEVEL, getControlDepth(p_doSt, m_astContext));
		IncrementMetric(m_currentUnit, METRIC_TYPE_LOOPS);

		CountStatements(p_doSt->getBody());
	}
	return true;
}

/* TODO: Remove this */
bool MetricVisitor::VisitReturnStmt(clang::ReturnStmt *p_returnSt) 
{
	if( m_currentUnit )
	{
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
		clang::Decl* calleeDecl = p_callExpr->getCalleeDecl();
		if (calleeDecl)
		{
			clang::FunctionDecl* p_calleeFn = calleeDecl->getAsFunction();

			/* Check the function could be determined - probably not in the case of a function pointer */
			if (p_calleeFn != NULL)
			{
				std::string calleeName = p_calleeFn->getQualifiedNameAsString();

#if defined( DEBUG_FN_TRACE_OUTOUT )
				std::cout << "VisitCallExpr : Function call to " << calleeName << std::endl;
#endif

				/* Not registered this function as being called yet? */
				// TODO: Does this work for C++ namespacing?
				if (m_fnsCalled.find(calleeName) == m_fnsCalled.end()) {

					// Update the set containing the names of all the functions called
					m_fnsCalled.insert(calleeName);

#if defined( DEBUG_FN_TRACE_OUTOUT )
					std::cout << "VisitCallExpr : Not yet registered as called" << std::endl;
#endif

					/* This look-up will only find functions where the body is visible from to the current TU */
					clang::Stmt* calleeBody = p_callExpr->getDirectCallee()->getBody();

					if (calleeBody)
					{
						/* Called function is visible from the current position */

						clang::SourceLocation calleeBodyLocation = calleeBody->getLocStart();
						std::string name = getDefResolvedFileName(calleeBodyLocation);

#if defined( DEBUG_FN_TRACE_OUTOUT )
						std::cout << "VisitCallExpr : Found function body in " << name << std::endl;
#endif

						if (ShouldIncludeFile(name))
						{
							if (m_options.ShouldIncludeFunction( calleeName))
							{
								MetricUnit* fileUnit = m_topUnit->getSubUnit(name, METRIC_UNIT_FILE);
								MetricUnit* targFn = fileUnit->getSubUnit(calleeName, METRIC_UNIT_FUNCTION);
								IncrementMetric(targFn, METRIC_TYPE_CALLED_BY_LOCAL, fileUnit);
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
						/* Function body not visible from the current position */

						/* Add function to list of those which were not resolvable */
						m_currentUnit->addUnresolvedFn(calleeName);
					}
				}
				else
				{
#if defined( DEBUG_FN_TRACE_OUTOUT )
					std::cout << "VisitCallExpr : Already registered as called" << std::endl;
#endif
				}

				if (p_calleeFn->getBody() != NULL)
				{
					IncrementMetric(m_currentUnit, METRIC_TYPE_LOCAL_FUNCTION_CALLS);
				}
			}
			else
			{
#if defined( DEBUG_FN_TRACE_OUTOUT )
				std::cout << "VisitCallExpr : Not able to determin name of called function" << std::endl;
#endif
				// TODO: Number of functions called isn't incremented, which might be misleading ... should document this behaviour at least
			}
		}
		else
		{
#if defined( DEBUG_FN_TRACE_OUTOUT )
			std::cout << "VisitCallExpr : Not able to get decl for function call" << std::endl;
#endif
			/* Decl not available! */
		}
		IncrementMetric(m_currentUnit, METRIC_TYPE_FUNCTION_CALLS);
	}

    return true;
}

bool MetricVisitor::VisitSwitchStmt(clang::SwitchStmt *p_switchSt) 
{
	if( m_currentUnit )
	{
		m_currentUnit->setMax( METRIC_TYPE_NESTING_LEVEL, getControlDepth( p_switchSt, m_astContext ));
		IncrementMetric( m_currentUnit, METRIC_TYPE_DECISIONS );

		CountStatements(p_switchSt->getBody());
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

bool MetricVisitor::VisitContinueStmt(clang::ContinueStmt *p_continueSt)
{
	if (m_currentUnit)
	{
	}
	return true;
}

bool MetricVisitor::VisitDefaultStmt(clang::DefaultStmt *p_defaultSt) 
{
	if( m_currentUnit )
	{
		CountStatements(p_defaultSt->getSubStmt());
	}
    return true;
}

bool MetricVisitor::VisitCaseStmt(clang::CaseStmt *p_caseSt) 
{
	if( m_currentUnit )
	{
		CountStatements(p_caseSt->getSubStmt());
	}
    return true;
}

bool MetricVisitor::VisitUnaryExprOrTypeTraitExpr( clang::UnaryExprOrTypeTraitExpr* p_unaryExpr )
{
	if( m_currentUnit )
	{
		switch( p_unaryExpr->getKind() )
		{
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
		std::map<clang::BinaryOperator::Opcode, MetricType_e>::const_iterator it = binaryOperatorToMetricMap.find(p_binOp->getOpcode());
		if (it != binaryOperatorToMetricMap.end())
		{
			IncrementMetric(m_currentUnit, it->second);
		}
		else
		{
#if defined( DEBUG_FN_TRACE_OUTOUT )
			std::cout << "VisitBinaryOperator - Unhandled operator" << std::endl;
#endif
		}
	}
    return true;
}


bool MetricVisitor::VisitStmt(clang::Stmt *p_statement) 
{
	if (m_currentUnit)
	{
		switch (p_statement->getStmtClass())
		{
		case clang::Stmt::WhileStmtClass:
			default:
				break;
		}

		/* All of these statement classes count towards the METRIC_TYPE_STATEMENTS metric */
		switch (p_statement->getStmtClass())
		{
			case clang::Stmt::WhileStmtClass:
			case clang::Stmt::DoStmtClass:
			case clang::Stmt::ForStmtClass:
			case clang::Stmt::IfStmtClass:
			case clang::Stmt::SwitchStmtClass:
			case clang::Stmt::CaseStmtClass:
			case clang::Stmt::DefaultStmtClass:
			case clang::Stmt::GotoStmtClass:
			case clang::Stmt::ContinueStmtClass:
			case clang::Stmt::BreakStmtClass:
			case clang::Stmt::ReturnStmtClass:
				IncrementMetric(m_currentUnit, METRIC_TYPE_STATEMENTS);
				if (! p_statement->getLocStart().isMacroID())
				{
					m_currentUnit->increment(METRIC_TYPE_TOKEN_STATEMENTS);
				}
				break;
			default:
				break;
		}
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
		m_currentUnit->setMax( METRIC_TYPE_NESTING_LEVEL, getControlDepth( p_ifSt, m_astContext ));
		IncrementMetric( m_currentUnit, METRIC_TYPE_DECISIONS );

		CountStatements(p_ifSt->getThen());

		if( p_ifSt->getElse() )
		{
			CountStatements(p_ifSt->getElse());
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
		if (m_options.getDumpAST())
		{
			llvm::raw_os_ostream out(m_options.getOutput());
			p_decl->dump(out);			
		}
	}
	
	bool retVal = clang::RecursiveASTVisitor<MetricVisitor>::TraverseDecl( p_decl );

	if( declKind == clang::Decl::TranslationUnit )
	{
		CloseOutFnOrMtd();
	}

	return retVal;
}

void MetricVisitor::CountStatements(const clang::Stmt* const p_stmt)
{
	if (p_stmt != NULL)
	{
		switch (p_stmt->getStmtClass())
		{
			case clang::Stmt::NullStmtClass:
			case clang::Stmt::DeclStmtClass:
				/* These certainly don't count as statements */
				break;
			case clang::Stmt::CompoundStmtClass:
			case clang::Stmt::LabelStmtClass:
				/* These aren't usefully counted as statements */
				break;
			case clang::Stmt::WhileStmtClass:
			case clang::Stmt::DoStmtClass:
			case clang::Stmt::ForStmtClass:
			case clang::Stmt::IfStmtClass:
			case clang::Stmt::SwitchStmtClass:
			case clang::Stmt::CaseStmtClass:
			case clang::Stmt::DefaultStmtClass:
			case clang::Stmt::GotoStmtClass:
			case clang::Stmt::ContinueStmtClass:
			case clang::Stmt::BreakStmtClass:
			case clang::Stmt::ReturnStmtClass:
				/* These are handled in VisitStmt() */
				break;

			default:
				IncrementMetric(m_currentUnit, METRIC_TYPE_STATEMENTS);
				if (!p_stmt->getLocStart().isMacroID())
				{
					m_currentUnit->increment(METRIC_TYPE_TOKEN_STATEMENTS);
				}
				break;
		}
	}
}

void MetricVisitor::CountStatements(const clang::Stmt::const_child_range& p_children)
{
	for (clang::Stmt::const_child_iterator cit = begin(p_children);
		cit != end(p_children);
		cit++)
	{
		if (*cit != NULL)
		{
			CountStatements(*cit);
		}
	}
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
			std::cout << "File has been processed - not incrementing metric " << MetricUnit::getMetricName( p_metricType ) << std::endl;
#endif
		}
	}
	else
	{
#if defined( DEBUG_FN_TRACE_OUTOUT )
		std::cout << "File not specified - not incrementing metric " << MetricUnit::getMetricName(p_metricType) << std::endl;
#endif
	}
}

bool MetricVisitor::ShouldIncludeFile( const std::string& p_file )
{
	// TODO: Ensure this function is sane with regard to different directory separator styles - \ vs /
	bool ret_val = false;
	if( m_options.ShouldIncludeFile( p_file ) )
	{
		ret_val = true;
	}
	return ( ret_val );
}
