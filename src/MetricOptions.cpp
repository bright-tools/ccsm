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

#include "MetricUnit.hpp"
#include "MetricOptions.hpp"

#include <algorithm>

static const std::pair<std::string, std::string> metricAliasListData[] =
{
#define METRIC_ALIAS( _name, _alias ) std::make_pair( _name, _alias ),
#define METRIC( _enum, _short_name, _long_name, _applies_global, _applies_file, _applies_function, _applies_method, _cumulative, _multipass, _report_local, _scaling, _description  )
#include "metrics.def"
#undef  METRIC
#undef  METRIC_ALIAS
};
static const std::set<std::pair<std::string, std::string>> metricAliasList(metricAliasListData,
	metricAliasListData + sizeof metricAliasListData / sizeof metricAliasListData[0]);

MetricOptions::MetricOptions( std::vector<std::string>* const p_excludeFiles, 
							  std::vector<std::string>* const p_excludeFunctions, 
							  std::vector<std::string>* const p_outputMetrics, 
							  std::vector<std::string>* const p_defFiles )
	: ExcludeFiles( p_excludeFiles ), ExcludeFunctions( p_excludeFunctions ), 
	  OutputMetrics( p_outputMetrics ), DefFiles( p_defFiles ),
	  m_dumpTokens( false ),
	  m_dumpAST( false ),
	  m_useShortNames( false ),
	  m_useAbsoluteFileNames( false )
{
}

MetricOptions::~MetricOptions()
{
}

bool MetricOptions::isFileInList( const std::vector<std::string>* const p_list, const std::string& p_name ) const
{
	bool ret_val = false;
	if ( p_list != NULL )
	{	
		for( std::vector<std::string>::const_iterator it = p_list->begin();
			 it != p_list->end();
			 it++ )
		{
			if( p_name.find( *it ) !=  std::string::npos )
			{
				ret_val = true;
				break;
			}
		}
	}
	return ret_val;
}

bool MetricOptions::isDefFile( const std::string& p_fn ) const
{
	return isFileInList( DefFiles, p_fn );
}

bool MetricOptions::ShouldIncludeFile( const std::string& p_fn ) const
{
	return !isFileInList( ExcludeFiles, p_fn );
}

bool MetricOptions::ShouldIncludeFunction( const std::string& p_fn ) const
{
	return (( ExcludeFunctions == NULL ) ||
			( std::find( ExcludeFunctions->begin(), ExcludeFunctions->end(), p_fn ) == ExcludeFunctions->end() ));
}

bool MetricOptions::ShouldIncludeMetric( const std::string& p_name, bool p_checkAliases ) const
{
	bool ret_val = false;

	if(( OutputMetrics == NULL ) ||
	   ( OutputMetrics->size() == 0 ))
	{
		ret_val = true;
	}
	else
	{
		for( std::vector<std::string>::const_iterator it = OutputMetrics->begin();
		     (it != OutputMetrics->end()) && !ret_val;
			 it++ )
		{
			size_t len = (*it).length();
			size_t lmo = len - 1;

			/* Check to see if last character is a wildcard */
			if(( len > 0 ) && ((*it)[lmo] == '*' ))
			{
				if( p_name.substr( 0, lmo ) == (*it).substr( 0, lmo ))
				{
					ret_val = true;
				}
			}
			else if( (*it) == p_name )
			{
				ret_val = true;
			}
		}

		/* Check against the list of aliases for the metric, if necessary */
		if (!ret_val && p_checkAliases)
		{
			for (std::set<std::pair<std::string, std::string>>::const_iterator it = metricAliasList.begin();
				(it != metricAliasList.end()) && !ret_val;
				it++)
			{
				if (it->first == p_name)
				{
					ret_val = ShouldIncludeMetric(it->second,false); 
				}
			}
		}
	}
	return ret_val;
}

void MetricOptions::setDumpTokens( const bool p_dump )
{
	m_dumpTokens = p_dump;
}

void MetricOptions::setDumpAST(const bool p_dump)
{
	m_dumpAST = p_dump;
}

bool MetricOptions::getDumpTokens( void ) const
{
	return m_dumpTokens;
}

bool MetricOptions::getDumpAST(void) const
{
	return m_dumpAST;
}

void MetricOptions::setUseShortNames(const bool p_shortNames)
{
	m_useShortNames = p_shortNames;
}

bool MetricOptions::getUseShortNames(void) const
{
	return m_useShortNames;
}

void MetricOptions::setPrototypesAreFileScope(const bool p_proto)
{
	m_prototypesAreFileScope = p_proto;
}

bool MetricOptions::getPrototypesAreFileScope(void) const
{
	return m_prototypesAreFileScope;
}

void MetricOptions::setUseAbsoluteFileNames(const bool p_absoluteFn)
{
	m_useAbsoluteFileNames = p_absoluteFn;
}

bool MetricOptions::getUseAbsoluteFileNames(void) const
{
	return m_useAbsoluteFileNames;
}

void MetricOptions::setOutputFormat(const MetricDumpFormat_e p_outputFormat)
{
	m_outputFormat = p_outputFormat;
}

MetricDumpFormat_e MetricOptions::getOutputFormat(void) const
{
	return m_outputFormat;
}

void MetricOptions::setOutputMetric(const MetricUnitType_e p_metric, const bool p_output)
{
	m_outputMetric[p_metric] = p_output;
}

bool MetricOptions::getOutputMetric(const MetricUnitType_e p_metric) const
{
	return m_outputMetric[p_metric];
}

void MetricOptions::setDumpFnMap(const bool p_dump)
{
	m_dumpFnMap = p_dump;
}

bool MetricOptions::getDumpFnMap(void) const
{
	return m_dumpFnMap;
}

void MetricOptions::setExcludeStdHeaders(const bool p_exclude)
{
	m_excludeStdHeaders = p_exclude;
}

bool MetricOptions::getExcludeStdHeaders(void) const
{
	return m_excludeStdHeaders;
}