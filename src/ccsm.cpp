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

#include "CcsmOptionsHandler.hpp"
#include "MetricFrontendActors.hpp"
#include "FunctionLocator.hpp"
#include "MetricLinkageResolver.hpp"
#include "MetricDumper.hpp"

#include "clang/Tooling/Tooling.h"
#include "llvm/Support/Signals.h" 

#include <iostream>

int main(int argc, const char **argv) {
	MetricUnit topUnit( NULL, "Global", METRIC_UNIT_GLOBAL);
	std::set<std::string> commentFileList;
	CcsmOptionsHandler OptionsHandler;

	OptionsHandler.ParseOptions(argc, argv);
	const MetricOptions& metricOptions = *(OptionsHandler.getMetricOptions());

	GlobalFunctionLocator srcMap(*(OptionsHandler.getMetricOptions()));

	llvm::sys::PrintStackTraceOnErrorSignal();
		
	clang::tooling::ClangTool Tool(OptionsHandler.getOptionsParser()->getCompilations(), 
		                           OptionsHandler.getOptionsParser()->getSourcePathList());

	// First tool-run to gather metrics from the AST.  This is done separately from the second tool-rum
	// as different pre-processor options are used
	int Result = Tool.run(newASTMetricFrontendActionFactory(metricOptions, &topUnit, &srcMap, &commentFileList));

	// Success?
	if( Result == 0 )
	{
		// Now that all TUs have been processed, try to resolve function references which couldn't be 
		//  resolved during the processing of individual translation units
		resolveLinkages( &topUnit );

		// Second tool run to gather metrics from the pre-processor.  This is performed after the AST
		//  generation as the details of the function locations gathered from the AST are used
		//  for determining whether or not a function should be included
		Result = Tool.run(newPPMetricFrontendActionFactory(metricOptions, &topUnit, &srcMap, true));

		if (Result == 0)
		{
			Result = Tool.run(newPPMetricFrontendActionFactory(metricOptions, &topUnit, &srcMap, false));
		}

		// Success?
		if( Result == 0 )
		{
			// Time to dump the results
			if (metricOptions.getDumpFnMap())
			{
				srcMap.dump( std::cout );
			}

			MetricDumper::dump(std::cout, &topUnit, metricOptions);
		}
		else
		{
			std::cout << "Tool.run returned " << Result << std::endl;
		}
	}
	else
	{
		std::cout << "Tool.run returned " << Result << std::endl;
	}

	return Result;
}
