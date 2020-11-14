#include "MetricUtils.hpp"

#include "clang/AST/Decl.h"
#include "clang/AST/ParentMapContext.h"
#include "clang/AST/Stmt.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Basic/SourceManager.h"

#include <iostream>

const SourceFileAndLine_t InvalidSourceAndLine = {false, 0, "", 0};

bool isStmtAnIfFromElseIf(const clang::Stmt *const p_parentStmt, const clang::Stmt * const p_stmt) {
    if (p_parentStmt->getStmtClass() == clang::Stmt::IfStmtClass) {
        const clang::Stmt *elseStmt = static_cast<const clang::IfStmt *>(p_parentStmt)->getElse();
        if ((elseStmt == p_stmt) && (elseStmt->getStmtClass() == clang::Stmt::IfStmtClass)) {
            return true;
        }
    }
    return false;
}

unsigned getControlDepth(const clang::Stmt *const p_stmt, clang::ASTContext *p_context,
                         const bool p_childIsElseIf) {
    unsigned ret_val = 0;

    clang::SourceLocation loc, sloc;

    /* Examine all the parents */
    for (const auto &Parent : p_context->getParents(*p_stmt)) {
        const clang::Stmt *stmt = Parent.get<clang::Stmt>();
        if (stmt != NULL) {
            ret_val += getControlDepth(stmt, p_context, isStmtAnIfFromElseIf(stmt, p_stmt));
        }
    }

    switch (p_stmt->getStmtClass()) {
        case clang::Stmt::IfStmtClass: {
            /* We don't count "else if"s as increasing the depth, as the original "if" will already
               have done that */
            if (!p_childIsElseIf) {
                ret_val++;
            }
        }
        break;
        case clang::Stmt::SwitchStmtClass:
        case clang::Stmt::DoStmtClass:
        case clang::Stmt::WhileStmtClass:
        case clang::Stmt::ForStmtClass:
            ret_val += 1;
#if 0
			std::cout << "getControlDepth : " << p_stmt->getStmtClassName() << ": " << ret_val << "\n";
#endif
            break;
        default:
            break;
    }

    return ret_val;
}

extern size_t countNewlines(clang::StringRef &p_buffer) {
    return p_buffer.count('\n');
}

#ifdef _WIN32
#define HANDLE_CHAR_CASE(_x) tolower(_x)
#include <direct.h>
#define getcwd _getcwd // Deal with MSFT "deprecation" warning
#else
#define HANDLE_CHAR_CASE(_x) (_x)
#include <strings.h>
#include <unistd.h>
#define stricmp strcasecmp
#endif

#include "llvm/Support/Path.h"

#if 0
void test()
{
	std::cout << makeRelative("C:\\foo.h", "C:\\" ) << std::endl;
	std::cout << makeRelative("C:\\foo.h", "C:\\Windows") << std::endl;
	std::cout << makeRelative("C:\\foo.h", "C:\\Windows\\") << std::endl;
	std::cout << makeRelative("F:\\foo.h", "C:\\Windows\\") << std::endl;
	std::cout << makeRelative("C:\\Temp\\foo.h", "C:\\Windows\\") << std::endl;
	std::cout << makeRelative("c:\\WINDOWS\\foo.h", "C:\\Windows\\") << std::endl;
	std::cout << makeRelative("c:\\WINDOWS\\jimbo\\zooma\\foo.h", "C:\\Windows\\") << std::endl;
	std::cout << makeRelative("c:\\WINDOWS\\jimbo\\zooma\\foo.h", "C:\\Windows") << std::endl;
	std::cout << makeRelative("c:\\WINDOWS", "C:\\Windows") << std::endl;
	std::cout << makeRelative("c:\\WINDOW", "C:\\Windows") << std::endl;
	std::cout << makeRelative("c:\\WINDOWS", "C:\\Window") << std::endl;
	std::cout << makeRelative("c:\\WINDOW\\", "C:\\Windows") << std::endl;
	std::cout << makeRelative("c:\\WINDOWS\\", "C:\\Window") << std::endl;
	std::cout << makeRelative("c:\\WINDOW", "C:\\Windows\\") << std::endl;
	std::cout << makeRelative("c:\\WINDOWS", "C:\\Window\\") << std::endl;
}
#endif

