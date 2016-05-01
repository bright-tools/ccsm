/*
   @file
   @brief 

   @author John Bailey
   @copyright Copyright 2016 John Bailey

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

#if !defined( LIMITS_CHECKER_HPP )
#define       LIMITS_CHECKER_HPP

#include "MetricUnit.hpp"
#include "MetricOptions.hpp"
#include "LimitLibrary.hpp"

class LimitsChecker
{
    protected:
		static std::string check(const MetricUnit* const p_topLevel, const MetricOptions& p_options, const LimitLibrary& p_limits);
    public:
        static void dump(const MetricUnit* const p_topLevel, const MetricOptions& p_options );
		static void dump(const MetricUnit* const p_topLevel, const MetricOptions& p_options, const LimitLibrary& p_limits);
};

#endif
