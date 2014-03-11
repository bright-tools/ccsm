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

MetricOptions::MetricOptions( std::vector<std::string>* const p_excludeFiles, std::vector<std::string>* const p_excludeFunctions )
	: ExcludeFiles( p_excludeFiles ), ExcludeFunctions( p_excludeFunctions )
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

