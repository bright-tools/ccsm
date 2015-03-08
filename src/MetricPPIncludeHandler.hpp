/*
   @file
   @brief TODO

   @author John Bailey
   @copyright Copyright 2015 John Bailey

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
#if !defined( METRIC_PP_INCLUDE_HANDLER_HPP )
#define       METRIC_PP_INCLUDE_HANDLER_HPP

#include "MetricOptions.hpp"

#include "clang/Lex/Preprocessor.h"

#include <string>

class MetricPPIncludeHandler : public clang::PPCallbacks
{
	private:
		std::string& m_currentFile;
		MetricOptions* m_options;
    public:
		MetricPPIncludeHandler( MetricOptions* p_options, std::string& p_currentFile );
		virtual ~MetricPPIncludeHandler( void );
		virtual void InclusionDirective (clang::SourceLocation HashLoc, const clang::Token &IncludeTok, clang::StringRef FileName, bool IsAngled, clang::CharSourceRange FilenameRange, const clang::FileEntry *File, clang::StringRef SearchPath, clang::StringRef RelativePath, const clang::Module *Imported);

};

#endif