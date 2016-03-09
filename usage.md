Generic Usage
-------------

The command-line when using a [Compilation Database](http://clang.llvm.org/docs/JSONCompilationDatabase.html) is:

    ccsm [ccsm options] <source0> [... <sourceN>]

If not using a compilation database, use:
    
    ccsm [ccsm options] <source0> [... <sourceN>] -- [compiler options]

Getting Command Line Help
-------------------------

`ccsm --help`

Excluding Files From The Results
--------------------------------

Files can be excluded from the results using the `-exclude-file` option.  This
option accepts a comma separated list of regex patterns which will be used to
exclude.  For example:

    --exclude-file=comment.h$$,free.h$$

would exclude files with names ending `comment.h` and those ending `free.h`

    --exclude-file=.h$$

would exclude all files with names ending `.h` (likely most header files)
