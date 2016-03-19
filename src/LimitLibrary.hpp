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

#if !defined( LIMITS_LIBRARY_HPP )
#define       LIMITS_LIBRARY_HPP

#include "MetricUnit.hpp"
#include <map>
#include <list>

class LimitLibrary
{
    protected:
        typedef struct {
			bool                  global;
			bool                  file;
			std::string           fileName;
            std::string           funcName;
			std::string           operand;
			std::string           text;
            MetricUnit::counter_t limit;
        } limitPattern_t;

		typedef std::list<limitPattern_t> patternSet_t;
        typedef std::map<MetricType_e,patternSet_t> patternMap_t;

        patternMap_t m_patternMap;

    public:
        LimitLibrary();
        bool load( const std::string p_fileName );

		void checkLimit(const MetricUnit& p_unit, const MetricOptions& p_options) const;
};

#endif
