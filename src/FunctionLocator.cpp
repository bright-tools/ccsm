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

#include "clang/AST/Stmt.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/AST/ASTContext.h"

#include <iostream>

TranslationUnitFunctionLocator* GlobalFunctionLocator::getLocatorFor( const std::string p_fileName )
{
	TranslationUnitFunctionLocator* ret_val = NULL;
	return &(m_map[ p_fileName ]);
#if 0
	MainSrcToFnLocMap_t::iterator it = m_map.find( p_fileName );

	if( it != m_map.end() )
	{
		ret_val = it->second; 
	}
	else
	{
		ret_val = new TranslationUnitFunctionLocator();
		m_map[ p_fileName ] = ret_val;
	}

	return ret_val;
#endif
}

GlobalFunctionLocator::~GlobalFunctionLocator()
{
#if 0
	for( MainSrcToFnLocMap_t::iterator it = m_map.begin();
		 it != m_map.end();
		 it++ )
	{
		delete( it->second );
	}
#endif
}

void TranslationUnitFunctionLocator::addFunctionLocation( const clang::ASTContext* const p_context, const std::string& p_name, const clang::Stmt* const p_body  )
{
	clang::SourceLocation endLoc = p_body->getLocEnd();
	clang::SourceLocation startLoc = p_body->getLocStart();
	clang::FileID fId = p_context->getSourceManager().getFileID( p_body->getLocStart() );
	unsigned int hashVal = fId.getHashValue();

//	std::cout << "addFunctionLocation : Adding to function map: " << p_name << " " << hashVal << " ( " << startLoc.getRawEncoding() << " to " << endLoc.getRawEncoding() << ")" << std::endl;

	LocationNamePair_t endNamePair( endLoc, p_name );
	m_map[ hashVal ][ startLoc ] = endNamePair;
}


std::string TranslationUnitFunctionLocator::FindFunction( const clang::SourceManager& p_SourceManager, clang::SourceLocation& p_loc ) const
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
				( (*func_it).first < p_loc ) &&
				( p_loc < (*func_it).second.first ))
			{
				ret_val = (*func_it).second.second;
				break;
			}
			/* Next function in the map */
			func_it++;
		}
	}
	return ret_val;
}