std::string makeRelative(const std::string &p_path, const std::string &p_cwd) {
    std::string ret_val;
    const std::string sep = llvm::sys::path::get_separator().str();

    size_t path_pos = p_path.find(sep);
    size_t cwd_pos = p_cwd.find(sep);

    /* If the first character is a separator, then we're using UNIX-style paths.
       Otherwise case-insensitively compare the roots (e.g. 'C:') - if these are
       different then we can't construct a relative path */
    if (llvm::sys::path::is_separator(p_cwd[0]) ||
        (stricmp(p_path.substr(0, path_pos).c_str(), p_cwd.substr(0, cwd_pos).c_str()) == 0)) {
        const size_t shortest =
            (p_cwd.length() < p_path.length()) ? p_cwd.length() : p_path.length();
        size_t pos;
        size_t last_sep = 0;
        ret_val = "";

        /* Determine the extent to which the paths match */
        for (pos = 0; pos < shortest; pos++) {
            /* Paths no longer match, so break out */
            if (HANDLE_CHAR_CASE(p_cwd[pos]) != HANDLE_CHAR_CASE(p_path[pos])) {
                break;
            }
            if (llvm::sys::path::is_separator(p_cwd[pos])) {
                last_sep = pos;
            }
        }

        /* Was there a mis-match part way through a string (e.g. 'window' vs
           'windows')?  If so, we need to treat them as different and
           back-track to the last separator */
        if ((pos < p_path.length() && !(llvm::sys::path::is_separator(p_path[pos]) ||
                                        llvm::sys::path::is_separator(p_path[pos - 1]))) ||
            (pos < p_cwd.length() && !(llvm::sys::path::is_separator(p_cwd[pos]) ||
                                       llvm::sys::path::is_separator(p_cwd[pos - 1])))) {
            pos = last_sep + 1;
        }

        /* Is the last matching point higher in the directory tree than the
         * CWD?*/
        if (pos < p_cwd.length()) {
            /* Add ".." backtracks for each separator */
            for (size_t back = pos; back < p_cwd.length(); back++) {
                if (llvm::sys::path::is_separator(p_cwd[back])) {
                    ret_val += ".." + sep;
                }
            }
            /* If the cwd didn't end in a separator then we need to add an
             * additional backtrack */
            if (!llvm::sys::path::is_separator(p_cwd[p_cwd.length() - 1])) {
                ret_val += ".." + sep;
            }
        }

#if 1
        /* Append the remaining non-matching part of p_path.  If it starts with
         * a separator, ignore it */
        if (llvm::sys::path::is_separator(p_path[pos])) {
            pos++;
        }
#endif
        ret_val += p_path.substr(pos);

        if (ret_val.length() == 0) {
            ret_val = ".";
        }
    } else {
        ret_val = p_path;
    }

    return ret_val;
}

std::string makeRelative(const std::string &p_path) {
    const std::string cwd = getcwd(NULL, 0);
    return makeRelative(p_path, cwd);
}

SourceFileAndLine_t getFileAndLine(const clang::SourceManager &p_sourceManager,
                                   const clang::SourceLocation *p_sourceLoc) {
    SourceFileAndLine_t ret_val;

    ret_val.Valid = false;

    if (p_sourceLoc) {
        clang::SourceLocation resolved = *p_sourceLoc;
        if (resolved.isMacroID()) {
            resolved = p_sourceManager.getFileLoc(resolved);
        }

        ret_val.Valid = true;
        ret_val.FileName = p_sourceManager.getFilename(resolved).str();
        ret_val.LineNo = p_sourceManager.getSpellingLineNumber(resolved);
        ret_val.Column = p_sourceManager.getSpellingColumnNumber(resolved);
    }

    return ret_val;
}