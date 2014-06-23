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

#include <vector>
#include <string>

class MetricOptions
{
protected:
	std::vector<std::string>* ExcludeFiles;
	std::vector<std::string>* ExcludeFunctions;
	std::vector<std::string>* OutputMetrics;
public:
	MetricOptions( std::vector<std::string>* const p_excludeFiles = NULL, 
		           std::vector<std::string>* const p_excludeFunctions = NULL,
				   std::vector<std::string>* const p_outputMetrics = NULL );
	virtual ~MetricOptions();
	bool ShouldIncludeFile( const std::string& p_fn ) const;
	bool ShouldIncludeFunction( const std::string& p_fn ) const;
	bool ShouldIncludeMetric( const std::string& p_name ) const;
};

#define SHOULD_INCLUDE_FILE( _m, _f ) (((_m)==NULL) || ((_m)->ShouldIncludeFile( _f )))
#define SHOULD_INCLUDE_FUNCTION( _m, _f ) (((_m)==NULL) || ((_m)->ShouldIncludeFunction( _f )))
#define SHOULD_INCLUDE_METRIC( _m, _t ) (((_m)==NULL) || ((_m)->ShouldIncludeMetric( _t )))

#endif
