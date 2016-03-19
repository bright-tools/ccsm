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

#include <llvm/Support/Regex.h>
#include <llvm/ADT/StringRef.h>

LimitLibrary::LimitLibrary()
{
}

#include <iostream>

bool LimitLibrary::load( const std::string p_fileName )
{
    bool ret_val = false;
    csv::ifstream is(p_fileName.c_str());
    is.enable_trim_quote_on_str(true,'"');
	is.enable_terminate_on_blank_line(false);

    if( is.is_open() )
    {
        std::string  metricName;
		limitPattern_t pattern;
		std::string  global;
		std::string  file;

		while (is.read_line())
		{
			is >> metricName >> global >> file >> pattern.fileName >> pattern.funcName >> pattern.operand >> pattern.limit >> pattern.text;
#if 0
			std::cout << "Read: " << metricName << " / " << pattern.fileName << " / " << pattern.funcName << " / " << pattern.operand << " / " << pattern.limit << std::endl;
#endif

			if (global.length())
			{
				pattern.global = true;
			}
			else
			{
				pattern.global = false;
			}

			if (file.length())
			{
				pattern.file = true;
			}
			else
			{
				pattern.file = false;
			}

			/* TODO: deal with scaling */
			/* TODO: deal with unknown operands */

			if (metricName[0] != ';')
			{
				MetricType_e metric = MetricUnit::getMetricByShortName(metricName);

				if (metric != METRIC_TYPE_MAX)
				{
#if 0
					std::cout << "Metric is " << metric << std::endl;
#endif
					m_patternMap[metric].push_back(pattern);
				}
				else
				{
					std::cerr << "Ignoring limits file line starting " << metricName << "\n";
					// TODO: error
				}
			}
			else
			{
				/* It's a comment line - ignore */
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

		if (MetricUnit::doesMetricApplyForUnit(metric,p_unit.GetType()))
		{
			patternMap_t::const_iterator it = m_patternMap.find(metric);
			const limitPattern_t* pattern = NULL;
			
			/* Are there any limits against this metric? */
			if (it != m_patternMap.end())
			{
				for (patternSet_t::const_iterator pit = it->second.begin();
					pit != it->second.end();
					pit++)
				{
					bool matches = false;

					switch (p_unit.GetType())
					{
					case METRIC_UNIT_GLOBAL:
						if (pit->global)
						{
							matches = true;
						}
						break;
					case METRIC_UNIT_FILE:
						if (pit->file)
						{
							llvm::Regex regex(pit->fileName);
							if (regex.match(p_unit.getUnitName(p_options)))
							{
								matches = true;
							}
						}
						break;
					case METRIC_UNIT_FUNCTION:
						if (pit->funcName.length())
						{
							llvm::Regex regex(pit->funcName);
							if (regex.match(p_unit.getUnitName(p_options)))
							{
								llvm::Regex fileRegex(pit->fileName);
								if (fileRegex.match(p_unit.getParent()->getUnitName(p_options)))
								{
									matches = true;
								}
							}
						}
						break;
					default:
						break;
					}

					if (matches)
					{
						pattern = &(*pit);
					}
				}


				if (pattern != NULL)
				{
					MetricUnit::counter_t val = p_unit.getCounter(metric, MetricUnit::isMetricCumulative(metric));

#if 0
					std::cout << "Checking " << p_unit.getUnitName(p_options) << " for '" << MetricUnit::getMetricName(it->first) << "'\n";
#endif

					if (((pattern->operand == "<") && (val < pattern->limit)) ||
						((pattern->operand == ">") && (val > pattern->limit)) ||
						((pattern->operand == ">=") && (val >= pattern->limit)) ||
						((pattern->operand == "<=") && (val <= pattern->limit)))
					{
						/* Passed check */
					}
					else
					{
						// TODO: Qualify the unitName - is it a function?  Is it a file?  Global?
						// TODO: Taking scaling into account on actual vs limit
						std::cout << p_unit.getUnitName(p_options) << " failed limits check '" << MetricUnit::getMetricName(it->first) << "' (actual: " << val << " expected: " << pattern->operand << pattern->limit << ")";
						if (pattern->text.length())
						{
							std::cout << ": " << pattern->text;
						}
						std::cout << "\n";
					}
				}
			}
		}
	}
}