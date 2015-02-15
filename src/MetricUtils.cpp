#include "MetricUtils.hpp"

#include "clang/Basic/SourceManager.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/Decl.h"

#include <iostream>

bool isLastExecutableStmtInFn(const clang::Stmt* const p_stmt, clang::ASTContext* p_context )
{
	bool ret_val = true;
	clang::SourceLocation loc,sloc;
	clang::SourceManager &SM = p_context->getSourceManager();

	/* Examine all the parents */
    for (const auto &Parent : p_context->getParents( *p_stmt ) ) {
		/* Is the parent a Stmt? */
		const clang::Stmt* stmt = Parent.get<clang::Stmt>();
		if( stmt != NULL )
		{
			/* If the parent's an condition then the child won't necessarily be the last statement to execute */
			switch( stmt->getStmtClass() )
			{
				/* TODO: More needed here? - case, default? */
				case clang::Stmt::IfStmtClass:
					ret_val = false;
					break;
				default:
					break;
			}
			//std::cout << "Parent is type " << stmt->getStmtClassName() << std::endl;

			if( ret_val )
			{
				/* Is the node passed as a parameter the last child in the parent? */
				clang::Stmt::const_child_iterator children = stmt->child_begin();
				const clang::Stmt* last_child = NULL;
				while( children != stmt->child_end() )
				{
					last_child = *children;
					children++;
				}

				if( last_child == p_stmt )
				{
					/* Recurse up the node tree */
					ret_val = isLastExecutableStmtInFn( stmt, p_context );
				}
				else
				{
//					std::cout << "Statmement is not last child!" << std::endl;
					ret_val = false;
				}
			}
		}
		else
		{
			/* Parent is not a statement - Decl? */
			const clang::Decl* decl = Parent.get<clang::Decl>();
			/* Function decl? */
			if(( decl != NULL ) &&
			   ( decl->getKind() == clang::Decl::Function ))
			{
				/* If we've got here then the node originally passed must be the last child
				   of all the nodes between it and this function decl and mustn't have
				   been wrapped in a conditional, so yes, it's the last executable statement */
				ret_val = true;
//				std::cout << "Found decl: " << decl->getDeclKindName() << std::endl;
			}
			else
			{
				/* Not a statement and not a declaration - what is it? */
//				std::cout<< "Oh dear" << std::endl;
				ret_val = false;
			}
		}
    }

	return ret_val;
}

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
//			std::cout << p_stmt->getStmtClassName() << "\n";
			ret_val += 1;
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