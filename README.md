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
| COMMENT_BYTE_CNT | Comments in bytes | Counts the total number of bytes which are used in comments.  Note that --exclude-function does not have any effect on this metric. |
| COMMENT_CNT | Comment Count | Counts the total number of comments.  Note that --exclude-function does not have any effect on this metric. |
| FILE_CNT | Files |  |
| FILE_LINE_CNT | Size in lines | Counts number of newlines in the file |
| FILE_SIZE | Size in bytes |  |
| FUNC_CALLED_BY_EXTERN | Number of external functions calling this function |  |
| FUNC_CALLED_BY_LOCAL | Number of local functions calling this function |  |
| FUNC_CNT | Functions |  |
| FUNC_EXTERN_EXPL_CNT | External (explicit) functions declarations |  |
| FUNC_EXTERN_IMPL_CNT | External (implicit) functions declarations |  |
| FUNC_INLINE_CNT | Inline Functions |  |
| FUNC_LINE_CNT | Function (body) size in lines | Counts number of newlines in the function body |
| FUNC_LOCAL_CNT | Local Functions |  |
| FUNC_NESTING | Nesting Level |  |
| HIS_CALLING | Number of functions which call this function |  |
| HIS_VOCF | VOCF |  |
| LOCAL_FN_CALL_CNT | Local Function Calls | Does not include functions called via function pointers |
| OP_ADD_ASSIGN_CNT | Addition with assign operators |  |
| OP_ADD_CNT | Addition operators |  |
| OP_ADDR_OF_CNT | Address of operators |  |
| OP_ALIGNOF_CNT | Alignof operators |  |
| OP_ARRAY_SUBSCRIPT_CNT | Array subscript operators |  |
| OP_ASSIGN_CNT | Basic assignment operators |  |
| OP_BITWISE_AND_ASSIGN_CNT | Bitwise and with assign operators |  |
| OP_BITWISE_AND_CNT | Bitwise and operators |  |
| OP_BITWISE_NIT_CNT | Bitwise not operators |  |
| OP_BITWISE_OR_ASSIGN_CNT | Bitwise or with assign operators |  |
| OP_BITWISE_OR_CNT | Bitwise or operators |  |
| OP_BITWISE_XOR_ASSIGN_CNT | Bitwise xor with assign operators |  |
| OP_BITWISE_XOR_CNT | Bitwise xor operators |  |
| OP_CAST_CNT | Cast operators |  |
| OP_CMP_EQ_CNT | Equality operators |  |
| OP_CMP_GT_CNT | Greater than operators |  |
| OP_CMP_GT_EQ_CNT | Greater than or equal operators |  |
| OP_CMP_LT_CNT | Less than operators |  |
| OP_CMP_LT_EQ_CNT | Less than or equal operators |  |
| OP_CMP_NEQ_CNT | Inequality operators |  |
| OP_COMMA_CNT | Comma operators |  |
| OP_DEC_POST_CNT | Post-decrement operators |  |
| OP_DEC_PRE_CNT | Pre-decrement operators |  |
| OP_DEREF_CNT | Dereference operators |  |
| OP_DIV_ASSIGN_CNT | Division with assign operators |  |
| OP_DIV_CNT | Division operators |  |
| OP_FN_CALL_CNT | Function Calls |  |
| OP_FN_CALL_UNIQUE_CNT | No. different functions called | Of the total number of functions called (see OP_FN_CALL_CNT), this represents the number of uniquely addressed functions.  Does not include functions called via function pointers |
| OP_INC_POST_CNT | Post-increment operators |  |
| OP_INC_PRE_CNT | Pre-increment operators |  |
| OP_LOG_AND_CNT | LOGICAL AND operators |  |
| OP_LOG_NIT_CNT | LOGICAL NOT operators |  |
| OP_LOG_OR_CNT | LOGICAL OR operators |  |
| OP_MEMBER_ACCESS_DIRECT_CNT | Member access (direct) operators |  |
| OP_MEMBER_ACCESS_POINTER_CNT | Member access (via pointer) operators |  |
| OP_MOD_ASSIGN_CNT | Modulo with assign operators |  |
| OP_MOD_CNT | Modulo operators |  |
| OP_MULT_ASSIGN_CNT | Multiplication with assignment operators |  |
| OP_MULT_CNT | Multiplication operators |  |
| OP_PTR_TO_MEMBER_DIRECT_CNT | Pointer to member (direct) operators |  |
| OP_PTR_TO_MEMBER_INDIRECT_CNT | Pointer to member (indirect) operators |  |
| OP_SHFT_LEFT_ASSIGN_CNT | Left shift with assign operators |  |
| OP_SHFT_LEFT_CNT | Left shift operators |  |
| OP_SHFT_RGHT_ASSIGN_CNT | Right shift with assign operators |  |
| OP_SHFT_RGHT_CNT | Right shift operators |  |
| OP_SIZEOF_CNT | Sizeof operators |  |
| OP_SUB_ASSIGN_CNT | Subtraction with assign operators |  |
| OP_SUB_CNT | Subtraction operators |  |
| OP_TERNARY_CNT | TERNARY operators |  |
| OP_TYPES_CNT | Number of different types of C operator |  |
| OP_TYPES_HALSTEAD_CNT | Number of different types of operator (Halstead) |  |
| OP_UNARY_MINUS_CNT | Unary minus operators |  |
| OP_UNARY_PLUS_CNT | Unary plus operators |  |
| RETURN_POINT_CNT | Return points |  |
| STMT_CASE_CNT | CASE statements |  |
| STMT_CNT | Statements |  |
| STMT_DEFAULT_CNT | DEFAULT statements |  |
| STMT_ELSE_CNT | ELSE statements |  |
| STMT_FOR_CNT | FOR loops |  |
| STMT_GOTO_CNT | GOTO statements |  |
| STMT_HIS_PARAM | Function Parameters |  |
| STMT_IF_CNT | IF statements |  |
| STMT_LABEL_CNT | LABEL statements |  |
| STMT_MCCABE | Statement based McCabe |  |
| STMT_MOD_MCCABE | Statement based Modified McCabe |  |
| STMT_RETURN_CNT | RETURN statements |  |
| STMT_SWITCH_CNT | SWITCH statements |  |
| STMT_WHILE_CNT | WHILE loops |  |
| TODO | Decisions |  |
| TODO | Loops |  |
| TOK_AMP | Number of '&' tokens |  |
| TOK_AMPAMP | Number of '&&' tokens |  |
| TOK_AND_ASSIGN | Number of '&=' tokens |  |
| TOK_ASM | Number of 'asm' tokens |  |
| TOK_ASSIGN | Number of '=' tokens |  |
| TOK_ASTERISK | Number of '*' tokens |  |
| TOK_ASTERISK_ASSIGN | Number of '*=' tokens |  |
| TOK_AUTO | Number of 'auto' tokens |  |
| TOK_BOOL | Number of 'bool' tokens |  |
| TOK_BREAK | Number of 'break' tokens |  |
| TOK_CARET | Number of '^' tokens |  |
| TOK_CARET_ASSIGN | Number of '^=' tokens |  |
| TOK_CASE | Number of 'case' tokens |  |
| TOK_CATCH | Number of 'catch' tokens |  |
| TOK_CHAR | Number of 'char' tokens |  |
| TOK_CHAR_CONSTS | Number of character constants |  |
| TOK_CHAR_CONSTS_UNIQ | Number of unique character constants |  |
| TOK_CLASS | Number of 'class' tokens |  |
| TOK_COLON | Number of ':' tokens |  |
| TOK_COLONCOLON | Number of '::' tokens |  |
| TOK_COMMA | Number of ',' tokens |  |
| TOK_COMPARISON | Number of '==' tokens |  |
| TOK_CONST | Number of 'const' tokens |  |
| TOK_CONTINUE | Number of 'continue' tokens |  |
| TOK_DEFAULT | Number of 'default' tokens |  |
| TOK_DELETE | Number of 'delete' tokens |  |
| TOK_DO | Number of 'do' tokens |  |
| TOK_DOUBLE | Number of 'double' tokens |  |
| TOK_ELLIPSIS | Number of '...' tokens |  |
| TOK_ELSE | Number of 'else' tokens |  |
| TOK_ENUM | Number of 'enum' tokens |  |
| TOK_EXPLICIT | Number of 'explicit' tokens |  |
| TOK_EXTERN | Number of 'extern' tokens |  |
| TOK_FALSE | Number of 'false' tokens |  |
| TOK_FLOAT | Number of 'float' tokens |  |
| TOK_FOR | Number of 'for' tokens |  |
| TOK_FRIEND | Number of 'friend' tokens |  |
| TOK_GOTO | Number of 'goto' tokens |  |
| TOK_HIS_COMF | Comment density | Relationship of the number of comments to the number of statements.  HIS metric. |
| TOK_HIS_STMT | Statement count | Number of 'statements' |
| TOK_IF | Number of 'if' tokens |  |
| TOK_INLINE | Number of 'inline' tokens |  |
| TOK_INT | Number of 'int' tokens |  |
| TOK_LBRACE | Number of '{' tokens |  |
| TOK_LESS | Number of '<' tokens |  |
| TOK_LESS_EQUAL | Number of '<=' tokens |  |
| TOK_LESSLESS | Number of '<<' tokens |  |
| TOK_LESSLESS_ASSIGN | Number of '<<=' tokens |  |
| TOK_LONG | Number of 'long' tokens |  |
| TOK_LPAREN | Number of '(' tokens |  |
| TOK_LSQUARE | Number of '[' tokens |  |
| TOK_MCCABE | Token based McCabe |  |
| TOK_MEMBER_PTR | Number of '->' tokens |  |
| TOK_MEMBER_REF | Number of '.' tokens |  |
| TOK_MINUS | Number of '-' tokens |  |
| TOK_MINUS_ASSIGN | Number of '-=' tokens |  |
| TOK_MINUSMINUS | Number of '--' tokens |  |
| TOK_MOD_MCCABE | Token based Modified McCabe |  |
| TOK_MODULO | Number of '%' tokens |  |
| TOK_MODULO_ASSIGN | Number of '%=' tokens |  |
| TOK_MORE | Number of '>' tokens |  |
| TOK_MORE_EQUAL | Number of '>=' tokens |  |
| TOK_MOREMORE | Number of '>>' tokens |  |
| TOK_MOREMORE_ASSIGN | Number of '>>=' tokens |  |
| TOK_MUTABLE | Number of 'mutable' tokens |  |
| TOK_NAMESPACE | Number of 'namespace' tokens |  |
| TOK_NEW | Number of 'new' tokens |  |
| TOK_NOT | Number of '!' tokens |  |
| TOK_NOT_EQUAL | Number of '!=' tokens |  |
| TOK_NUMERIC_CONST | Number of numerical constant tokens |  |
| TOK_NUMERIC_CONST_UNIQ | Number of unique numerical constant tokens |  |
| TOK_OPERATOR | Number of 'operator' tokens |  |
| TOK_PIPE | Number of '|' tokens |  |
| TOK_PIPE_ASSIGN | Number of '|=' tokens |  |
| TOK_PIPEPIPE | Number of '||' tokens |  |
| TOK_PLUS | Number of '+' tokens |  |
| TOK_PLUS_ASSIGN | Number of '+=' tokens |  |
| TOK_PLUSPLUS | Number of '++' tokens |  |
| TOK_PRIVATE | Number of 'private' tokens |  |
| TOK_PROTECTED | Number of 'protected' tokens |  |
| TOK_PUBLIC | Number of 'public' tokens |  |
| TOK_QUESTION | Number of '?' tokens |  |
| TOK_RBRACE | Number of '}' tokens |  |
| TOK_REGISTER | Number of 'register' tokens |  |
| TOK_RETURN | Number of 'return' tokens |  |
| TOK_RPAREN | Number of ')' tokens |  |
| TOK_RSQUARE | Number of ']' tokens |  |
| TOK_SHORT | Number of 'short' tokens |  |
| TOK_SIGNED | Number of 'signed' tokens |  |
| TOK_SIZEOF | Number of 'sizeof' tokens |  |
| TOK_SLASH | Number of '/' tokens |  |
| TOK_SLASH_ASSIGN | Number of '/=' tokens |  |
| TOK_STATIC | Number of 'static' tokens |  |
| TOK_STRING_LITERALS | Number of string literal tokens |  |
| TOK_STRING_LITERALS_UNIQ | Number of unique string literal tokens |  |
| TOK_STRUCT | Number of 'struct' tokens |  |
| TOK_SWITCH | Number of 'switch' tokens |  |
| TOK_TEMPLATE | Number of 'template' tokens |  |
| TOK_THIS | Number of 'this' tokens |  |
| TOK_THROW | Number of 'throw' tokens |  |
| TOK_TILDE | Number of '~' tokens |  |
| TOK_TRUE | Number of 'true' tokens |  |
| TOK_TRY | Number of 'try' tokens |  |
| TOK_TYPEDEF | Number of 'typedef' tokens |  |
| TOK_TYPEID | Number of 'typeid' tokens |  |
| TOK_TYPENAME | Number of 'typename' tokens |  |
| TOK_UNION | Number of 'union' tokens |  |
| TOK_UNRESERVED_IDENTIFIERS | Number of (unreserved) identifier tokens |  |
| TOK_UNRESERVED_IDENTIFIERS_UNIQ | Number of unique (unreserved) identifier tokens |  |
| TOK_UNSIGNED | Number of 'unsigned' tokens |  |
| TOK_USING | Number of 'using' tokens |  |
| TOK_VIRTUAL | Number of 'virtual' tokens |  |
| TOK_VOID | Number of 'void' tokens |  |
| TOK_VOLATILE | Number of 'volatile' tokens |  |
| TOK_WHILE | Number of 'while' tokens |  |
| TYPEDEF_FILE_CNT | File-scope typedefs |  |
| TYPEDEF_FN_CNT | Function-scope typedefs |  |
| VAR_FILE_EXT_CNT | File-scope external variable declarations |  |
| VAR_FILE_LOC_CNT | File-scope total local variable declarations | Includes auto and static but not extern |
| VAR_FILE_LOC_STATIC_CNT | File-scope static variable declarations |  |
| VAR_FN_EXT_CNT | Function-scope external variable declarations |  |
| VAR_FN_LOC_AUTO_CNT | Function-scope auto variable declarations |  |
| VAR_FN_LOC_CNT | Function-scope total local variable declarations | Includes auto and static but not extern |
| VAR_FN_LOC_REG_CNT | Function-scope register variable declarations |  |
| VAR_FN_LOC_STATIC_CNT | Function-scope static variable declarations |  |


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
| CALLING     | Implemented - `HIS_CALLING`                  | Number of different functions calling this function.  Note that functions called via function pointer are not counted.|
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
