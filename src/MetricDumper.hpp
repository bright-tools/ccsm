/*
@file
@brief Class to support outputting of metrics relating to C code

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

#if !defined( METRIC_DUMPER_HPP )
#define       METRIC_DUMPER_HPP

#include "MetricUnit.hpp"
#include "MetricOptions.hpp"

#include <iostream>

class MetricDumper
{
protected:
	static const std::string m_dumpPrefix[METRIC_UNIT_MAX];
	static const std::string m_namePrefix[METRIC_UNIT_MAX];

	static void dumpMetric(std::ostream& out, const MetricUnit* const p_unit, const MetricType_e p_metric, const MetricDumpFormat_e p_fmt, const std::string& p_sep, const bool p_recurse, const std::string& p_suffix, const bool p_useShortNames);
public:
	static void dump(const MetricUnit* const p_topLevel, const MetricOptions& p_options );
};

#endif
