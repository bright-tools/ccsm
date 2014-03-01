#include "clang/Basic/SourceManager.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/Decl.h"

#include "MetricUtils.hpp"

#include <iostream>

bool isLastExecutableStmtInFn(clang::Stmt* p_stmt, clang::ASTContext* p_context )
{
	bool ret_val;
	clang::SourceLocation loc,sloc;
	clang::SourceManager &SM = p_context->getSourceManager();

	clang::ASTContext::ParentVector ancestors = p_context->getParents( *p_stmt );

	/* Examine all the parents */
    for (clang::ASTContext::ParentVector::const_iterator I = ancestors.begin(),
                                                         E = ancestors.end();
         I != E;
         ++I) {
		const clang::Stmt* decl = I->get<clang::Stmt>();
		if( decl != NULL )
		{
			std::cout << "MUNCHY" << decl->getStmtClassName() << std::endl;
		}
		else
		{
			std::cout << "OHDEAR" << std::endl;
		}
    }

    if( p_stmt->getLocEnd().isMacroID() ) {
		// TODO: If the start is in a macro, won't the end be in a macro?
        llvm::errs() << "Annotator::HandleNonCompound ~ Statement end is in a macro\n";
        loc = SM.getSpellingLoc(p_stmt->getLocEnd());
    } else {
		loc = p_stmt->getLocEnd();
	}

    // Non-compound statement is likely to have a semi-colon separator.  Try and find it.
    sloc = findSemiAfterLocation( loc, *p_context );
	sloc = findSemiAfterLocation( sloc, *p_context );

	if( sloc.isValid() )
	{
		ret_val = false;
	} else
	{
		ret_val = true;
	}
	return ret_val;
}

/*

==============================================================================
LLVM Release License
==============================================================================
University of Illinois/NCSA
Open Source License

Copyright (c) 2007-2013 University of Illinois at Urbana-Champaign.
All rights reserved.

Developed by:

    LLVM Team

    University of Illinois at Urbana-Champaign

    http://llvm.org

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal with
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:


    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimers.

    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimers in the
      documentation and/or other materials provided with the distribution.

    * Neither the names of the LLVM Team, University of Illinois at
      Urbana-Champaign, nor the names of its contributors may be used to
      endorse or promote products derived from this Software without specific
      prior written permission.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS WITH THE
SOFTWARE.

==============================================================================
The LLVM software contains code written by third parties.  Such software will
have its own individual LICENSE.TXT file in the directory in which it appears.
This file will describe the copyrights, license, and restrictions which apply
to that code.

The disclaimer of warranty in the University of Illinois Open Source License
applies to all code in the LLVM Distribution, and nothing in any of the
other licenses gives permission to use the names of the LLVM Team or the
University of Illinois to endorse or promote products derived from this
Software.

*/

#include "clang/Tooling/Refactoring.h"
#include "clang/Lex/Lexer.h"
#include "clang/AST/ASTContext.h"

using namespace clang;

/* TODO: This source is pulled from ARCMigrate in clang */

SourceLocation findSemiAfterLocation(SourceLocation loc,
                                            ASTContext &Ctx,
                                            bool IsDecl) {
  SourceManager &SM = Ctx.getSourceManager();
  if (loc.isMacroID()) {
    if (!Lexer::isAtEndOfMacroExpansion(loc, SM, Ctx.getLangOpts(), &loc))
      return SourceLocation();
  }
  loc = Lexer::getLocForEndOfToken(loc, /*Offset=*/0, SM, Ctx.getLangOpts());

  // Break down the source location.
  std::pair<FileID, unsigned> locInfo = SM.getDecomposedLoc(loc);

  // Try to load the file buffer.
  bool invalidTemp = false;
  StringRef file = SM.getBufferData(locInfo.first, &invalidTemp);
  if (invalidTemp)
  {
    return SourceLocation();
  }
  const char *tokenBegin = file.data() + locInfo.second;

  // Lex from the start of the given location.
  Lexer lexer(SM.getLocForStartOfFile(locInfo.first),
              Ctx.getLangOpts(),
              file.begin(), tokenBegin, file.end());
  Token tok;
  lexer.LexFromRawLexer(tok);
  if (tok.isNot(tok::semi)) {
    if (!IsDecl){
      return SourceLocation();
	}
    // Declaration may be followed with other tokens; such as an __attribute,
    // before ending with a semicolon.
    return findSemiAfterLocation(tok.getLocation(), Ctx, /*IsDecl*/true);
  }

  return tok.getLocation();
}
