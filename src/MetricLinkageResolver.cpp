/*
Copyright 2015 John Bailey

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

#include "MetricLinkageResolver.hpp"
#include "MetricUtils.hpp"

#include <iostream>

extern void resolveLinkages(MetricUnit *p_topUnit) {
    MetricUnit::FunctionMap_t mapping = p_topUnit->getAllFunctionMap();

    /* Iterate all of the functions */
    for (MetricUnit::FunctionMap_t::const_iterator it = mapping.begin(); it != mapping.end();
         it++) {
        /* Get a list of unresolved function calls within the current function &
         * iterate */
        const std::set<std::string> &unresolvedFns = it->second->getUnresolvedFns();
        for (std::set<std::string>::const_iterator uit = unresolvedFns.begin();
             uit != unresolvedFns.end(); uit++) {
            /* Get a list of all functions with a matching name ... */
            MetricUnit::FunctionMap_t::iterator fns = mapping.find(*uit);
            while (fns != mapping.end()) {
                /* First matching function with external linkage will be
                 * assocated with the call */
                if (fns->second->hasExternalLinkage()) {
                    /* TODO: Adding a non-NULL SourceLocation here would be
                     * useful */
                    fns->second->increment(METRIC_TYPE_CALLED_BY_EXTERN, InvalidSourceAndLine);
                    break;
                }
                fns++;
            }
        }
    }
}
