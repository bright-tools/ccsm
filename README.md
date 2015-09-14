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

| Name | Description | Notes |
|------|-------------|-------|
| FILE_CNT | Files |  |
| FILE_SIZE | Size in bytes |  |
| FUNC_CNT | Functions |  |
| KW_IF_CNT | 'if' keyword count  |  |
| RAW_KW_IF_CNT | 'if' keyword count (raw source) |  |
| KW_ELSE_CNT | 'else' keyword count  |  |
| RAW_KW_ELSE_CNT | 'else' keyword count (raw source) |  |
| KW_FOR_CNT | 'for' keyword count |  |
| RAW_KW_FOR_CNT | 'for' keyword count (raw source) |  |
| KW_RETURN_CNT | 'return' keyword count |  |
| RAW_KW_RETURN_CNT | 'return' keyword count (raw source) |  |
| KW_DO_CNT | 'do' keyword count |  |
| RAW_KW_DO_CNT | 'do' keyword count (raw source) |  |
| KW_WHILE_CNT | 'while' keyword count |  |
| RAW_KW_WHILE_CNT | 'while' keyword count (raw source) |  |
| KW_SWITCH_CNT | 'switch' keyword count |  |
| RAW_KW_SWITCH_CNT | 'switch' keyword count (raw source) |  |
| KW_CASE_CNT | 'case' keyword count |  |
| RAW_KW_CASE_CNT | 'case' keyword count (raw source) |  |
| KW_BREAK_CNT | 'break' keyword count |  |
| RAW_KW_BREAK_CNT | 'break' keyword count (raw source) |  |
| KW_DEFAULT_CNT | 'default' keyword count |  |
| RAW_KW_DEFAULT_CNT | 'default' keyword count (raw source) |  |
| KW_GOTO_CNT | 'goto' keyword count |  |
| RAW_KW_GOTO_CNT | 'goto' keyword count (raw source) |  |
| KW_AUTO_CNT | 'auto' keyword count |  |
| RAW_KW_AUTO_CNT | 'auto' keyword count (raw source) |  |
| KW_VOLATILE_CNT | 'volatile' keyword count |  |
| RAW_KW_VOLATILE_CNT | 'volatile' keyword count (raw source) |  |
| KW_CONST_CNT | 'const' keyword count |  |
| RAW_KW_CONST_CNT | 'const' keyword count (raw source) |  |
| KW_TYPEDEF_CNT | 'typedef' keyword count |  |
| RAW_KW_TYPEDEF_CNT | 'typedef' keyword count (raw source) |  |
| KW_CONTINUE_CNT | 'continue' keyword count |  |
| RAW_KW_CONTINUE_CNT | 'continue' keyword count (raw source) |  |
| KW_UNION_CNT | 'union' keyword count |  |
| RAW_KW_UNION_CNT | 'union' keyword count (raw source) |  |
| KW_STRUCT_CNT | 'struct' keyword count |  |
| RAW_KW_STRUCT_CNT | 'struct' keyword count (raw source) |  |
| KW_ENUM_CNT | 'enum' keyword count |  |
| RAW_KW_ENUM_CNT | 'enum' keyword count (raw source) |  |
| KW_CHAR_CNT | 'char' keyword count |  |
| RAW_KW_CHAR_CNT | 'char' keyword count (raw source) |  |
| KW_UNSIGNED_CNT | 'unsigned' keyword count |  |
| RAW_KW_UNSIGNED_CNT | 'unsigned' keyword count (raw source) |  |
| KW_SIGNED_CNT | 'signed' keyword count |  |
| RAW_KW_SIGNED_CNT | 'signed' keyword count (raw source) |  |
| KW_DOUBLE_CNT | 'double' keyword count |  |
| RAW_KW_DOUBLE_CNT | 'double' keyword count (raw source) |  |
| KW_FLOAT_CNT | 'float' keyword count |  |
| RAW_KW_FLOAT_CNT | 'float' keyword count (raw source) |  |
| KW_INT_CNT | 'int' keyword count |  |
| RAW_KW_INT_CNT | 'int' keyword count (raw source) |  |
| KW_LONG_CNT | 'long' keyword count |  |
| RAW_KW_LONG_CNT | 'long' keyword count (raw source) |  |
| KW_SHORT_CNT | 'short' keyword count |  |
| RAW_KW_SHORT_CNT | 'short' keyword count (raw source) |  |
| KW_STATIC_CNT | 'static' keyword count |  |
| RAW_KW_STATIC_CNT | 'static' keyword count (raw source) |  |
| KW_EXTERN_CNT | 'extern' keyword count |  |
| RAW_KW_EXTERN_CNT | 'extern' keyword count (raw source) |  |
| KW_REGISTER_CNT | 'register' keyword count |  |
| RAW_KW_REGISTER_CNT | 'register' keyword count (raw source) |  |
| KW_CNT | Number of keywords used | Total number of C keywords used |
| KW_TYPES_CNT | Number of types of keyword used |  |
| KW_LABEL_CNT | 'label' keyword count |  |
| VAR_FILE_LOC_CNT | File-scope total local variable declarations | Includes auto and static but not extern |
| VAR_FILE_LOC_STATIC_CNT | File-scope static variable declarations |  |
| VAR_FILE_EXT_CNT | File-scope external variable declarations |  |
| VAR_FILE_VOLATILE_CNT | File-scope volatile variable declarations |  |
| VAR_FILE_CONST_CNT | File-scope constant variable declarations |  |
| VAR_FN_LOC_CNT | Function-scope total local variable declarations | Includes auto and static but not extern |
| VAR_FN_LOC_STATIC_CNT | Function-scope static variable declarations |  |
| VAR_FN_LOC_CONST_CNT | Function-scope constant variable declarations |  |
| VAR_FN_LOC_VOLATILE_CNT | Function-scope volatile variable declarations |  |
| VAR_FN_LOC_REG_CNT | Function-scope register variable declarations |  |
| VAR_FN_LOC_AUTO_CNT | Function-scope auto variable declarations |  |
| VAR_FN_EXT_CNT | Function-scope external variable declarations |  |
| RETURN_POINT_CNT | Return points |  |
| STMT_CNT | Statement count | Number of statements |
| COMMENT_HIS_COMF | Comment density | Relationship of the number of comments to the number of statements, with range 0-1.  HIS metric. |
| COMMENT_BYTE_CNT | Comments in bytes | Counts the total number of bytes which are used in comments.  Note that --exclude-function does not have any effect on this metric. |
| COMMENT_CNT | Comment count | Counts the total number of comments.  Note that --exclude-function does not have any effect on this metric. |
| MCCABE | McCabe complexity |  |
| MCCABE_MOD | Modified McCabe complexity |  |
| RAW_MCCABE | McCabe complexity (raw source) |  |
| RAW_MCCABE_MOD | Modified McCabe complexity (raw source) |  |
| FUNC_LOCAL_CNT | Local Functions |  |
| FUNC_EXTERN_EXPL_CNT | External (explicit) functions declarations |  |
| FUNC_EXTERN_IMPL_CNT | External (implicit) functions declarations |  |
| FUNC_INLINE_CNT | Inline Functions |  |
| FUNC_CALLED_BY_LOCAL | Number of local functions calling this function |  |
| FUNC_CALLED_BY_EXTERN | Number of external functions calling this function |  |
| OP_FN_CALL_CNT | Function Calls |  |
| FUNC_PATHS | Number of paths through the function |  |
| OP_FN_CALL_UNIQUE_CNT | No. different functions called | Of the total number of functions called (see OP_FN_CALL_CNT), this represents the number of uniquely addressed functions.  Does not include functions called via function pointers |
| LOCAL_FN_CALL_CNT | Local Function Calls | Does not include functions called via function pointers |
| FILE_LINE_CNT | Size in lines | Counts number of newlines in the file |
| FUNC_LINE_CNT | Function (body) size in lines | Counts number of newlines in the function body |
| STMT_HIS_PARAM | Function Parameters |  |
| TODO | Decisions |  |
| TODO | Loops |  |
| FUNC_NESTING | Nesting Level |  |
| HIS_VOCF | VOCF |  |
| OP_ASSIGN_CNT | Basic assignment operators |  |
| OP_ADD_CNT | Addition operators |  |
| OP_ADD_ASSIGN_CNT | Addition with assign operators |  |
| OP_SUB_CNT | Subtraction operators |  |
| OP_SUB_ASSIGN_CNT | Subtraction with assign operators |  |
| OP_UNARY_PLUS_CNT | Unary plus operators |  |
| OP_UNARY_MINUS_CNT | Unary minus operators |  |
| OP_MULT_CNT | Multiplication operators |  |
| OP_MULT_ASSIGN_CNT | Multiplication with assignment operators |  |
| OP_DIV_CNT | Division operators |  |
| OP_DIV_ASSIGN_CNT | Division with assign operators |  |
| OP_MOD_CNT | Modulo operators |  |
| OP_MOD_ASSIGN_CNT | Modulo with assign operators |  |
| OP_INC_PRE_CNT | Pre-increment operators |  |
| OP_INC_POST_CNT | Post-increment operators |  |
| OP_DEC_PRE_CNT | Pre-decrement operators |  |
| OP_DEC_POST_CNT | Post-decrement operators |  |
| OP_SHFT_LEFT_CNT | Left shift operators |  |
| OP_SHFT_LEFT_ASSIGN_CNT | Left shift with assign operators |  |
| OP_SHFT_RGHT_CNT | Right shift operators |  |
| OP_SHFT_RGHT_ASSIGN_CNT | Right shift with assign operators |  |
| OP_CMP_LT_CNT | Less than operators |  |
| OP_CMP_GT_CNT | Greater than operators |  |
| OP_CMP_LT_EQ_CNT | Less than or equal operators |  |
| OP_CMP_GT_EQ_CNT | Greater than or equal operators |  |
| OP_CMP_EQ_CNT | Equality operators |  |
| OP_CMP_NEQ_CNT | Inequality operators |  |
| OP_COMMA_CNT | Comma operators |  |
| OP_TERNARY_CNT | TERNARY operators |  |
| OP_LOG_AND_CNT | LOGICAL AND operators |  |
| OP_LOG_OR_CNT | LOGICAL OR operators |  |
| OP_LOG_NIT_CNT | LOGICAL NOT operators |  |
| OP_BITWISE_AND_CNT | Bitwise and operators |  |
| OP_BITWISE_AND_ASSIGN_CNT | Bitwise and with assign operators |  |
| OP_BITWISE_OR_CNT | Bitwise or operators |  |
| OP_BITWISE_OR_ASSIGN_CNT | Bitwise or with assign operators |  |
| OP_BITWISE_XOR_CNT | Bitwise xor operators |  |
| OP_BITWISE_XOR_ASSIGN_CNT | Bitwise xor with assign operators |  |
| OP_BITWISE_NIT_CNT | Bitwise not operators |  |
| OP_PTR_TO_MEMBER_DIRECT_CNT | Pointer to member (direct) operators |  |
| OP_PTR_TO_MEMBER_INDIRECT_CNT | Pointer to member (indirect) operators |  |
| OP_ADDR_OF_CNT | Address of operators |  |
| OP_DEREF_CNT | Dereference operators |  |
| OP_ARRAY_SUBSCRIPT_CNT | Array subscript operators |  |
| OP_MEMBER_ACCESS_DIRECT_CNT | Member access (direct) operators |  |
| OP_MEMBER_ACCESS_POINTER_CNT | Member access (via pointer) operators |  |
| OP_SIZEOF_CNT | Sizeof operators |  |
| OP_ALIGNOF_CNT | Alignof operators |  |
| OP_CAST_CNT | Cast operators |  |
| OP_TYPES_CNT | Number of different types of C operator | Number of distinct different types of operators. |
| OP_CNT | Number of operators | Total number of operators used |
| OP_HALSTEAD_TYPES_CNT | Number of different types of halstead operators | Number of distinct different types of operators (as classified for the purpose of Halstead metrics; this is not the same classification as the 'C' standard) |
| OP_HALSTEAD_CNT | Number of halstead operators | Total number of operators (as classified for the purpose of Halstead metrics; this is not the same classification as the 'C' standard) used |
| TOK_BOOL | Number of 'bool' tokens |  |
| TOK_VOID | Number of 'void' tokens |  |
| TOK_INLINE | Number of 'inline' tokens |  |
| TOK_VIRTUAL | Number of 'virtual' tokens |  |
| TOK_MUTABLE | Number of 'mutable' tokens |  |
| TOK_FRIEND | Number of 'friend' tokens |  |
| TOK_ASM | Number of 'asm' tokens |  |
| TOK_CLASS | Number of 'class' tokens |  |
| TOK_DELETE | Number of 'delete' tokens |  |
| TOK_NEW | Number of 'new' tokens |  |
| TOK_OPERATOR | Number of 'operator' tokens |  |
| TOK_PRIVATE | Number of 'private' tokens |  |
| TOK_PROTECTED | Number of 'protected' tokens |  |
| TOK_PUBLIC | Number of 'public' tokens |  |
| TOK_SIZEOF | Number of 'sizeof' tokens |  |
| TOK_THIS | Number of 'this' tokens |  |
| TOK_NAMESPACE | Number of 'namespace' tokens |  |
| TOK_USING | Number of 'using' tokens |  |
| TOK_TRY | Number of 'try' tokens |  |
| TOK_CATCH | Number of 'catch' tokens |  |
| TOK_THROW | Number of 'throw' tokens |  |
| TOK_TYPEID | Number of 'typeid' tokens |  |
| TOK_TEMPLATE | Number of 'template' tokens |  |
| TOK_EXPLICIT | Number of 'explicit' tokens |  |
| TOK_TRUE | Number of 'true' tokens |  |
| TOK_FALSE | Number of 'false' tokens |  |
| TOK_TYPENAME | Number of 'typename' tokens |  |
| TOK_NOT | Number of '!' tokens |  |
| TOK_NOT_EQUAL | Number of '!=' tokens |  |
| TOK_MODULO | Number of '%' tokens |  |
| TOK_MODULO_ASSIGN | Number of '%=' tokens |  |
| TOK_AMP | Number of '&' tokens |  |
| TOK_AMPAMP | Number of '&&' tokens |  |
| TOK_PIPEPIPE | Number of '<code>&#124;</code><code>&#124;</code>' tokens |  |
| TOK_AND_ASSIGN | Number of '&=' tokens |  |
| TOK_LPAREN | Number of '(' tokens |  |
| TOK_RPAREN | Number of ')' tokens |  |
| TOK_ASTERISK | Number of '*' tokens |  |
| TOK_ASTERISK_ASSIGN | Number of '*=' tokens |  |
| TOK_PLUS | Number of '+' tokens |  |
| TOK_PLUSPLUS | Number of '++' tokens |  |
| TOK_PLUS_ASSIGN | Number of '+=' tokens |  |
| TOK_COMMA | Number of ',' tokens |  |
| TOK_MINUS | Number of '-' tokens |  |
| TOK_MINUSMINUS | Number of '--' tokens |  |
| TOK_MINUS_ASSIGN | Number of '-=' tokens |  |
| TOK_MEMBER_PTR | Number of '->' tokens |  |
| TOK_MEMBER_REF | Number of '.' tokens |  |
| TOK_ELLIPSIS | Number of '...' tokens |  |
| TOK_SLASH | Number of '/' tokens |  |
| TOK_SLASH_ASSIGN | Number of '/=' tokens |  |
| TOK_COLON | Number of ':' tokens |  |
| TOK_COLONCOLON | Number of '::' tokens |  |
| TOK_LESS | Number of '<' tokens |  |
| TOK_LESSLESS | Number of '<<' tokens |  |
| TOK_LESSLESS_ASSIGN | Number of '<<=' tokens |  |
| TOK_LESS_EQUAL | Number of '<=' tokens |  |
| TOK_ASSIGN | Number of '=' tokens |  |
| TOK_COMPARISON | Number of '==' tokens |  |
| TOK_MORE | Number of '>' tokens |  |
| TOK_MOREMORE | Number of '>>' tokens |  |
| TOK_MOREMORE_ASSIGN | Number of '>>=' tokens |  |
| TOK_MORE_EQUAL | Number of '>=' tokens |  |
| TOK_LSQUARE | Number of '[' tokens |  |
| TOK_RSQUARE | Number of ']' tokens |  |
| TOK_LBRACE | Number of '{' tokens |  |
| TOK_RBRACE | Number of '}' tokens |  |
| TOK_QUESTION | Number of '?' tokens |  |
| TOK_CARET | Number of '^' tokens |  |
| TOK_CARET_ASSIGN | Number of '^=' tokens |  |
| TOK_PIPE | Number of '<code>&#124;</code>' tokens |  |
| TOK_PIPE_ASSIGN | Number of '<code>&#124;</code>=' tokens |  |
| TOK_TILDE | Number of '~' tokens |  |
| TOK_NUMERIC_CONST | Number of numerical constant tokens |  |
| TOK_NUMERIC_CONST_UNIQ | Number of unique numerical constant tokens |  |
| TOK_STRING_LITERALS | Number of string literal tokens |  |
| TOK_STRING_LITERALS_UNIQ | Number of unique string literal tokens |  |
| TOK_UNRESERVED_IDENTIFIERS | Number of (unreserved) identifier tokens |  |
| TOK_UNRESERVED_IDENTIFIERS_UNIQ | Number of unique (unreserved) identifier tokens |  |
| TOK_CHAR_CONSTS | Number of character constants |  |
| TOK_CHAR_CONSTS_UNIQ | Number of unique character constants |  |
| HIS_CALLING | Number of functions which call this function |  |

