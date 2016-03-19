Limits File Format
==================

The limits file is a CSV file, with the following columns:

    Metric Name,Global Flag,File Flag,File Regex,Function Regex,Operator,Limit

| Field | Description |
|-------|-------------|
| Metric Name | Short name for the metric to be checked |
| Global Flag | Indicate whether or not the check should be applied at the global wrap-up level.  An empty value indicates that it should not, any non-zero
length string indicates that it should |
| File Flag | Indicate whether or not the check should be applied at the file level.  An empty value indicates that it should not, any non-zero
length string indicates that it should |
| File Regex | Regex for matching file names. |
| Function Regex | Regex for matching function names.  Leaving this empty indicates that the check should not be applied at the function level |
| Operator | Operator used in the comparison of the value against the limit.  Currently valid are `>`, `<`, `>=` and `<=` |
| Limit | Limit to compare the value of the metric against |
| Description | Optional free-form textual description of the limit.  This is output when the limit is not met |

Any line which starts with a semi-colon character - `;` - is treated as a
comment.

Limit Precidence
----------------

Limits are specified in increasing order of precidence.  That is to say that
a failure will only be reported in the case that a particular element exceeds
the last matching limit rule which applies to it.

This is intended to allow specific exceptions to be added for checks.

For example:

    ; By default, no function should have a nesting value >4
    FUNC_NESTING,,,*,*,<=,4
    ; Functions withing the 'nesty.c' file are allowed to have a nesting value
    ;  of up to 6
    FUNC_NESTING,,,nesty.c,*,<=,6
    ; Any fuction which is called 'complicated' is allowed to have a nesting
    ;  value of up to 100
    FUNC_NESTING,,,*.c,^complicated$,<=,100

In this example, a function called `complicated` within the `nesty.c` file would
have a limit of 100, as the 3rd rule maches and takes precidence over the 2nd
rule.

Examples
--------

    COMMENT_HIS_COMF,GLOBAL,FILE,,,>,0.2

This implements a check at the global level on the comment HIS metric which 
ensures that the ration between statements and comments is greater than 0.2.
The check will not be applied at the function level
