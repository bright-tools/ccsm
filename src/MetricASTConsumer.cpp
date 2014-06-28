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

#include "MetricASTConsumer.hpp"

MetricASTConsumer::MetricASTConsumer(clang::CompilerInstance &CI, MetricUnit* p_topUnit, MetricOptions* p_options ) : visitor(new MetricVisitor(CI, p_topUnit, p_options))
{ 
}

MetricASTConsumer::~MetricASTConsumer(void) 
{
};

void MetricASTConsumer::HandleTranslationUnit(clang::ASTContext &Context) {
	/* we can use ASTContext to get the TranslationUnitDecl, which is
		a single Decl that collectively represents the entire source file */
	visitor->TraverseDecl(Context.getTranslationUnitDecl());
}

void MetricASTConsumer::dump( std::ostream& out, const bool p_output[ METRIC_UNIT_MAX ], const MetricDumpFormat_e p_fmt )
{
	visitor->dump( out, p_output, p_fmt );
}
