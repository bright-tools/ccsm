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

MetricSrcLexer::MetricSrcLexer(clang::CompilerInstance &p_CI, MetricUnit* p_topUnit, MetricOptions* p_options) : m_compilerInstance(p_CI), 
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

	if( m_options->getDumpTokens() )
	{
		std::cout << std::endl << "Start lexing translation unit: " << SM.getFileEntryForID( SM.getMainFileID() )->getName() << std::endl;
	}

	// Start preprocessing the specified input file.
	clang::Token result;
	PP.EnterMainSourceFile();
//	PP.SetMacroExpansionOnlyInDirectives();
	PP.SetCommentRetentionState(true,true);
	// TODO: What is this for??
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

		std::string fileName = m_currentFileName;

		// TODO: Need to differentiate between "non-function" and "whole-file" level counts - e.g. 
		//  unique numerical constants that aren't in functions and unique numerical constants across the whole file
		if( SHOULD_INCLUDE_FILE( m_options, fileName ))
		{
			if( !m_options->isDefFile( fileName ))
			{
				m_currentFileName = fileName;
			}
			fileUnit = m_topUnit->getSubUnit(m_currentFileName, METRIC_UNIT_FILE);

			// Not lex'd this file yet?
			if (!fileUnit->hasBeenProcessed(getLexType()))
			{
				// Check to see if we need to do a new function name lookup
				if ((m_currentFunctionName == "") || (!tokenLoc.isMacroID() && ((tokenLoc.getRawEncoding() > fnEnd.getRawEncoding()) || (tokenLoc.getRawEncoding() < fnStart.getRawEncoding()))))
				{
					std::string funcName = p_fnLocator->FindFunction( SM, tokenLoc, &fnEnd );
					fnStart = tokenLoc;

					if(( funcName.length() > 0 ) && m_options->getDumpTokens() )
					{
						std::cout << std::endl << "[fn:" << funcName << "@" << tokenLoc.getRawEncoding() << "-" << fnEnd.getRawEncoding() << "]" << std::endl << "  ";
					}

					m_currentFunctionName = funcName;

					if( m_currentFunctionName != "" ) 
					{
						if( SHOULD_INCLUDE_FUNCTION( m_options, m_currentFunctionName ))
						{
							m_currentUnit = fileUnit->getSubUnit(m_currentFunctionName, METRIC_UNIT_FUNCTION);
						}
						else
						{
							m_currentUnit = NULL;
						}
					}
					else
					{
						m_currentUnit = fileUnit;
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
			}
		}
	} while (result.isNot(clang::tok::eof));

	for( clang::SourceManager::fileinfo_iterator it = SM.fileinfo_begin();
		it != SM.fileinfo_end();
		it++ )
	{
		bool Invalid = false;
		clang::FileID fid = SM.translateFile( it->first );
		clang::StringRef Buffer = SM.getBufferData(fid, &Invalid);
#if 0
		const llvm::MemoryBuffer* mb = p_sm.getMemoryBufferForFile(it->first, &Invalid);
		clang::Token result;
#endif
		std::string fileName = it->first->getName();

		if (Invalid)
		{
		   /* TODO: Something more useful */
			return;
		}
 
		if( SHOULD_INCLUDE_FILE( m_options, fileName ))
		{
			MetricUnit* fileUnit = m_topUnit->getSubUnit(fileName, METRIC_UNIT_FILE);
			fileUnit->set( METRIC_TYPE_LINE_COUNT, countNewlines( Buffer ) );
			fileUnit->setProcessed(getLexType());
#if 0

			m_currentFileNumerics.clear();
			m_currentFileStrings.clear();
			m_currentFileCharConsts.clear();
			m_currentFileIdentifiers.clear();

			m_currentFileName = fileName;
			m_currentUnit = fileUnit;

			for( unsigned initLoop = 0 ; initLoop < SCC_MAX ; initLoop++ )
			{
				m_semiContainerDepth[ initLoop ] = 0;
				m_semiContainerOpen[ initLoop ] = false;
			}


			clang::Preprocessor& pp = m_compilerInstance.getPreprocessor();

#if 0 
			  Preprocessor(IntrusiveRefCntPtr<PreprocessorOptions> PPOpts,
               DiagnosticsEngine &diags, LangOptions &opts,
               const TargetInfo *target,
               SourceManager &SM, HeaderSearch &Headers,
               ModuleLoader &TheModuleLoader,
               IdentifierInfoLookup *IILookup = 0,
               bool OwnsHeaderSearch = false,
               bool DelayInitialization = false,
               bool IncrProcessing = false);
			  clang::DiagnosticOptions diagnosticOptions;
clang::TextDiagnosticPrinter *pTextDiagnosticPrinter =
    new clang::TextDiagnosticPrinter(
        llvm::outs(),
        diagnosticOptions);
llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs> pDiagIDs;
			  clang::DiagnosticsEngine *pDiagnosticsEngine =
    new clang::DiagnosticsEngine(pDiagIDs, pTextDiagnosticPrinter);
			  clang::Preprocessor new_pp(pDiagnosticsEngine,
				  pp.getDiagnostics(),
				  pp.getTargetInfo(),
				  pp.getSourceManager(),
				  pp.getHeaderSearchInfo(),
				  pp.getModuleLoader() );
#endif

			// Create a lexer starting at the beginning of this token.
#if 0
			  clang::Lexer TheLexer(fid, 
				                  mb,
								  pp);
#endif
			clang::Lexer TheLexer( p_sm.getLocForStartOfFile(fid),
				                  m_compilerInstance.getASTContext().getLangOpts(),
					         	  Buffer.begin(), 
								  Buffer.begin(), 
								  Buffer.end());

			TheLexer.SetCommentRetentionState(true);
			m_lastToken = clang::tok::eof;

			do
			{
				bool shouldLexToken = true;

				TheLexer.LexFromRawLexer(result);
				//new_pp.Lex( result );

				/* TODO: Could optimise this by not doing the function look-up for every single token, but 
				   determining whether or not the token's position has exceeded the range of the current function */
				std::string funcName = FindFunction( p_sm, result.getLocation(), p_fnMap );
				if( funcName != m_currentFunctionName )
				{
					if( m_options->getDumpTokens() )
					{
						std::cout << "[fn:" << funcName << "]";
					}
					CloseOutFnOrMtd();
				}
				if( funcName != "" ) 
				{
					if( SHOULD_INCLUDE_FUNCTION( m_options, funcName ))
					{
						m_currentUnit = m_topUnit->getSubUnit(fileName, METRIC_UNIT_FILE)->getSubUnit(funcName, METRIC_UNIT_FUNCTION);
					}
					else
					{
						shouldLexToken = false;
					}
				}
				else
				{
					m_currentUnit = m_topUnit->getSubUnit(fileName, METRIC_UNIT_FILE);
				}
				m_currentFunctionName = funcName;

				if(( result.getKind() == 0 ) || result.needsCleaning() )
				{
					std::string x = TheLexer.getSpelling( result, p_sm, m_compilerInstance.getASTContext().getLangOpts(), NULL );

					std::cout << std::endl << x << "/" << x.length() << std::endl;
				}

				if( shouldLexToken )
				{
					CountToken( result );
				}


			} while (result.isNot(clang::tok::eof));

			CloseOutFnOrMtd();
			fileUnit->set( METRIC_TYPE_TOKEN_NUMERIC_CONSTANTS_UNIQ, m_currentFileNumerics.size() );
			fileUnit->set( METRIC_TYPE_TOKEN_STRING_LITERALS_UNIQ, m_currentFileStrings.size() );
			fileUnit->set( METRIC_TYPE_TOKEN_UNRESERVED_IDENTIFIERS_UNIQ, m_currentFileIdentifiers.size() );
			fileUnit->set( METRIC_TYPE_TOKEN_CHAR_CONSTS_UNIQ, m_currentFileCharConsts.size() );
// TODO: Need to differentiate between "non-function" and "whole-file" level counts - e.g. unique numerical constants that aren't in functions and unique numerical constants across the whole file
#endif
		}
	} 
}