/*
   @file
   @brief Class to support AST matching in order to gather
          code metrics.

   @author John Bailey
   @copyright Copyright 2014 John Bailey

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
#if !defined( METRIC_MATCHER_HPP )
#define       METRIC_MATCHER_HPP

#include "MetricUnit.hpp"
#include "MetricOptions.hpp"
#include "MetricUtils.hpp"

#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/AST/ASTContext.h"

#include <ostream>
#include <string>
#include <set>

class MetricVisitor : public clang::RecursiveASTVisitor<MetricVisitor>
{
protected:
	clang::CompilerInstance& m_compilerInstance;
	clang::ASTContext*		 m_astContext;
	MetricUnit*		         m_topUnit;
	std::string				 m_currentFileName;
	std::string				 m_currentFunctionName;
	MetricUnit*				 m_currentUnit;
	MetricOptions*			 m_options;
	std::set<std::string>	 m_fnsCalled;          
	SrcStartToFunctionMap_t* m_fnMap;

	void HandleLoc( clang::SourceLocation& p_loc );
	void DeclCommon( const clang::DeclContext* p_declCtxt, const clang::Decl* p_decl );
	void MetricVisitor::CloseOutFnOrMtd( void );
	bool ShouldIncludeFile( const std::string& p_file );

public:
	
	explicit MetricVisitor(clang::CompilerInstance &p_CI, MetricUnit* p_topUnit,MetricOptions* p_options = NULL,SrcStartToFunctionMap_t* p_fnMap = NULL);
	virtual ~MetricVisitor(void);
	virtual bool VisitFunctionDecl(clang::FunctionDecl *func);
	virtual bool VisitVarDecl(clang::VarDecl *p_varDec);
	virtual bool VisitForStmt(clang::ForStmt *p_forSt);
	virtual bool VisitGotoStmt(clang::GotoStmt *p_gotoSt);
	virtual bool VisitLabelStmt(clang::LabelStmt *p_LabelSt);
	virtual bool VisitWhileStmt(clang::WhileStmt *p_whileSt);
	virtual bool VisitReturnStmt(clang::ReturnStmt *p_returnSt);
	virtual bool VisitSwitchStmt(clang::SwitchStmt *p_switchSt);
	virtual bool VisitConditionalOperator(clang::ConditionalOperator *p_condOp);
	virtual bool VisitDefaultStmt(clang::DefaultStmt *p_defaultSt);
	virtual bool VisitCaseStmt(clang::CaseStmt *p_caseSt);
	virtual bool VisitBinaryOperator(clang::BinaryOperator *p_binOp);
	virtual bool VisitUnaryOperator(clang::UnaryOperator *p_uOp);
	virtual bool VisitStmt(clang::Stmt *p_statement);
	virtual bool VisitIfStmt(clang::IfStmt *p_ifSt);
	virtual bool VisitExplicitCastExpr(clang::ExplicitCastExpr *p_castExpr);
	virtual bool VisitCallExpr(clang::CallExpr *p_callExpr);
	virtual bool VisitArraySubscriptExpr (clang::ArraySubscriptExpr *p_subs);
	virtual bool VisitMemberExpr( clang::MemberExpr* p_memberExpr );
	virtual bool VisitUnaryExprOrTypeTraitExpr( clang::UnaryExprOrTypeTraitExpr* p_unaryExpr );
	virtual bool VisitTypedefDecl( clang::TypedefDecl* p_typeDef );
	virtual bool TraverseDecl(clang::Decl *p_decl);
	virtual bool TraverseStmt(clang::Stmt *p_stmt);

	const SrcStartToFunctionMap_t* getFunctionMap( void ) const;

#if 0
	void dump( std::ostream& out, const bool p_output[ METRIC_UNIT_MAX ], const MetricDumpFormat_e p_fmt = METRIC_DUMP_FORMAT_TREE );
#endif
};

#endif
