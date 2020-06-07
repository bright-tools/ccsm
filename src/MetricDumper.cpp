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

#include "MetricDumper.hpp"
#include "MetricOptions.hpp"

#define IS_OUTPUT_TREE_FORM( _fmt ) (((_fmt) == METRIC_DUMP_FORMAT_TREE ) || ((_fmt) == METRIC_DUMP_FORMAT_SPARSE_TREE ))

const std::string MetricDumper::m_namePrefix[METRIC_UNIT_MAX] = {
	"",
	"File: ",
	"\tFunction: ",
	"\tMethod: "
};
const std::string MetricDumper::m_dumpPrefix[METRIC_UNIT_MAX] = {
	"",
	"\t",
	"\t\t",
	"\t\t"
};

void MetricDumper::dumpMetric(std::ostream& out, const MetricUnit* const p_unit, const MetricType_e p_metric, const MetricDumpFormat_e p_fmt, const std::string& p_sep, const bool p_recurse, const std::string& p_suffix,bool p_useShortNames)
{
	MetricUnit::counter_t val = p_unit->getCounter(p_metric, p_recurse);
	const MetricUnitType_e metricType = p_unit->GetType();

	if ((p_fmt != METRIC_DUMP_FORMAT_SPARSE_TREE) ||
		((val != 0) && ( val <= MetricUnit::counter_t_Max )))
	{
		if (IS_OUTPUT_TREE_FORM(p_fmt)) {
			std::string metricName;

			if (p_useShortNames)
			{
				metricName = MetricUnit::getMetricShortName(p_metric);
			}
			else
			{
				metricName = MetricUnit::getMetricName(p_metric);
			}

			out << m_dumpPrefix[metricType] << metricName << p_suffix << ": ";
		}

		out << MetricUnit::getScaledMetricString(p_metric, val) << p_sep;
	}
}

void MetricDumper::dump(const MetricUnit* const p_topLevel, const MetricOptions& p_options)
{
	const MetricDumpFormat_e p_fmt = p_options.getOutputFormat();

	if (p_fmt != METRIC_DUMP_FORMAT_NONE)
	{
		std::string sep;
		const MetricUnitType_e metricUnitType = p_topLevel->GetType();
		const bool useShortNames = p_options.getUseShortNames();
		std::ostream& out = p_options.getOutput();

		if (p_options.getOutputMetric(metricUnitType))
		{
			switch (p_fmt) {
			case METRIC_DUMP_FORMAT_TREE:
			case METRIC_DUMP_FORMAT_SPARSE_TREE:
				sep = "\n";
				break;
			case METRIC_DUMP_FORMAT_TSV:
				sep = "\t";
				break;
			case METRIC_DUMP_FORMAT_CSV:
				sep = ",";
				break;
			default:
				break;
			}

			if ((p_fmt == METRIC_DUMP_FORMAT_TSV) ||
				(p_fmt == METRIC_DUMP_FORMAT_CSV)) {

				if (metricUnitType == METRIC_UNIT_GLOBAL)
				{
				out << "File" << sep << "Name" << sep;
					int loop;
					for (loop = 0;
						loop < METRIC_TYPE_MAX;
						loop++)
					{
						const MetricType_e metric = static_cast<MetricType_e>(loop);
						const bool localAndCumulativeOutputs = MetricUnit::isMetricCumulative(metric) && MetricUnit::isMetricLocalAndCumulative(metric);
						std::string quote_wrapper = "";

						if (p_fmt == METRIC_DUMP_FORMAT_CSV)
						{
							quote_wrapper = "\"";
						}

						/* TODO: Output a warning in the case that anything in p_options.OutputMetrics isn't understood */
						if (p_options.ShouldIncludeMetric(metric))
						{
							std::string metricName;

							if (useShortNames)
							{
								metricName = MetricUnit::getMetricShortName(metric);
							}
							else
							{
								metricName = MetricUnit::getMetricName(metric);
							}

							out << quote_wrapper << metricName;

							if (localAndCumulativeOutputs)
							{
								out << "(local)" << quote_wrapper << sep << quote_wrapper << metricName << "(cumulative)";
							}
							out << quote_wrapper << sep;
						}
					}
					out << std::endl;
				}
			}

			if (IS_OUTPUT_TREE_FORM(p_fmt)) {
				out << m_namePrefix[metricUnitType];
			}

		if ((p_fmt == METRIC_DUMP_FORMAT_TSV) ||
			(p_fmt == METRIC_DUMP_FORMAT_CSV)) 
		{
			if (metricUnitType == METRIC_UNIT_FUNCTION)
			{
				out << p_topLevel->getParent()->getUnitName(p_options) << sep;
			}
		}

			out << p_topLevel->getUnitName(p_options) << sep;

		if ((p_fmt == METRIC_DUMP_FORMAT_TSV) ||
			(p_fmt == METRIC_DUMP_FORMAT_CSV))
		{
			if (metricUnitType != METRIC_UNIT_FUNCTION)
			{
				out << sep;
			}
		}

			unsigned loop;
			for (loop = 0;
				loop < METRIC_TYPE_MAX;
				loop++)
			{
				const MetricType_e metric = static_cast<MetricType_e>(loop);
				const bool metricIsCumulative = MetricUnit::isMetricCumulative(metric);
				/* TODO: duped above */
				const bool localAndCumulativeOutputs = metricIsCumulative && MetricUnit::isMetricLocalAndCumulative(metric);

				if (p_options.ShouldIncludeMetric(metric))
				{
					/* Filter out metrics which only apply at file/method level */
					if (MetricUnit::doesMetricApplyForUnit(metric, metricUnitType))
					{
						std::string sfx = "";
						if (localAndCumulativeOutputs)
						{
							if (metricUnitType == METRIC_UNIT_FILE)
							{
								dumpMetric(out, p_topLevel, (MetricType_e)loop, p_fmt, sep, false, "(local)", useShortNames);
								sfx = "(cumulative)";
							}
							else if ((p_fmt == METRIC_DUMP_FORMAT_TSV) ||
								(p_fmt == METRIC_DUMP_FORMAT_CSV))
							{
								out << sep;
							}

						}
						dumpMetric(out, p_topLevel, (MetricType_e)loop, p_fmt, sep, metricIsCumulative, sfx, useShortNames);
					}
					else if ((p_fmt == METRIC_DUMP_FORMAT_TSV) ||
						(p_fmt == METRIC_DUMP_FORMAT_CSV))
					{
						out << sep;
						if (localAndCumulativeOutputs)
						{
							out << sep;
						}
					}
				}
			}

			if ((p_fmt == METRIC_DUMP_FORMAT_TSV) ||
				(p_fmt == METRIC_DUMP_FORMAT_CSV)) {
				out << std::endl;
			}
		}

		const MetricUnit::SubUnitMap_t* const subUnits = p_topLevel->getSubUnits();

		for (MetricUnit::SubUnitMap_t::const_iterator unitIt = subUnits->begin();
			unitIt != subUnits->end();
			++unitIt)
		{
			dump((*unitIt).second, p_options);
		}
	}
}

