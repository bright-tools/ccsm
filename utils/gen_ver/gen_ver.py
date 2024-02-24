#!/usr/bin/python3

import subprocess
import os
import sys
import argparse

if sys.version_info.major < 3:
    raise Exception('Python3 is required for this script')

def make_ver(git_dir=None):

    # Get the version string
    ver = subprocess.check_output(['git', 'describe', '--tags', '--match', 'v[0-9]*', '--abbrev=7', '--dirty'],
                                   cwd=git_dir, universal_newlines=True)

    ver_str = ver.rstrip()

    txt = ["/* Generated automatically by {} */".format(os.path.basename(__file__)),
           '#define GEN_VER_VERSION_STRING "{}"'.format(ver_str),
           ]

    return "\n".join(txt)


class Usage(Exception):
    """This class is used to catch user errors of argparse"""
    def __init__(self, parser, msg):
        self.parser = parser
        self.msg = msg


class NotGitRepo(Exception):
    """This class is used to inform we need to be in a GIT directory"""
    def __init__(self):
        pass


def main(argv=None):
    parser = None
    try:
        parser = argparse.ArgumentParser(description="Create an include file with a GIT version string")
        parser.add_argument('hfile', metavar='HFILE', type=str,
                            help='the name of the include file')
        parser.add_argument('-v', '--verbose', action='store_true',
                            help='be verbose')
    except argparse.ArgumentError as msg:
        # The construction of the parser failed
        raise Usage(parser, msg)

    args = parser.parse_args(argv)
    # print(args)

    if not os.path.exists('.git'):
        raise NotGitRepo()

    version = make_ver()
    if args.verbose:
        print(version)

    filename = args.hfile
    if not os.path.exists(filename):
        print("Creating new '{}'".format(filename))
        open(filename, "w").write(version)
    else:
        old_version = open(args.hfile).read()
        if old_version != version:
            print("Version has changed, updating '{}'".format(filename))
            open(filename, "w").write(version)
        else:
            print("Version has not changed, leaving '{}' as is".format(filename))

if __name__ == '__main__':
    try:
        main()
    except Usage as usg:
        # The construction of the argparser failed
        usg.parser.error(usg.msg)
    except NotGitRepo:
        print("This must be execute from a GIT repo (directory)")
    except subprocess.CalledProcessError as e:
        print("Failed to invoke git:")
        print(e.output)
    except IOError as io:
        print(io)
    except SystemExit as e:
        # print(e)
        sys.exit(e)
