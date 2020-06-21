/*
   Copyright 2015 John Bailey

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

#include "MetricPPIncludeHandler.hpp"

MetricPPIncludeHandler::MetricPPIncludeHandler(const MetricOptions &p_options,
                                               clang::SourceManager &p_SM,
                                               std::string &p_currentFile)
    : clang::PPCallbacks(), m_options(p_options), m_currentFile(p_currentFile),
      m_SM(p_SM) {}

MetricPPIncludeHandler::~MetricPPIncludeHandler(void) {}

void MetricPPIncludeHandler::FileChanged(
    clang::SourceLocation Loc, FileChangeReason Reason,
    clang::SrcMgr::CharacteristicKind FileType, clang::FileID PrevFID) {
    std::string fileName = m_SM.getFilename(Loc).str();

    if (!m_options.isDefFile(fileName)) {
        m_currentFile = fileName;
    }
}

#if 0
void MetricPPIncludeHandler::InclusionDirective(clang::SourceLocation HashLoc, const clang::Token &IncludeTok, clang::StringRef FileName, bool IsAngled, clang::CharSourceRange FilenameRange, const clang::FileEntry *File, clang::StringRef SearchPath, clang::StringRef RelativePath, const clang::Module *Imported)
{
	if( !m_options->isDefFile( FileName ))
	{
		m_currentFile = File->getName();
	}
}
#endif