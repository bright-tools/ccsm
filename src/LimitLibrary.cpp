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
#include "MetricOptions.hpp"

#include <llvm/Support/Regex.h>
#include <llvm/ADT/StringRef.h>
#include <iostream>

static const std::set<std::string> knownOperators = { ">", "<", "<=", ">=" };

LimitLibrary::LimitLibrary()
{
}

void LimitLibrary::parseCsvLine(csv::ifstream& p_is)
{
	std::string  metricName;
	limitPattern_t pattern;
	std::string  global;
	std::string  file;
	float        limit;

	p_is >> metricName >> global >> file >> pattern.fileName >> pattern.funcName >> pattern.operand >> limit >> pattern.text;
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
	
	if (metricName[0] != ';')
	{
		MetricType_e metric = MetricUnit::getMetricByShortName(metricName);

		if (metric != METRIC_TYPE_MAX)
		{
#if 0
			std::cout << "Metric is " << metric << std::endl;
#endif
			if ( knownOperators.find( pattern.operand ) == knownOperators.end() )
			{
				std::cerr << "Unknown operator in limits file '" << pattern.operand << "'\n";
				exit(EXIT_FAILURE);
			}

			pattern.limit = limit * MetricUnit::getMetricScaling(metric);

			m_patternMap[metric].push_back(pattern);
		}
		else
		{
			std::cerr << "Ignoring limits file line starting " << metricName << "\n";
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		/* It's a comment line - ignore */
	}

}

bool LimitLibrary::load( const std::string p_fileName )
{
    bool ret_val = false;
    csv::ifstream is(p_fileName.c_str());
    is.enable_trim_quote_on_str(true,'"');
	is.enable_terminate_on_blank_line(false);

    if( is.is_open() )
    {
		while (is.read_line())
		{
			parseCsvLine(is);
		}

        ret_val = true; 
    }

    return ret_val;
}

const LimitLibrary::limitPattern_t* LimitLibrary::findHighestPresidenceRule(const patternSet_t& p_set, const MetricUnit& p_unit, const MetricOptions& p_options) const
{
	const limitPattern_t* pattern = NULL;

	for (patternSet_t::const_iterator pit = p_set.begin();
		pit != p_set.end();
		pit++)
	{
		if (unitMatchesLimitPattern(p_unit, p_options, &(*pit)))
		{
			pattern = &(*pit);
		}
	}

	return pattern;
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
			
			/* Are there any limits against this metric at all? */
			if (it != m_patternMap.end())
			{
				/* Find the highest precident rule where the rule pattern matches this unit (if there is one) */
				pattern = findHighestPresidenceRule(it->second, p_unit, p_options);

				checkUnitPassesMetricLimit(p_unit, p_options, metric, pattern);
			}
		}
	}
}

void LimitLibrary::checkUnitPassesMetricLimit(const MetricUnit& p_unit, const MetricOptions& p_options, const MetricType_e p_metric, const limitPattern_t* const p_pattern)
{
	if (p_pattern != NULL)
	{
		MetricUnit::counter_t val = p_unit.getCounter(p_metric, MetricUnit::isMetricCumulative(p_metric));

#if 0
		std::cout << "Checking " << p_unit.getUnitName(p_options) << " for '" << MetricUnit::getMetricName(it->first) << "'\n";
#endif

		if (((p_pattern->operand == "<") && (val < p_pattern->limit)) ||
			((p_pattern->operand == ">") && (val > p_pattern->limit)) ||
			((p_pattern->operand == ">=") && (val >= p_pattern->limit)) ||
			((p_pattern->operand == "<=") && (val <= p_pattern->limit)))
		{
			/* Passed check */
		}
		else
		{
			std::ostream& out = p_options.getOutput();
			// TODO: Qualify the unitName - is it a function?  Is it a file?  Global?
			out << p_unit.getUnitName(p_options) << " failed limits check '" << MetricUnit::getMetricName(p_metric) << "' (actual: "
			    << MetricUnit::getScaledMetricString(p_metric,val) << " expected: " << p_pattern->operand
				<< MetricUnit::getScaledMetricString(p_metric, p_pattern->limit) << ")";

			if (p_pattern->text.length())
			{
				out << ": " << p_pattern->text;
			}
			out << "\n";

		}
	}
}

bool LimitLibrary::unitMatchesLimitPattern(const MetricUnit& p_unit, const MetricOptions& p_options, const limitPattern_t* const p_pattern)
{
	bool matches = false;
	switch (p_unit.GetType())
	{
		case METRIC_UNIT_GLOBAL:
			if (p_pattern->global)
			{
				matches = true;
			}
			break;
		case METRIC_UNIT_FILE:
			if (p_pattern->file)
			{
				llvm::Regex regex(p_pattern->fileName);
				if (regex.match(p_unit.getUnitName(p_options)))
				{
					matches = true;
				}
			}
			break;
		case METRIC_UNIT_FUNCTION:
			if (p_pattern->funcName.length())
			{
				llvm::Regex regex(p_pattern->funcName);
				if (regex.match(p_unit.getUnitName(p_options)))
				{
					llvm::Regex fileRegex(p_pattern->fileName);
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
	return matches;
}
