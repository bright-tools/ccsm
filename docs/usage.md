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

Note that the limits checking support does not allow for complex rules to
describe the limits.  It's suggested that such behaviour be supported by
a dedicated custom script which consumes the TSV or CSV output of CCSM.

Choosing Which Metrics Are Output
---------------------------------

By default CCSM will output all of the metrics (unless using a sparse output
format, where only metrics with a non-zero value will be output), however the
`--output-metrics` option allows the caller control over which metrics are
output.

The option takes a set of comma-separated strings, where each string is either:
* The short name of a metric or alias (e.g. `FILE_CNT`)
or
* A regex for one or more metrics or aliases (e.g. `RAW_KW.*`).  Regular expresions are POSIX format.

For example

    --output-metrics=FILE_CNT,RAW_KW.*
