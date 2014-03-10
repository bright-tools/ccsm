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

#include "clang/Tooling/Tooling.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/CommentVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"

#include "MetricUtils.hpp"

#include <ostream>
#include <iostream>
#include <string>
#include <vector>

#include "MetricUnit.hpp"

class MetricVisitor : public clang::RecursiveASTVisitor<MetricVisitor>, clang::comments::ConstCommentVisitor<MetricVisitor>
{
protected:
	clang::ASTContext *astContext;
	MetricUnit*		  m_topUnit;
	std::string       m_currentFileName;
	std::string       m_currentFunctionName;
	MetricUnit*       m_currentUnit;

	bool ShouldIncludeFile( const std::string& p_fn ) const;
	bool ShouldIncludeFunction( const std::string& p_fn ) const;

public:
	typedef struct
	{
		std::vector<std::string>* ExcludeFiles;
		std::vector<std::string>* ExcludeFunctions;
	} Options;
	
	explicit MetricVisitor(clang::ASTContext* p_Context, MetricUnit* p_topUnit,Options* p_options = NULL);
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
	virtual bool VisitStmt(clang::Stmt *p_statement);
	virtual bool VisitIfStmt(clang::IfStmt *p_ifSt);
	virtual bool VisitCallExpr(clang::CallExpr *p_callExpr);
	void HandleComment(const std::string& p_fn,const std::string& p_comment );

	virtual bool TraverseDecl(clang::Decl *p_decl);

	void dump( std::ostream& out, const bool p_output[ METRIC_UNIT_MAX ], const MetricDumpFormat_e p_fmt = METRIC_DUMP_FORMAT_TREE );

private:
	Options* m_options;
};

class MetricASTConsumer : public clang::ASTConsumer, public clang::CommentHandler
{
protected:
	MetricVisitor *visitor;

public:
    explicit MetricASTConsumer(clang::ASTContext *CI, MetricUnit* p_topUnit, MetricVisitor::Options* p_options = NULL);

	virtual ~MetricASTConsumer(void);
	virtual void HandleTranslationUnit(clang::ASTContext &Context);
	virtual bool HandleComment(clang::Preprocessor &PP, clang::SourceRange Loc);
	void dump( std::ostream& out, const bool p_output[ METRIC_UNIT_MAX ], const MetricDumpFormat_e p_fmt = METRIC_DUMP_FORMAT_TREE );
};




#endif
