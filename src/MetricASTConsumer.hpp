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
#if !defined( METRIC_AST_CONSUMER_HPP )
#define       METRIC_AST_CONSUMER_HPP

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"

#include <ostream>
#include <string>
#include <vector>

#include "MetricUnit.hpp"
#include "MetricOptions.hpp"
#include "MetricMatcher.hpp"

class MetricASTConsumer : public clang::ASTConsumer
{
protected:
	MetricVisitor *visitor;

public:
    explicit MetricASTConsumer(clang::ASTContext *CI, MetricUnit* p_topUnit, MetricOptions* p_options = NULL);

	virtual ~MetricASTConsumer(void);
	virtual void HandleTranslationUnit(clang::ASTContext &Context);
	void dump( std::ostream& out, 
		       const bool p_output[ METRIC_UNIT_MAX ], 
			   const MetricDumpFormat_e p_fmt = METRIC_DUMP_FORMAT_TREE );
};

#endif
