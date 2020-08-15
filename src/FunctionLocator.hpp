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

#if !defined(FUNCTION_LOCATOR_HPP)
#define FUNCTION_LOCATOR_HPP

#include "MetricOptions.hpp"

#include "clang/Basic/SourceLocation.h"

#include "clang/AST/ASTContext.h"
#include "clang/AST/Stmt.h"

#include <map>
#include <utility>

/**
    Supports finding the location of functions within a translation unit.

        The functions may be spread across different files (e.g. due to included
   files containing functions), hence this class tracks the source of each
   function by file ID

        @see GlobalFunctionLocator
 */
class TranslationUnitFunctionLocator {
  private:
    typedef struct {
        clang::SourceLocation EndLocation;
        clang::SourceLocation BodyStart;
        std::string Name;
    } FunctionInfo_t;

    /** Maps the function start-point against the end-point/name information
     * pair
     */
    typedef std::map<clang::SourceLocation, FunctionInfo_t> StartInfoPair_t;
    /** Maps file ID to a map of function start-point information */
    typedef std::map<unsigned, StartInfoPair_t> SrcStartToFunctionMap_t;

    /** Map of file IDs and associated function position/name data */
    SrcStartToFunctionMap_t m_map;
    /** Reference to the options selected by the user */
    MetricOptions &m_options;

  public:
    TranslationUnitFunctionLocator(MetricOptions &p_options);

    void addFunctionLocation(const clang::ASTContext *const p_context, const std::string &p_name,
                             const clang::FunctionDecl *const p_func);

    std::string FindFunction(const clang::SourceManager &p_SourceManager,
                             clang::SourceLocation &p_loc, clang::SourceLocation *p_end = NULL,
                             clang::SourceLocation *p_body = NULL) const;

    void dump() const;
};

/** Supports finding the location of functions across a number of translation
   units

    @see TranslationUnitFunctionLocator
*/
class GlobalFunctionLocator {
  private:
    typedef std::map<std::string, TranslationUnitFunctionLocator *> MainSrcToFnLocMap_t;

    MainSrcToFnLocMap_t m_map;

    /** Reference to the options selected by the user */
    MetricOptions &m_options;

  public:
    GlobalFunctionLocator(MetricOptions &p_options);
    TranslationUnitFunctionLocator *getLocatorFor(const std::string p_fileName);
    virtual ~GlobalFunctionLocator();
    void dump() const;
};

#endif
