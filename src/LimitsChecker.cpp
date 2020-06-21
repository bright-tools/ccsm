/*
   Copyright 2016 John Bailey

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

#include "LimitsChecker.hpp"
#include "LimitLibrary.hpp"

void LimitsChecker::dump(const MetricUnit *const p_topLevel,
                         const MetricOptions &p_options) {
    LimitLibrary limits;

    if (limits.load(p_options.getLimitsFiles())) {
        dump(p_topLevel, p_options, limits);
    } else {
        // TODO
    }
}

void LimitsChecker::dump(const MetricUnit *const p_topLevel,
                         const MetricOptions &p_options,
                         const LimitLibrary &p_limits) {
    p_options.getOutput() << check(p_topLevel, p_options, p_limits);
}

std::string LimitsChecker::check(const MetricUnit *const p_topLevel,
                                 const MetricOptions &p_options,
                                 const LimitLibrary &p_limits) {
    std::stringstream out;
    std::stringstream checkOut;
    std::string thisCheck;
    const std::string FILE_SEP = "----\n";

    const bool groupWarnings = (p_options.getLimitWarningOutputFormat() ==
                                METRIC_LIMITS_FORMAT_GROUP_BY_FILE);

    bool outputTail = false;
    thisCheck = p_limits.checkLimit(*p_topLevel, p_options);

    const MetricUnit::SubUnitMap_t *const subUnits = p_topLevel->getSubUnits();
    for (MetricUnit::SubUnitMap_t::const_iterator unitIt = subUnits->begin();
         unitIt != subUnits->end(); ++unitIt) {
        checkOut << check((*unitIt).second, p_options, p_limits);
    }

    if (groupWarnings && (checkOut.str().length() || thisCheck.length()) &&
        (p_topLevel->GetType() == METRIC_UNIT_FILE)) {
        out << "== Limit warnings for: " << p_topLevel->getUnitName(p_options)
            << "\n";
        outputTail = true;
    }

    out << thisCheck;

    if (groupWarnings && checkOut.str().length() &&
        (p_topLevel->GetType() == METRIC_UNIT_GLOBAL) && thisCheck.length()) {
        out << FILE_SEP;
    }

    out << checkOut.str();

    if (outputTail) {
        out << FILE_SEP;
    }

    return out.str();
}
