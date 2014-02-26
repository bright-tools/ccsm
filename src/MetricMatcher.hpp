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
#if !defined( METRIC_MATCHER_HPP )
#define       METRIC_MATCHER_HPP

#include "clang/Tooling/Tooling.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

#include <ostream>

#include "MetricUnit.hpp"

class MetricMatcher : public clang::ast_matchers::MatchFinder::MatchCallback
{
protected:
	MetricUnit* m_currentCU;
public:
    MetricMatcher(void);

	virtual ~MetricMatcher(void);

    virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) ;

	void dump( std::ostream& out );

};

#endif
