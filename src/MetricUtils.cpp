#include "MetricUtils.hpp"

#include "clang/Basic/SourceManager.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/Decl.h"

#include <iostream>

unsigned getControlDepth(const clang::Stmt* const p_stmt, clang::ASTContext* p_context )
{
	unsigned ret_val = 0;

	clang::SourceLocation loc,sloc;
	clang::SourceManager &SM = p_context->getSourceManager();
	const auto ancestors = p_context->getParents( *p_stmt );

	/* Examine all the parents */
    for (const auto &Parent : p_context->getParents( *p_stmt ) ) {
		/* Is the parent a Stmt? */
		const clang::Stmt* stmt = Parent.get<clang::Stmt>();
		if( stmt != NULL )
		{
			ret_val += getControlDepth( stmt, p_context );
		}
	}

	switch( p_stmt->getStmtClass() )
	{
		case clang::Stmt::IfStmtClass:
		case clang::Stmt::SwitchStmtClass:
		case clang::Stmt::DoStmtClass:
		case clang::Stmt::WhileStmtClass:
		case clang::Stmt::ForStmtClass:
			ret_val += 1;
#if 0
			std::cout << p_stmt->getStmtClassName() << ": " << ret_val << "\n";
#endif
			break;
		default:
			break;
	}

		 
	return ret_val;
}

extern size_t countNewlines( clang::StringRef& p_buffer )
{
	size_t pos = 0;
	size_t cnt = 0;
	while(( pos = p_buffer.find( '\n', pos )) && ( pos != p_buffer.npos))
	{
		cnt++;
		pos++;
	}
	return cnt;
}