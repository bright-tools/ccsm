#!/usr/bin/python

import os
import re
import sys

column_count = 3

escape_pipe = False
if( "--escape-pipe" in sys.argv ):
    escape_pipe = True

def_dir = 'src'
mtrc_def_mtch_re = re.compile('METRIC\(\s*\w+\s*,\s*"(\w+)"\s*,\s*"(.*?)".*"(.*?)"')

print "| Name | Description | Notes |"
print "|------|-------------|-------|"

# Extract a set of all of the different types of metrics from the def file
with open( def_dir + os.sep + 'metrics.def' ) as fh:
    for line in fh:
        results = re.match( mtrc_def_mtch_re, line )
        if results:
	    output = ['','','']
	    for i in range(1, column_count):
	    	output[i-1] = results.group(i)
	    if( escape_pipe ):
	    	for i in range(1, column_count):
	    	    output[i-1] = output[i-1].replace("|","<code>&#124;</code>")
            print "| " + output[0] + " | " + output[1] + " | " + output[2] + " |"
