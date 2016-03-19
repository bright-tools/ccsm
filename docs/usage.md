Generic Usage
-------------

The command-line when using a [Compilation Database](http://clang.llvm.org/docs/JSONCompilationDatabase.html) is:

    ccsm [ccsm options] <source0> [... <sourceN>]

If not using a compilation database, use:
    
    ccsm [ccsm options] <source0> [... <sourceN>] -- [compiler options]

Getting Command Line Help
-------------------------

A current list of CCSM options can be displayed by running with the `--help`
option:

    ccsm --help

Using Response Files
--------------------

Response files can be used to avoid the need to specify a long list of options
on the command line.  When using response files, the command line options are
contained in a text file and that text file is referenced on the command line
using the notation `@filename`.

For example:

    ccsm @opts.txt --

NOTE: Response files cannot be used after the `--` separator.  If you wish to
avoid having to specify a long list of compiler options, it's necessary to use a
[Compilation Database](http://clang.llvm.org/docs/JSONCompilationDatabase.html)
rather than response files.

Excluding Files From The Results
--------------------------------

Files can be excluded from the results using the `-exclude-file` option.  This
option accepts a comma separated list of regex patterns which will be used to
exclude.  For example:

    --exclude-file=comment.h$$,free.h$$

would exclude files with names ending `comment.h` and those ending `free.h`

    --exclude-file=.h$$

would exclude all files with names ending `.h` (likely most header files)

Limits Checking
---------------

CCSM includes functionality to check against limits for specific metrics.  In
order to use this, a limits file must be specified on the command line using the
`--limits` option, for example

    --limits=limit_file.csv

Please see the [limits file format](limits.md) for details of how to specify the
file.
