ccsm
====

C Code Source Metrics - tool to gather simple metrics from C code.

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

HIS Metrics Support
-------------------

| Metric      | Status                |
|-------------|-----------------------|
| COMF        | Not yet implemented   |
| PATH        | Not yet implemented   |
| GOTO        | Implemented           |
| v(G)        | Implemented           |
| CALLING     | Not yet implemeted    |
| CALLS       | Not yet implemented   |
| PARAM       | Implemented           |
| STMT        | Not yet implemented   |
| LEVEL       | Not yet implemented   |
| RETURN      | Implemented           |
| S           | Not planned           |
| VOCF        | Not yet implemented   |
| NOMV        | Not planned           |
| NOMVPR      | Not planned           |
| ap_cg_cycle | Not planned           |

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
