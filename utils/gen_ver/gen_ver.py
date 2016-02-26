#!/usr/bin/python

import subprocess
import os
import sys

if __name__ == '__main__':
    try:
        if ( len( sys.argv ) > 1 ):
            git_dir = sys.argv[1]
        else:
            git_dir = "."

        # Get the version string
        ver = subprocess.check_output(['git','describe','--match','v[0-9]*','--abbrev=7','HEAD'],cwd=git_dir)
        # Check for local changes
        chngs = subprocess.check_output(['git','diff-index','--name-only','HEAD','--'],cwd=git_dir)

        ver_str = ver.rstrip();
        if( len( chngs ) ):
            ver_str += ".dirty"

        print "/* Generated automatically by ",os.path.basename(__file__)," */"
        print '#define GEN_VER_VERSION_STRING      "{}"'.format(ver_str)
    except subprocess.CalledProcessError, e:
        print "Failed to invoke git:\n", e.output
