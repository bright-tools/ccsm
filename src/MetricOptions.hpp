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
#if !defined( METRIC_OPTIONS_HPP )
#define       METRIC_OPTIONS_HPP

#include "MetricUnit.hpp"

#include <vector>
#include <string>

typedef enum
{
	METRIC_DUMP_FORMAT_TREE,
	METRIC_DUMP_FORMAT_SPARSE_TREE,
	METRIC_DUMP_FORMAT_TSV,
	METRIC_DUMP_FORMAT_CSV
} MetricDumpFormat_e;

class MetricOptions
{
protected:
	std::vector<std::string>* ExcludeFiles;
	std::vector<std::string>* ExcludeFunctions;
	std::vector<std::string>* OutputMetrics;
	std::vector<std::string>* DefFiles;
	bool m_dumpTokens;
	bool m_dumpAST;
	bool m_useShortNames;
	bool m_declarationsAreNotFileScope;
	bool m_prototypesAreFileScope;
	bool m_useAbsoluteFileNames;
	bool m_dumpFnMap;
	bool m_excludeStdHeaders;
	MetricDumpFormat_e m_outputFormat;
	bool m_outputMetric[METRIC_UNIT_MAX];

	bool isFileInList( const std::vector<std::string>* const p_list, const std::string& p_name ) const;
public:
	MetricOptions( std::vector<std::string>* const p_excludeFiles = NULL, 
		           std::vector<std::string>* const p_excludeFunctions = NULL,
				   std::vector<std::string>* const p_outputMetrics = NULL,
				   std::vector<std::string>* const p_defFiles = NULL );
	virtual ~MetricOptions();
	bool ShouldIncludeFile( const std::string& p_fn ) const;
	bool ShouldIncludeFunction( const std::string& p_fn ) const;
	bool ShouldIncludeMetric(const std::string& p_name, bool p_checkAliases = true) const;
	bool isDefFile( const std::string& p_name ) const;
	void setDumpTokens( const bool p_dump );
	bool getDumpTokens( void ) const;
	void setDumpAST(const bool p_dump);
	bool getDumpAST(void) const;
	void setUseShortNames(const bool p_shortNames);
	bool getUseShortNames(void) const;
	void setPrototypesAreFileScope(const bool p_proto);
	bool getPrototypesAreFileScope(void) const;
	void setUseAbsoluteFileNames(const bool p_absoluteFn);
	bool getUseAbsoluteFileNames(void) const;
	void setOutputFormat(const MetricDumpFormat_e p_outputFormat);
	MetricDumpFormat_e getOutputFormat(void) const;
	void setOutputMetric(const MetricUnitType_e p_metric, const bool p_output);
	bool getOutputMetric(const MetricUnitType_e p_metric) const;
	void setDumpFnMap(const bool p_dump);
	bool getDumpFnMap(void) const;
	void setExcludeStdHeaders(const bool p_exclude);
	bool getExcludeStdHeaders(void) const;
};

#endif