Raw & Parsed Metrics
====================

Some of the metrics reported have 2 variants - one based on examination of the parsed code after preprocessor expansion and the other examining the unexpanded code.

Take for example:

    #define CHECK_FOR_ERROR( _val ) do { if( _val >= 0 ) { error(); } } while( 0 )
    void some_function( int neg_value )
    {
        CHECK_FOR_ERROR();

The fragment of some_function in the above code does not contain any 'if' tokens until the `CHECK_FOR_ERROR` macro is expanded.

Due to the fact that some people like to treat macros as a form of encapsulation and don't want the contents of macros counting towards function metrics while other people do, CCSM provides metrics for both where practical, allowing the user to pick and choose.


HIS Metrics Support
-------------------

| Metric      | Status                                            | Description     |
|-------------|---------------------------------------------------|-----------------|
| COMF        | Implemented - `TOK_HIS_COMF`                      | Comment density.  Ratio of comments to 'statements' |
| PATH        | Implemented - `FUNC_PATHS`                        | Number of non-cyclic execution paths in the function |
| GOTO        | Implemented - `KW_GOTO_CNT` and `RAW_KW_GOTO_CNT` | Number of GOTO statements |
| v(G)        | Implemented - `MCCABE` and `RAW_MCCABE`           | Cyclomatic complexity     |
| CALLING     | Implemented - `HIS_CALLING`                       | Number of different functions calling this function.  Note that functions called via function pointer are not counted.|
| CALLS       | Implemented - `OP_FN_CALL_CNT`                    | Number of different functions called.  Note that functions called via function pointer are not counted |
| PARAM       | Implemented - `STMT_HIS_PARAM`                    | Number of function parameters.  Note that only one variant of this metric exists as parameters hidden within macros are still parameters. |
| STMT        | Implemented - `TOK_HIS_STMT`                      | Number of instructions per function. |
| LEVEL       | Implemented - `FUNC_DEPTH`                        | Nesting depth within a function.  Note that HIS says that this is defined as "Maximum nesting levels within a function + 1", however the allowable range is 0-4, which doesn't seem consistent.  `FUNC_DEPTH` in this implementation is zero for an empty function |
| RETURN      | Implemented - `RETURN_POINT_CNT`                  | Number of return points in a function.  Note that only one variant of this metric exists as return points hidden within macros are still return points.  Note Issue #54 with respect to this metric. |
| S           | Not planned                                       | |
| VOCF        | Not yet implemented                               | |
| NOMV        | Not planned                                       | |
| NOMVPR      | Not planned                                       | |
| ap_cg_cycle | Not planned                                       | |

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

TODO: Needs to be updated

McCabe Complexity
=================

The `MCCABE` and `RAW_MCCABE` metrics are based on the McCabe method of calculating 
[cyclomatic complexity](http://www.mccabe.com/pdf/mccabe-nist235r.pdf).

> Cyclomatic complexity is defined for each module to be e - n + 2, where e and n are the number
> of edges and nodes in the control flow graph, respectively. [...]
> Cyclomatic complexity is also known as v(G), where v refers to the cyclomatic number in
> graph theory and G indicates that the complexity is a function of the graph.

Modified McCabe, where switch statements are counted as opposed to each individual case statement
are also supported via `MCCABE_MOD` and `RAW_MCCABE_MOD`.

Statement Counting
==================

Statement counting is based on the definition provided in the C language specification, namely

"A statement specifies an action to be performed"

The following are not counted as statements, as they do not result in any operation being performed:
* Compound statements, as by themselves they serve only as a method of creating a block.
* Null statements (consisting of just a semi-colon)
* Declarations
* Labels

Statement counting is frequently used as an indication of how complex the task being implemented by the
code is, hence counting the above items may skew this in some situations (e.g. source code containing
a significant number of null statements could skew the metric while not actually resulting in any actions)

CCSM provides the `STMT_CNT` metric to report the number of statements found in the parsed C code and `RAW_STMT_CNT` to report the number of statements found in the raw source.  Note that as the raw source code has not been pre-processed, there are times when parsing of the code will be ambiguous, for example:

    #define A unsigned
    ...
    A var;

Once expanded, the above is clearly a declaration, as compared to:

    #define A x =
    ...
    A var;

which, once expanded, is not a declaration.

Function and File Scoping
=========================

Considering the following code snippet:

    unsigned local_var;

    unsigned adder( unsigned x, unsigned y );

    unsigned subber( unsigned x, unsigned y );
    {
        unsigned ret_val;
        if( y > x )
        {
            ret_val = 0;
        }
        else
        {
            ret_val = x-y;
        }
        return ret_val;
    }

It's clear that `local_var` is file-scope and should be counted as such in the
metrics.  It's also clear that the body of the `subber` function belongs to that
function (as opposed to being file scope) and should be counted as such.  What's
less clear is how the declaration of `adder` and the prototype for `subber` should
be counted.  Do these count as being function scope or file scope?

By default CCSM will:
* Count declarations within file-scope metrics
* Cound the prototype associated with a definition within the function-scope
metrics.

The second behaviour may be modified via command line parameter:
* `--prototypes-are-filescope` will prevent the prototype of functions from
counting towards function metrics; it will, instead, be included in the
file-level metrics

Comparison To Other Tools
=========================

Other tools are available which analyse C code for metrics.  It's possible that the results for some of the metrics will vary slightly between the tools.

Useful Links
============

[1]: http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf  "C99 Draft"
