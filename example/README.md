Running CCSM
============

Command Line Structure
----------------------

CCSM is run from the command line, in the same way that many compilers are.
This means that it should be relatively simple to integrate it into most build
systems, such as those based on Makefiles.

The commandline has several parts:

    ccsm --ccsm-options-here file1.c file2.c -- --compiler-options-here

1.  `ccsm` - used to run CCSM!
2.  `--ccsm-options-here` - this part of the command line is used to specify
    options which control CCSM's behaviour, such as changing the output format.
    A complete list of options can be seen by using `ccsm --help` from the
    command line
3.  `file1.c file2.c` - these are the files to be analysed.  Note that the
    ordering of parts 2 and 3 is not significant - files may be specified 
    before CCSM options.
4.  `--` - separates the compiler options (see next) from the rest of the
    command line
5.  `--compiler-options-here` - these options are those normally used during the
    compilation of the code, such as setting include paths or defining
    pre-processor symbols.  Note that the options here are those which are
    understood by [Clang](http://clang.llvm.org/docs/UsersManual.html), which
    are broadly compatible with GCC and Microsoft compilers.  However, you may
    need to adjust the format of the options in this section from those which
    you usually use.

Example
-------

An example invocation is:

    ccsm --output-format=sparsetree example1.c -- -I../externals/c_headers/include

Header Files
------------

Many compilers supply standard header files (e.g. `stdlib.h`) which include 
compiler-specific extensions which cannot easily be processed by CCSM.  In order
to work around this, the [C headers repository](https://github.com/bright-tools/c_headers) aims to provide a 'clean' set of standard headers which can be used during code analysis.  Generally it's not interesting to include the standard library headers in the analysis, so this should not have a discernable effect on the results of the metrics.
