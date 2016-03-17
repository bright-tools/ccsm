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
#include "FunctionLocator.hpp"

#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/AST/ASTContext.h"

#include <ostream>
#include <string>
#include <set>

class MetricVisitor : public clang::RecursiveASTVisitor<MetricVisitor>
{
protected:
	typedef struct
	{
		MetricUnit::counter_t path_count;
		bool                  path_has_return;
	} PathResults;

	/** Table of pairs which match binary operators against metrics, used to initialise binaryOperatorToMetricMap */
	const static std::pair<clang::BinaryOperator::Opcode, MetricType_e> binaryOperatorToMetricPairs[];

	/** Map used to look up the metric associated with a particular binary operator */
	const static std::map<clang::BinaryOperator::Opcode, MetricType_e> binaryOperatorToMetricMap;

	clang::CompilerInstance&        m_compilerInstance;
	clang::ASTContext*		        m_astContext           = NULL;
	MetricUnit*		                m_topUnit              = NULL;
	std::string				        m_currentFileName;
	std::string				        m_currentFunctionName;
	MetricUnit*				        m_currentUnit          = NULL;
    MetricOptions&	                m_options;
	std::set<std::string>	        m_fnsCalled;          
	TranslationUnitFunctionLocator* m_fnLocator            = NULL;

	void HandleLoc( const clang::SourceLocation& p_loc );
	void DeclCommon(const clang::DeclContext* p_declCtxt, const clang::Decl* p_decl);
	void CloseOutFnOrMtd( void );
	bool ShouldIncludeFile( const std::string& p_file );
	void IncrementMetric( MetricUnit* const p_unit, const MetricType_e p_metricType );
	void IncrementMetric( MetricUnit* const p_unit, const MetricType_e p_metricType, const MetricUnit* const p_file );
	void UpdateCurrentFileName( const clang::SourceLocation &loc );
	std::string getDefResolvedFileName( const clang::SourceLocation &loc );
	PathResults getPathCount(const clang::Stmt* const p_stmt, uint16_t depth = 0);
	PathResults getOtherPathCount(const clang::Stmt* const p_stmt, uint16_t depth = 0);
	PathResults getIfPathCount(const clang::IfStmt* const p_stmt, uint16_t depth = 0);
	PathResults getSwitchPathCount(const clang::SwitchStmt* const p_stmt, uint16_t depth = 0);
	void CountStatements(const clang::Stmt* const p_stmt);
	void CountStatements(const clang::Stmt::const_child_range& p_children);

public:
	
	explicit MetricVisitor(clang::CompilerInstance &p_CI, MetricUnit* p_topUnit, MetricOptions& p_options,TranslationUnitFunctionLocator* p_fnLocator = NULL);
	virtual ~MetricVisitor(void);
	virtual bool VisitFunctionDecl(clang::FunctionDecl *func);
	virtual bool VisitVarDecl(clang::VarDecl *p_varDec);
	virtual bool VisitForStmt(clang::ForStmt *p_forSt);
	virtual bool VisitGotoStmt(clang::GotoStmt *p_gotoSt);
	virtual bool VisitLabelStmt(clang::LabelStmt *p_LabelSt);
	virtual bool VisitDoStmt(clang::DoStmt *p_doSt);
	virtual bool VisitWhileStmt(clang::WhileStmt *p_whileSt);
	virtual bool VisitTypedefNameDecl(const clang::TypedefNameDecl *T);
	virtual bool VisitReturnStmt(clang::ReturnStmt *p_returnSt);
	virtual bool VisitSwitchStmt(clang::SwitchStmt *p_switchSt);
	virtual bool VisitConditionalOperator(clang::ConditionalOperator *p_condOp);
	virtual bool VisitDefaultStmt(clang::DefaultStmt *p_defaultSt);
	virtual bool VisitContinueStmt(clang::ContinueStmt *p_continueSt);
	virtual bool VisitCaseStmt(clang::CaseStmt *p_caseSt);
	virtual bool VisitCompoundStmt(clang::CompoundStmt *p_compoundSt);
	virtual bool VisitBinaryOperator(clang::BinaryOperator *p_binOp);
	virtual bool VisitUnaryOperator(clang::UnaryOperator *p_uOp);
	virtual bool VisitStmt(clang::Stmt *p_statement);
	virtual bool VisitIfStmt(clang::IfStmt *p_ifSt);
	virtual bool VisitRecordDecl(clang::RecordDecl* p_recordDecl);
	virtual bool VisitExplicitCastExpr(clang::ExplicitCastExpr *p_castExpr);
	virtual bool VisitCallExpr(clang::CallExpr *p_callExpr);
	virtual bool VisitArraySubscriptExpr (clang::ArraySubscriptExpr *p_subs);
	virtual bool VisitMemberExpr( clang::MemberExpr* p_memberExpr );
	virtual bool VisitUnaryExprOrTypeTraitExpr( clang::UnaryExprOrTypeTraitExpr* p_unaryExpr );
	virtual bool TraverseDecl(clang::Decl *p_decl);
	virtual bool TraverseStmt(clang::Stmt *p_stmt);
	virtual bool VisitEnumDecl(clang::EnumDecl* p_enumDecl);
	virtual bool VisitCastExpr(clang::CastExpr *p_castExp);


#if 0
	void dump( std::ostream& out, const bool p_output[ METRIC_UNIT_MAX ], const MetricDumpFormat_e p_fmt = METRIC_DUMP_FORMAT_TREE );
#endif
};

#endif
