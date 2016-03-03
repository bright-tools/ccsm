#!/usr/bin/python

import subprocess
import os
import sys

def make_ver( git_dir ):
    ret_val = 1
    string = ""
    try:
        # Get the version string
        ver = subprocess.check_output(['git','describe','--match','v[0-9]*','--abbrev=7','HEAD'],cwd=git_dir)
        # Check for local changes
        chngs = subprocess.check_output(['git','diff-index','--name-only','HEAD','--'],cwd=git_dir)

        ver_str = ver.rstrip();
        if( len( chngs ) ):
            ver_str += ".dirty"

        string  = "/* Generated automatically by {} */\n"\
                  '#define GEN_VER_VERSION_STRING "{}"'.format(os.path.basename(__file__),ver_str)
        ret_val = 0
    except subprocess.CalledProcessError, e:
         string = "Failed to invoke git:\n", e.output

    return ret_val, string

if __name__ == '__main__':
    ret_val = 1
    if ( len( sys.argv ) < 2 ):
        print "You must specify the output file as a parameter"
    else:
        filename = sys.argv[1]
        if ( len( sys.argv ) > 2 ):
            git_dir = sys.argv[2]
        else:
            git_dir = "."

        code, string = make_ver( git_dir )

        if( code != 0 ):
            print string
            ret_val = code
        else:
            content = ""
            if( os.path.isfile( filename )):
                with open(filename, 'r') as content_file:
                    content = content_file.read()
            if( content != string ):
                print "Version has changed, updating '{}'".format(filename)
                output_file = open(filename,'w')
                output_file.write(string)
            else:
                print "Version has not changed, leaving '{}' as is".format(filename)
            ret_val = 0

    sys.exit( ret_val )
