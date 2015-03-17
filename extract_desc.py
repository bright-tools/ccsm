#!/usr/bin/python

import os
import re

def_dir = 'src'
mtrc_def_mtch_re = re.compile('METRIC\(\s*\w+\s*,\s*"(\w+)"\s*,\s*"(.*?)".*"(.*?)"')

print "| Name | Description | Notes |"
print "|------|-------------|-------|"

# Extract a set of all of the different types of metrics from the def file
with open( def_dir + os.sep + 'metrics.def' ) as fh:
    for line in fh:
        results = re.match( mtrc_def_mtch_re, line )
        if results:
            print "| " + results.group(1) + " | " + results.group(2) + " | " + results.group(3) + " |"
