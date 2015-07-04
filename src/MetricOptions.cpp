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

MetricOptions::MetricOptions( std::vector<std::string>* const p_excludeFiles, 
							  std::vector<std::string>* const p_excludeFunctions, 
							  std::vector<std::string>* const p_outputMetrics, 
							  std::vector<std::string>* const p_defFiles )
	: ExcludeFiles( p_excludeFiles ), ExcludeFunctions( p_excludeFunctions ), 
	  OutputMetrics( p_outputMetrics ), DefFiles( p_defFiles ),
	  m_dumpTokens( false ),
	  m_dumpAST( false ),
	  m_useShortNames( false )
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
