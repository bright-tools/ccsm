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

#include "FunctionLocator.hpp"
#include "MetricUtils.hpp"

#include "clang/AST/Stmt.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/AST/ASTContext.h"

#include <iostream>
#include <string.h>

GlobalFunctionLocator::GlobalFunctionLocator(MetricOptions& p_options) : m_options(p_options)
{
}

TranslationUnitFunctionLocator* GlobalFunctionLocator::getLocatorFor( const std::string p_fileName )
{
	TranslationUnitFunctionLocator* ret_val = NULL;
	MainSrcToFnLocMap_t::iterator it = m_map.find( p_fileName );

	if( it != m_map.end() )
	{
		ret_val = it->second; 
	}
	else
	{
		ret_val = new TranslationUnitFunctionLocator(m_options);
		m_map.insert(std::pair<std::string, TranslationUnitFunctionLocator* >(p_fileName, ret_val));
	}

	return ret_val;
}

void GlobalFunctionLocator::dump() const
{
	MainSrcToFnLocMap_t::const_iterator it;

	for( it = m_map.begin();
		 it != m_map.end();
		 it++ )
	{
		std::string fileName = it->first;
		if (!m_options.getUseAbsoluteFileNames())
		{
			fileName = makeRelative(fileName);
		}
		m_options.getOutput() << "Translation Unit: " << fileName << std::endl;
		it->second->dump();
	}
}


GlobalFunctionLocator::~GlobalFunctionLocator()
{
	for( MainSrcToFnLocMap_t::iterator it = m_map.begin();
		 it != m_map.end();
		 it++ )
	{
		delete( it->second );
	}
}

TranslationUnitFunctionLocator::TranslationUnitFunctionLocator(MetricOptions& p_options) : m_options(p_options)
{
}

void TranslationUnitFunctionLocator::addFunctionLocation(const clang::ASTContext* const p_context, const std::string& p_name, const clang::FunctionDecl * const p_func)
{
	// TODO: need to check if getLocEnd() is a macro location?
	clang::SourceLocation endLoc = p_func->getBody()->getLocEnd();
	clang::SourceLocation startLoc;

	if (m_options.getPrototypesAreFileScope())
	{
		startLoc = p_func->getBody()->getLocStart();
	}
	else
	{
		startLoc = p_func->getLocStart();
	}
	if (startLoc.isMacroID())
	{
		startLoc = p_context->getSourceManager().getFileLoc(startLoc);
	}
	clang::FileID fId = p_context->getSourceManager().getFileID(startLoc);
	unsigned int hashVal = fId.getHashValue();

#if 0
	std::cout << "addFunctionLocation : Adding to function map: " << p_name << " " << hashVal << " ( " << startLoc.getRawEncoding() << " to " << endLoc.getRawEncoding() << ")" << std::endl;
#endif

	LocationNamePair_t endNamePair( endLoc, p_name );
	m_map[ hashVal ][ startLoc ] = endNamePair;
}

void TranslationUnitFunctionLocator::dump() const
{
	SrcStartToFunctionMap_t::const_iterator it;

	for( it = m_map.begin();
		 it != m_map.end();
		 it++ )
	{
		m_options.getOutput() << " File ID: " << it->first << std::endl;
		StartEndPair_t::const_iterator pit;

		for( pit = it->second.begin();
			 pit != it->second.end();
			 pit++ )
		{
			m_options.getOutput() << "  Function Definition: " << pit->second.second << " (" << pit->first.getRawEncoding() << "-" << pit->second.first.getRawEncoding() << ")" << std::endl;
		}
	}
}

std::string TranslationUnitFunctionLocator::FindFunction( const clang::SourceManager& p_SourceManager, clang::SourceLocation& p_loc, clang::SourceLocation* p_end ) const
{
	std::string ret_val = "";
	unsigned fileIdHash = p_SourceManager.getFileID( p_loc ).getHashValue();
	SrcStartToFunctionMap_t::const_iterator file_it = m_map.find(fileIdHash);
	if( file_it != m_map.end() )
	{
		StartEndPair_t::const_iterator func_it = file_it->second.begin();

		/* While we've not found a matching function and there are still functions to consider ... */
		while(( ret_val == "" ) && ( func_it != file_it->second.end()))
		{
			/* Does the location we're considering match the function start or end or is it within those bounds? */
			if(( p_loc == (*func_it).first ) || 
				( p_loc == (*func_it).second.first ) ||
				(( (*func_it).first < p_loc ) &&
				 ( p_loc < (*func_it).second.first )))
			{
				ret_val = (*func_it).second.second;
				if( p_end != NULL )
				{
					*p_end = (*func_it).second.first;
				}
				break;
			}
			/* Next function in the map */
			func_it++;
		}
	}
	return ret_val;
}
