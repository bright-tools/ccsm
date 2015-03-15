ccsm
====

C Code Source Metrics - tool to gather simple metrics from C code (C++ will hopefully be supported in the future).

CCSM is built as an [LLVM](http://clang.llvm/org/) tool.  clang does the heavy
lifting of parsing the C/C++ into an AST, then CCSM processes the AST to gather
the metrics.

Some of the metrics are very straight forward (and arguably may not be
particularly useful) by themselves, for example the count of the number of 'if'
conditions.  Other metrics are built on these, such as the [McCabe cyclomatic
complexity](http://en.wikipedia.org/wiki/Cyclomatic_complexity) measure.

The aim is to also include metrics in order to support the set specified by
[HIS](http://portal.automotive-his.de/images/pdf/SoftwareTest/his-sc-metriken.1.3.1_e.pdf).

Output is hierachial - for example, the metrics relating to each function are
output and the metrics for a file will include the functions relating to the
functions within that file.  A global wrap-up provides a top-level overview of
all files.

Example Output
==============

Output can be in the form of CSV or TSV (ideal for processing) or a more human
readable 'tree' form:

    Global
    Files: 1
    Functions: 2
    IF statements: 6
    ELSE statements: 4
    FOR loops: 1
    RETURN statements: 3
    WHILE loops: 1
    SWITCH statements: 1
    CASE statements: 4
    DEFAULT statements: 1
    LOGICAL AND operators: 0
    LOGICAL OR operators: 1
    TERNARY operators: 1
    GOTO statements: 1
    LABEL statements: 2
    Variables: 5
    Return points: 2
    Statements: 113
    File: src\1.c
            Functions/Methods: 2
            Functions: 2
            IF statements: 6
            ELSE statements: 4
            FOR loops: 1
            RETURN statements: 3
            WHILE loops: 1
            SWITCH statements: 1
            CASE statements: 4
            DEFAULT statements: 1
            LOGICAL AND operators: 0
            LOGICAL OR operators: 1
            TERNARY operators: 1
            GOTO statements: 1
            LABEL statements: 2
            Variables: 5
            Return points: 2
            Statements: 113
            Function: main
                    IF statements: 3
                    ELSE statements: 1
                    FOR loops: 0
                    RETURN statements: 2
                    WHILE loops: 0
                    SWITCH statements: 0
                    CASE statements: 0
                    DEFAULT statements: 0
                    LOGICAL AND operators: 0
                    LOGICAL OR operators: 0
                    TERNARY operators: 0
                    GOTO statements: 1
                    LABEL statements: 2
                    Variables: 0
                    Return points: 1
                    Statements: 27
                    McCabe: 4
                    Modified McCabe: 4
            Function: vain
                    IF statements: 3
                    ELSE statements: 3
                    FOR loops: 1
                    RETURN statements: 1
                    WHILE loops: 1
                    SWITCH statements: 1
                    CASE statements: 4
                    DEFAULT statements: 1
                    LOGICAL AND operators: 0
                    LOGICAL OR operators: 1
                    TERNARY operators: 1
                    GOTO statements: 0
                    LABEL statements: 0
                    Variables: 1
                    Return points: 1
                    Statements: 86
                    McCabe: 12
                    Modified McCabe: 9

Metric Details
==============

TODO

Token & Statement Based Metrics
===============================

Many of the metrics reported have 2 variants - one counting the number of statements in the code after preprocessor expansion and the other counting the number of tokens in the unexpanded code.

Take for example:

    #define CHECK_FOR_ERROR( _val ) do { if( _val >= 0 ) { error(); } } while( 0 )
    void some_function( int neg_value )
    {
        CHECK_FOR_ERROR();

The fragment of some_function in the above code does not contain any 'if' tokens until the `CHECK_FOR_ERROR` macro is expanded.

Due to the fact that some people like to treat macros as a form of encapsulation and don't want the contents of macros counting towards function metrics while other people do, CCSM provides metrics for both, allowing the user to pick and choose.


HIS Metrics Support
-------------------

| Metric      | Status                                       | Description     |
|-------------|----------------------------------------------|-----------------|
| COMF        | Implemented - `TOK_HIS_COMF`                 | Comment density.  Ratio of comments to 'statements' |
| PATH        | Not yet implemented                          | |
| GOTO        | Implemented - `STMT_GOTO_CNT` and `TOK_GOTO` | Number of GOTO statements |
| v(G)        | Implemented - `TOK_MCCABE` and `STMT_MCCABE` | Cyclomatic complexity     |
| CALLING     | Not yet implemeted                           | |
| CALLS       | Implemented - `OP_FN_CALL_CNT`               | Number of different functions called.  Note that functions called via function pointer are not counted |
| PARAM       | Implemented - `STMT_HIS_PARAM`               | Number of function parameters.  Note that only one variant of this metric exists as parameters hidden within macros are still parameters. |
| STMT        | Implemented - `TOK_HIS_STMT`                 | Number of instructions per function. |
| LEVEL       | Implemented - `FUNC_DEPTH`                   | Nesting depth within a function.  Note that HIS says that this is defined as "Maximum nesting levels within a function + 1", however the allowable range is 0-4, which doesn't seem consistent.  `FUNC_DEPTH` in this implementation is zero for an empty function |
| RETURN      | Implemented - `RETURN_POINT_CNT`             | Number of return points in a function.  Note that only one variant of this metric exists as return points hidden within macros are still return points.  Note Issue #54 with respect to this metric. |
| S           | Not planned                                  | |
| VOCF        | Not yet implemented                          | |
| NOMV        | Not planned                                  | |
| NOMVPR      | Not planned                                  | |
| ap_cg_cycle | Not planned                                  | |

Building The Project
====================

TODO

Code Structure
==============

File                  | Content
----------------------|--------
ccsm.cpp              | main() function to process command line options, set up clang and invoke the processing of the AST via a MetricMatcher
MetricASTConsumer.cpp | Class which received callbacks as new AST translation units are processed 
MetricUnit.cpp        | Class to support counting of the metrics, arranged th objects arranged in a hierachial structure
MetricUtils.cpp       | Support functions for doing any significant AST processing not supported by clang out-of-the-box
MetricMatcher.cpp     | AST visitor which deals with processing the AST call-backs and recording the appropriate metrics via MetricUnit objects
MetricPPConsumer.cpp  | Class to receive call-backs from the pre-processor in order to collect metrics relating to comments (not present in the AST tree)

McCabe Complexity
=================

The `TOK_MCCABE` and `STMT_MCCAKE` metrics are based on the McCabe method of calculating 
[cyclomatic complexity](http://www.mccabe.com/pdf/mccabe-nist235r.pdf).

> Cyclomatic complexity is defined for each module to be e - n + 2, where e and n are the number
> of edges and nodes in the control flow graph, respectively. [...]
> Cyclomatic complexity is also known as v(G), where v refers to the cyclomatic number in
> graph theory and G indicates that the complexity is a function of the graph.

Useful Links
============

[C99 + TCs](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf)
