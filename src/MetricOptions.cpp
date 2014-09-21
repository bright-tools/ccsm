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

#include "MetricOptions.hpp"

MetricOptions::MetricOptions( std::vector<std::string>* const p_excludeFiles, std::vector<std::string>* const p_excludeFunctions, std::vector<std::string>* const p_outputMetrics )
	: ExcludeFiles( p_excludeFiles ), ExcludeFunctions( p_excludeFunctions ), OutputMetrics( p_outputMetrics ), m_dumpTokens( false )
{
}

MetricOptions::~MetricOptions()
{
}

bool MetricOptions::ShouldIncludeFile( const std::string& p_fn ) const
{
	bool ret_val = true;
	if ( ExcludeFiles != NULL )
	{	
		for( std::vector<std::string>::const_iterator it = ExcludeFiles->begin();
			 it != ExcludeFiles->end();
			 it++ )
		{
			if( p_fn.find( *it ) !=  std::string::npos )
			{
				ret_val = false;
				break;
			}
		}
	}
	return ret_val;
}

bool MetricOptions::ShouldIncludeFunction( const std::string& p_fn ) const
{
	return (( ExcludeFunctions == NULL ) ||
			( std::find( ExcludeFunctions->begin(), ExcludeFunctions->end(), p_fn ) == ExcludeFunctions->end() ));
}

bool MetricOptions::ShouldIncludeMetric( const std::string& p_name ) const
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
		     it != OutputMetrics->end();
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

			if( ret_val )
			{
				break;
			}
		}
	}
	return ret_val;
}

void MetricOptions::setDumpTokens( const bool p_dump )
{
	m_dumpTokens = p_dump;
}

bool MetricOptions::getDumpTokens( void ) const
{
	return m_dumpTokens;
}
