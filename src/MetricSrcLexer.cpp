/*
   Copyright 2014 John Bailey

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

#include "MetricSrcLexer.hpp"
#include "MetricUtils.hpp"
#include "MetricPPIncludeHandler.hpp"
#include "MetricUnit.hpp"
#include "clang/Lex/Preprocessor.h"
#include "clang/AST/ASTContext.h"

#include <iostream>

MetricSrcLexer::MetricSrcLexer(clang::CompilerInstance &p_CI, MetricUnit* p_topUnit, MetricOptions& p_options) : m_compilerInstance(p_CI), 
	                                                                                                             m_topUnit( p_topUnit ), 
	                                                                                                             m_options( p_options ),
																												 m_currentUnit( NULL )
{
}

MetricSrcLexer::~MetricSrcLexer(void)
{
}

void MetricSrcLexer::LexSources( clang::CompilerInstance& p_ci, const TranslationUnitFunctionLocator* const p_fnLocator )
{
	clang::Preprocessor &PP = p_ci.getPreprocessor();
	clang::SourceManager &SM = p_ci.getSourceManager();

	if( m_options.getDumpTokens() )
	{
		m_options.getOutput() << std::endl << "Start lexing translation unit: " << SM.getFileEntryForID( SM.getMainFileID() )->getName() << std::endl;
	}

	// Start preprocessing the specified input file.
	clang::Token result;
	m_lastToken.setKind( clang::tok::eof );
	PP.EnterMainSourceFile();
	PP.SetCommentRetentionState(true,true);
	PP.addPPCallbacks( llvm::make_unique<MetricPPIncludeHandler>( m_options, SM, m_currentFileName ) );
	clang::SourceLocation fnStart;
	clang::SourceLocation fnEnd;
	m_currentFileName = SM.getFileEntryForID( SM.getMainFileID() )->getName();
	MetricUnit* fileUnit = NULL;

	do 
	{
		clang::SourceLocation tokenLoc;
		bool shouldLexToken = true;

		PP.Lex(result);

		tokenLoc = result.getLocation();

		/* If it's a macro then we want the expansion location */
		if (tokenLoc.isMacroID())
		{
			tokenLoc = m_compilerInstance.getSourceManager().getFileLoc(tokenLoc);
		}

		std::string fileName = m_currentFileName;

		// TODO: Need to differentiate between "non-function" and "whole-file" level counts - e.g. 
		//  unique numerical constants that aren't in functions and unique numerical constants across the whole file
		if( m_options.ShouldIncludeFile( fileName ))
		{
			if( !m_options.isDefFile( fileName ))
			{
				m_currentFileName = fileName;
			}
			fileUnit = m_topUnit->getSubUnit(m_currentFileName, METRIC_UNIT_FILE);

			// Not lex'd this file yet?
			if (!fileUnit->hasBeenProcessed(getLexType()))
			{
				// Check to see if we need to do a new function name lookup
				if ((m_currentFunctionName == "") || (tokenLoc.getRawEncoding() > fnEnd.getRawEncoding()) || (tokenLoc.getRawEncoding() < fnStart.getRawEncoding()))
				{
					std::string funcName = p_fnLocator->FindFunction(SM, tokenLoc, &fnEnd, &m_bodyStartLocation);
					m_inBody = false;
					fnStart = tokenLoc;

					if(( funcName.length() > 0 ) && m_options.getDumpTokens() )
					{
						m_options.getOutput() << std::endl << "[fn:" << funcName << "@" << tokenLoc.getRawEncoding() << "-" << fnEnd.getRawEncoding() << "]" << std::endl << "  ";
					}

					m_currentFunctionName = funcName;

					if( m_currentFunctionName != "" ) 
					{
						if( m_options.ShouldIncludeFunction( m_currentFunctionName ))
						{
							CloseOutFnOrMtd();
							m_currentUnit = fileUnit->getSubUnit(m_currentFunctionName, METRIC_UNIT_FUNCTION);
							m_waitingForBody = true;
						}
						else
						{
							CloseOutFnOrMtd();
							m_currentUnit = NULL;
						}
					}
					else
					{
						if (m_currentUnit != fileUnit)
						{
							CloseOutFnOrMtd();
							m_currentUnit = fileUnit;
							EnterFileScope();
						}
					}
				}
				else
				{
					if (m_waitingForBody)
					{
						if ((m_bodyStartLocation < tokenLoc) ||
							(m_bodyStartLocation == tokenLoc))
						{
							m_inBody = true;
							m_waitingForBody = false;
						}
					}
				}

				if( m_currentUnit == NULL )
				{
					shouldLexToken = false;
				}
			}
			else
			{
				shouldLexToken = false;
			}

			if( shouldLexToken )
			{
				ProcessToken( result );
				m_lastToken = result;
			}
		}
	} while (result.isNot(clang::tok::eof));

	if (m_currentUnit != NULL)
	{
		CloseOutFnOrMtd();
	}

	for (clang::SourceManager::fileinfo_iterator it = SM.fileinfo_begin();
		it != SM.fileinfo_end();
		it++)
	{
		bool Invalid = false;
		clang::FileID fid = SM.translateFile(it->first);
		clang::StringRef Buffer = SM.getBufferData(fid, &Invalid);
		std::string fileName = it->first->getName();

		if (m_options.ShouldIncludeFile( fileName))
		{
			MetricUnit* fileUnit = m_topUnit->getSubUnit(fileName, METRIC_UNIT_FILE);
			fileUnit->set(METRIC_TYPE_LINE_COUNT, countNewlines(Buffer), NULL);
			fileUnit->setProcessed(getLexType());
		}
	}
}