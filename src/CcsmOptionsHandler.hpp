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

#if !defined( CCSM_OPTIONS_HANDLER_HPP )
#define       CCSM_OPTIONS_HANDLER_HPP

#include "clang/Tooling/CommonOptionsParser.h"
#include "MetricOptions.hpp"

class CcsmOptionsHandler {

private:
	clang::tooling::CommonOptionsParser* m_optionsParser;
	MetricOptions* m_metricOptions;

public:

	CcsmOptionsHandler();

	void ParseOptions(int argc, const char ** const argv);

	const MetricOptions* getMetricOptions() const;
	clang::tooling::CommonOptionsParser* getOptionsParser() const;

	virtual ~CcsmOptionsHandler();
};

#endif
