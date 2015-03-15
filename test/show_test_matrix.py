#!/usr/bin/python

import fnmatch
import os
import re

test_dir = 'tests'
def_dir = '../src'
mtrc_mk_mtch_re = re.compile('(\w+):\s*METRICS=(.*)')
mtrc_def_mtch_re = re.compile('METRIC\(\s*\w+\s*,\s*"(\w+)')
test_dict = dict()
all_metrics = set()

# Extract a list of the metric names referenced in the test files &
#  compose them into a dictionary (keyed on metric name) of sets
#  (containing the set of filenames which reference the metric)
for file in os.listdir( test_dir ):
    if( fnmatch.fnmatch(file,'*.mk')):
        with open( test_dir + os.sep + file ) as fh:
            for line in fh:
                results = re.match( mtrc_mk_mtch_re, line )
                if results:
                    for metric_name in results.group(2).split(','):
                        if metric_name not in test_dict:
                            test_dict[ metric_name ] = set()
                        test_dict[ metric_name ].add( results.group(1 ))

# Extract a set of all of the different types of metrics from the def file
with open( def_dir + os.sep + 'metrics.def' ) as fh:
    for line in fh:
        results = re.match( mtrc_def_mtch_re, line )
        if results:
            all_metrics.add( results.group( 1 ))

metrics_without_test = 0
# Dump out the mapping of metrics to test files
for metric in all_metrics:
    out = ""
    if( test_dict.has_key( metric )):
        for filename in test_dict[metric]:
            if len( out ):
                out = out + ", "
            out = out + filename
    if( len( out ) == 0 ):
        metrics_without_test = metrics_without_test + 1
    print metric + ": " + out

print
print "%d metrics without test" % (metrics_without_test)
