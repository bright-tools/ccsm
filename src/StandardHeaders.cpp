/*

   Copyright 2022 John Bailey

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

#include "StandardHeaders.hpp"

#include "llvm/Support/Path.h"

#include <map>
#include <set>
#include <string>
#include <vector>

const std::string sep = "[\\\\/^]";
const std::string end = "$";

static const std::set<std::string> c89_std_headers = {
    sep + "assert.h" + end, sep + "locale.h" + end, sep + "stddef.h" + end, sep + "ctype.h" + end,
    sep + "math.h" + end,   sep + "stdio.h" + end,  sep + "errno.h" + end,  sep + "setjmp.h" + end,
    sep + "stdlib.h" + end, sep + "float.h" + end,  sep + "signal.h" + end, sep + "string.h" + end,
    sep + "limits.h" + end, sep + "stdarg.h" + end, sep + "time.h" + end};

static const std::set<std::string> c99_std_headers = {
    sep + "assert.h" + end, sep + "inttypes.h" + end, sep + "signal.h" + end,
    sep + "stdlib.h" + end, sep + "complex.h" + end,  sep + "iso646.h" + end,
    sep + "stdarg.h" + end, sep + "string.h" + end,   sep + "ctype.h" + end,
    sep + "limits.h" + end, sep + "stdbool.h" + end,  sep + "tgmath.h" + end,
    sep + "errno.h" + end,  sep + "locale.h" + end,   sep + "stddef.h" + end,
    sep + "time.h" + end,   sep + "fenv.h" + end,     sep + "math.h" + end,
    sep + "stdint.h" + end, sep + "wchar.h" + end,    sep + "float.h" + end,
    sep + "setjmp.h" + end, sep + "stdio.h" + end,    sep + "wctype.h" + end};

static const std::set<std::string> c11_std_headers = {
    sep + "assert.h" + end,   sep + "math.h" + end,      sep + "stdlib.h" + end,
    sep + "complex.h" + end,  sep + "setjmp.h" + end,    sep + "stdnoreturn.h" + end,
    sep + "ctype.h" + end,    sep + "signal.h" + end,    sep + "string.h" + end,
    sep + "errno.h" + end,    sep + "stdalign.h" + end,  sep + "tgmath.h" + end,
    sep + "fenv.h" + end,     sep + "stdarg.h" + end,    sep + "threads.h" + end,
    sep + "float.h" + end,    sep + "stdatomic.h" + end, sep + "time.h" + end,
    sep + "inttypes.h" + end, sep + "stdbool.h" + end,   sep + "uchar.h" + end,
    sep + "iso646.h" + end,   sep + "stddef.h" + end,    sep + "wchar.h" + end,
    sep + "limits.h" + end,   sep + "stdint.h" + end,    sep + "wctype.h" + end,
    sep + "locale.h" + end,   sep + "stdio.h" + end};

static const std::set<std::string> cpp_std_headers = {
    sep + "algorithm" + end, sep + "fstream" + end, sep + "list" + end, sep + "regex" + end,
    sep + "tuple" + end, sep + "array" + end, sep + "functional" + end, sep + "locale" + end,
    sep + "scoped_allocator" + end, sep + "type_traits" + end, sep + "atomic" + end,
    sep + "future" + end, sep + "map" + end, sep + "set" + end, sep + "typeindex" + end,
    sep + "bitset" + end, sep + "initializer_list" + end, sep + "memory" + end,
    sep + "sstream" + end, sep + "typeinfo" + end, sep + "chrono" + end, sep + "iomanip" + end,
    sep + "mutex" + end, sep + "stack" + end, sep + "unordered_map" + end, sep + "codecvt" + end,
    sep + "ios" + end, sep + "new" + end, sep + "stdexcept" + end, sep + "unordered_set" + end,
    sep + "complex" + end, sep + "iosfwd" + end, sep + "numeric" + end, sep + "streambuf" + end,
    sep + "utility" + end, sep + "condition_variable" + end, sep + "iostream" + end,
    sep + "ostream" + end, sep + "string" + end, sep + "valarray" + end, sep + "deque" + end,
    sep + "istream" + end, sep + "queue" + end, sep + "strstream" + end, sep + "vector" + end,
    sep + "exception" + end, sep + "iterator" + end, sep + "random" + end,
    sep + "system_error" + end, sep + "forward_list" + end, sep + "limits" + end,
    sep + "ratio" + end, sep + "thread" + end,
    /* C++ headers for C library functions */
    sep + "cassert" + end, sep + "cinttypes" + end, sep + "csignal" + end, sep + "cstdio" + end,
    sep + "cwchar" + end, sep + "ccomplex" + end, sep + "ciso646" + end, sep + "cstdalign" + end,
    sep + "cstdlib" + end, sep + "cwctype" + end, sep + "cctype" + end, sep + "climits" + end,
    sep + "cstdarg" + end, sep + "cstring" + end, sep + "cerrno" + end, sep + "clocale" + end,
    sep + "cstdbool" + end, sep + "ctgmath" + end, sep + "cfenv" + end, sep + "cmath" + end,
    sep + "cstddef" + end, sep + "ctime" + end, sep + "cfloat" + end, sep + "csetjmp" + end,
    sep + "cstdint" + end, sep + "cuchar" + end};

static const std::map<LangStd_e, const std::set<std::string> &> lang_std_headers_map{
    {STD_C89, c89_std_headers},
    {STD_C99, c99_std_headers},
    {STD_C11, c11_std_headers},
    {STD_CPP, cpp_std_headers}};

void AppendStandardHeaders(const bool usedStandards[STD_COUNT],
                           std::vector<std::string> &receivingVector) {
    for (int i = STD_FIRST; i < STD_COUNT; i++) {
        if (usedStandards[i]) {
            const std::set<std::string> &headers =
                lang_std_headers_map.at(static_cast<LangStd_e>(i));
            receivingVector.insert(receivingVector.end(), headers.begin(), headers.end());
        }
    }
}