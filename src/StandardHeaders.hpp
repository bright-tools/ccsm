/*
   @file
   @brief Functions to support determining the appropriate standard
          headers based on a particular language standard

   @author John Bailey
   @copyright Copyright 2022 John Bailey

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

#if !defined STANDARD_HEADERS_HPP
#define STANDARD_HEADERS_HPP

#include <string>
#include <vector>

typedef enum
{
    STD_FIRST,
    STD_C89 = STD_FIRST,
    STD_C11,
    STD_C99,
    STD_CPP,
    STD_COUNT
} LangStd_e;

void AppendStandardHeaders(const bool usedStandards[STD_COUNT],
                           std::vector<std::string> &receivingVector);

#endif