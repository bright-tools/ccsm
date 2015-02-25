/*
   @file
   @brief 

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

#if !defined( FUNCTION_LOCATOR_HPP )
#define       FUNCTION_LOCATOR_HPP

#include "clang/Basic/SourceLocation.h"

#include "clang/AST/ASTContext.h"
#include "clang/AST/Stmt.h"

#include <utility>
#include <map>

class TranslationUnitFunctionLocator
{
private:
	typedef std::pair< clang::SourceLocation, std::string >      LocationNamePair_t;
	typedef std::map< clang::SourceLocation, LocationNamePair_t> StartEndPair_t;
	/** Maps file ID to a map of ... */
	typedef std::map< unsigned, StartEndPair_t>					 SrcStartToFunctionMap_t;

	SrcStartToFunctionMap_t m_map;
public:
	void addFunctionLocation( const clang::ASTContext* const p_context, const std::string& p_name, const clang::FunctionDecl * const p_func );
	std::string FindFunction( const clang::SourceManager& p_SourceManager, clang::SourceLocation& p_loc ) const;

	void dump( std::ostream& p_out ) const;
};

class GlobalFunctionLocator
{
	private:
		typedef std::map< std::string, TranslationUnitFunctionLocator >     MainSrcToFnLocMap_t;

		MainSrcToFnLocMap_t m_map;

	public:
        TranslationUnitFunctionLocator* getLocatorFor( const std::string p_fileName );
		virtual ~GlobalFunctionLocator();
		void dump( std::ostream& p_out ) const;
};

#endif
