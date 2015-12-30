/*
   @file
   @brief TODO

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
#if !defined( METRIC_PP_CUSTOMER_HPP )
#define       METRIC_PP_CUSTOMER_HPP

#include "MetricUnit.hpp"
#include "MetricOptions.hpp"
#include "FunctionLocator.hpp"

#include "clang/AST/CommentVisitor.h"
#include "clang/Lex/Preprocessor.h"

#include <set>
#include <string>

class MetricPPCustomer : public clang::PPCallbacks, public clang::CommentHandler
{
protected:
	MetricUnit*		                  m_topUnit;
	const MetricOptions&              m_options;
	std::string                       m_commentFile;
    std::set<std::string>*            m_commentFileList;

public:
	MetricPPCustomer( MetricUnit* p_topUnit, std::set<std::string>* p_commentFileList, const MetricOptions& p_options );
	virtual ~MetricPPCustomer();
	virtual void InclusionDirective (clang::SourceLocation HashLoc, const clang::Token &IncludeTok, clang::StringRef FileName, bool IsAngled, clang::CharSourceRange FilenameRange, const clang::FileEntry *File, clang::StringRef SearchPath, clang::StringRef RelativePath, const clang::Module *Imported);
	virtual bool HandleComment(clang::Preprocessor &PP, clang::SourceRange Loc);
};

#endif
