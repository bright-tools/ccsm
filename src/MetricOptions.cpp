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
#include <iostream>
#include <fstream>

MetricOptions::MetricOptions( std::vector<std::string>* const p_excludeFiles, 
							  std::vector<std::string>* const p_excludeFunctions, 
							  std::set<MetricType_e>          p_outputMetrics,
							  std::vector<std::string>* const p_defFiles )
	: ExcludeFiles( p_excludeFiles ), ExcludeFunctions( p_excludeFunctions ), 
	  OutputMetrics( p_outputMetrics ), DefFiles( p_defFiles ),
	  m_dumpTokens( false ),
	  m_dumpAST( false ),
	  m_useShortNames( false ),
	  m_useAbsoluteFileNames( false ),
	  m_outputStream(&std::cout)
{
}

MetricOptions::~MetricOptions()
{
	if (m_outputStream != &std::cout)
	{
		delete(m_outputStream);
	}
}

#include <llvm/Support/Regex.h>
#include <llvm/ADT/StringRef.h>

bool MetricOptions::isFileInList( const std::vector<std::string>* const p_list, const std::string& p_name ) const
{
	bool ret_val = false;
	if ( p_list != NULL )
	{	
		for( std::vector<std::string>::const_iterator it = p_list->begin();
			 it != p_list->end();
			 it++ )
		{
			llvm::Regex regex(*it);
			if( regex.match(p_name))
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

bool MetricOptions::ShouldIncludeFile( const std::string& p_fn )
{
	bool ret_val;
	std::map<std::string, bool>::const_iterator fnd;

	/* See if we already checked this file and if so, use that result.  If not,
	   check to see if the file is in the exclude list
	   TODO: This caching mechanism will not take into account changes to ExcludeFiles */
	fnd = m_shouldIncludeFileCache.find(p_fn);

	if (fnd == m_shouldIncludeFileCache.end())
	{
		ret_val = !isFileInList(ExcludeFiles, p_fn);
		m_shouldIncludeFileCache[p_fn] = ret_val;
	}
	else
	{
		ret_val = fnd->second;
	}
	return ret_val;
}

bool MetricOptions::ShouldIncludeFunction( const std::string& p_fn ) const
{
	return (( ExcludeFunctions == NULL ) ||
			( std::find( ExcludeFunctions->begin(), ExcludeFunctions->end(), p_fn ) == ExcludeFunctions->end() ));
}

bool MetricOptions::ShouldIncludeMetric(MetricType_e p_metric) const
{
	return OutputMetrics.find(p_metric) != OutputMetrics.end();
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

bool MetricOptions::optionsOk(void) const
{
	return m_optionsOk;
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

bool MetricOptions::setOutputFile(const std::string p_fileName)
{
	bool retVal = false;
	/* If the user specified a file to sent the output to, try and open it and set the output
	   stream point to that handle */
	if (p_fileName.length())
	{
		std::ofstream* outputFile = new std::ofstream();
		outputFile->open(p_fileName);
		if (outputFile->is_open())
		{
			m_outputStream = outputFile;
			m_outputFileName = p_fileName;
			retVal = true;
		}
		else
		{
			std::cerr << "Failed to open output file '" << p_fileName << "'\n";
			m_optionsOk = false;
		}
	}

	return retVal;
}

std::ostream& MetricOptions::getOutput(void) const
{
	return *m_outputStream;
}

std::string MetricOptions::getOutputFile(void) const
{
	return m_outputFileName;
}

void MetricOptions::setUsePrefix(const bool p_prefix)
{
	m_usePrefix = p_prefix;
}

bool MetricOptions::getUsePrefix() const
{
	return m_usePrefix;
}

void MetricOptions::setLimitsFile(const std::vector<std::string>& p_fileName)
{
	m_limitsFileNames = p_fileName;
}

const std::vector<std::string>& MetricOptions::getLimitsFiles(void) const
{
	return m_limitsFileNames;
}

void MetricOptions::setGroupLimitWarningsByFile(const bool p_groupByFile)
{
	m_groupLimitWarningsByFile = p_groupByFile;
}

bool MetricOptions::getGroupLimitWarningsByFile() const
{
	return m_groupLimitWarningsByFile;
}