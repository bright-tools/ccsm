/*
   Copyright 2016 John Bailey

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

#include "LimitLibrary.hpp"
#include "minicsv.h"

LimitLibrary::LimitLibrary()
{
}

#include <iostream>

bool LimitLibrary::load( const std::string p_fileName )
{
    bool ret_val = false;
    csv::ifstream is(p_fileName.c_str());
    is.enable_trim_quote_on_str(true,'"');

    if( is.is_open() )
    {
        std::string  metricName;
		limitPattern_t pattern;

		while (is.read_line())
		{
			is >> metricName >> pattern.fileName >> pattern.funcName >> pattern.operand >> pattern.limit;
#if 0
			std::cout << "Read: " << metricName << " / " << pattern.fileName << " / " << pattern.funcName << " / " << pattern.operand << " / " << pattern.limit << std::endl;
#endif
			/* TODO: ignore comments */
			/* TODO: deal with scaling */

			MetricType_e metric = MetricUnit::getMetricByShortName(metricName);

			if (metric != METRIC_TYPE_MAX)
			{
#if 0
				std::cout << "Metric is " << metric << std::endl;
#endif
				m_patternMap[metric].push_back(pattern);
			}
		}

        ret_val = true; 
    }

    return ret_val;
}

void LimitLibrary::checkLimit(const MetricUnit& p_unit, const MetricOptions& p_options) const
{
	unsigned loop;
	for (loop = 0;
		loop < METRIC_TYPE_MAX;
		loop++)
	{
		const MetricType_e metric = static_cast<MetricType_e>(loop);

		patternMap_t::const_iterator it = m_patternMap.find(metric);
		
		/* Are there any limits against this metric? */
		if (it != m_patternMap.end())
		{
			// TODO: Check for match

			// TODO: Check against limit
		}
	}
}