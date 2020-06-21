load test_helper.bash

EXPECTED=$(cat <<EOF
Global
'sizeof' keyword/operator count: 2
'sizeof' keyword/operator count (raw source): 2
Basic assignment operators: 43
Addition operators: 2
Addition with assign operators: 1
Subtraction operators: 2
Subtraction with assign operators: 1
Unary plus operators: 1
Unary minus operators: 1
Multiplication operators: 2
Multiplication with assignment operators: 1
Division operators: 3
Division with assign operators: 1
Modulo operators: 2
Modulo with assign operators: 1
Pre-increment operators: 1
Post-increment operators: 3
Pre-decrement operators: 1
Post-decrement operators: 1
Left shift operators: 1
Left shift with assign operators: 1
Right shift operators: 1
Right shift with assign operators: 1
Less than operators: 1
Greater than operators: 1
Less than or equal operators: 1
Greater than or equal operators: 1
Equality operators: 1
Inequality operators: 1
Comma operators: 1
TERNARY operators: 1
Logical AND operators: 1
Logical OR operators: 1
Logical NOT operators: 2
Bitwise AND operators: 1
Bitwise AND with assign operators: 1
Bitwise OR operators: 1
Bitwise OR with assign operators: 1
Bitwise XOR operators: 1
Bitwise XOR with assign operators: 1
Bitwise NOT operators: 1
Pointer to member (direct) operators: 0
Pointer to member (indirect) operators: 0
Address of operators: 2
Dereference operators: 1
Array subscript operators: 3
Member access (direct) operators: 1
Member access (via pointer) operators: 1
Alignof operators: 0
Cast operators: 1
Number of different types of C operator: 47
Number of operators: 102
Number of 'operator' tokens: 0
File: src/empty.c
	'sizeof' keyword/operator count(local): 0
	'sizeof' keyword/operator count(cumulative): 0
	'sizeof' keyword/operator count (raw source)(local): 0
	'sizeof' keyword/operator count (raw source)(cumulative): 0
	Basic assignment operators: 0
	Addition operators: 0
	Addition with assign operators: 0
	Subtraction operators: 0
	Subtraction with assign operators: 0
	Unary plus operators: 0
	Unary minus operators: 0
	Multiplication operators: 0
	Multiplication with assignment operators: 0
	Division operators: 0
	Division with assign operators: 0
	Modulo operators: 0
	Modulo with assign operators: 0
	Pre-increment operators: 0
	Post-increment operators: 0
	Pre-decrement operators: 0
	Post-decrement operators: 0
	Left shift operators: 0
	Left shift with assign operators: 0
	Right shift operators: 0
	Right shift with assign operators: 0
	Less than operators: 0
	Greater than operators: 0
	Less than or equal operators: 0
	Greater than or equal operators: 0
	Equality operators: 0
	Inequality operators: 0
	Comma operators: 0
	TERNARY operators: 0
	Logical AND operators: 0
	Logical OR operators: 0
	Logical NOT operators: 0
	Bitwise AND operators: 0
	Bitwise AND with assign operators: 0
	Bitwise OR operators: 0
	Bitwise OR with assign operators: 0
	Bitwise XOR operators: 0
	Bitwise XOR with assign operators: 0
	Bitwise NOT operators: 0
	Pointer to member (direct) operators: 0
	Pointer to member (indirect) operators: 0
	Address of operators: 0
	Dereference operators: 0
	Array subscript operators: 0
	Member access (direct) operators: 0
	Member access (via pointer) operators: 0
	Alignof operators: 0
	Cast operators: 0
	Number of different types of C operator(local): 0
	Number of different types of C operator(cumulative): 0
	Number of operators(local): 0
	Number of operators(cumulative): 0
	Number of 'operator' tokens: 0
File: src/oper_cnt.c
	'sizeof' keyword/operator count(local): 0
	'sizeof' keyword/operator count(cumulative): 2
	'sizeof' keyword/operator count (raw source)(local): 0
	'sizeof' keyword/operator count (raw source)(cumulative): 2
	Basic assignment operators: 43
	Addition operators: 2
	Addition with assign operators: 1
	Subtraction operators: 2
	Subtraction with assign operators: 1
	Unary plus operators: 1
	Unary minus operators: 1
	Multiplication operators: 2
	Multiplication with assignment operators: 1
	Division operators: 3
	Division with assign operators: 1
	Modulo operators: 2
	Modulo with assign operators: 1
	Pre-increment operators: 1
	Post-increment operators: 3
	Pre-decrement operators: 1
	Post-decrement operators: 1
	Left shift operators: 1
	Left shift with assign operators: 1
	Right shift operators: 1
	Right shift with assign operators: 1
	Less than operators: 1
	Greater than operators: 1
	Less than or equal operators: 1
	Greater than or equal operators: 1
	Equality operators: 1
	Inequality operators: 1
	Comma operators: 1
	TERNARY operators: 1
	Logical AND operators: 1
	Logical OR operators: 1
	Logical NOT operators: 2
	Bitwise AND operators: 1
	Bitwise AND with assign operators: 1
	Bitwise OR operators: 1
	Bitwise OR with assign operators: 1
	Bitwise XOR operators: 1
	Bitwise XOR with assign operators: 1
	Bitwise NOT operators: 1
	Pointer to member (direct) operators: 0
	Pointer to member (indirect) operators: 0
	Address of operators: 2
	Dereference operators: 1
	Array subscript operators: 3
	Member access (direct) operators: 1
	Member access (via pointer) operators: 1
	Alignof operators: 0
	Cast operators: 1
	Number of different types of C operator(local): 2
	Number of different types of C operator(cumulative): 47
	Number of operators(local): 2
	Number of operators(cumulative): 102
	Number of 'operator' tokens: 0
	Function: arithmetic
		'sizeof' keyword/operator count: 0
		'sizeof' keyword/operator count (raw source): 0
		Function Calls: 0
		No. different functions called: 0
		Basic assignment operators: 8
		Addition operators: 1
		Addition with assign operators: 0
		Subtraction operators: 1
		Subtraction with assign operators: 0
		Unary plus operators: 1
		Unary minus operators: 1
		Multiplication operators: 1
		Multiplication with assignment operators: 0
		Division operators: 1
		Division with assign operators: 0
		Modulo operators: 1
		Modulo with assign operators: 0
		Pre-increment operators: 1
		Post-increment operators: 1
		Pre-decrement operators: 1
		Post-decrement operators: 1
		Left shift operators: 0
		Left shift with assign operators: 0
		Right shift operators: 0
		Right shift with assign operators: 0
		Less than operators: 0
		Greater than operators: 0
		Less than or equal operators: 0
		Greater than or equal operators: 0
		Equality operators: 0
		Inequality operators: 0
		Comma operators: 0
		TERNARY operators: 0
		Logical AND operators: 0
		Logical OR operators: 0
		Logical NOT operators: 0
		Bitwise AND operators: 0
		Bitwise AND with assign operators: 0
		Bitwise OR operators: 0
		Bitwise OR with assign operators: 0
		Bitwise XOR operators: 0
		Bitwise XOR with assign operators: 0
		Bitwise NOT operators: 0
		Address of operators: 0
		Dereference operators: 0
		Array subscript operators: 0
		Member access (direct) operators: 0
		Member access (via pointer) operators: 0
		Alignof operators: 0
		Cast operators: 0
		Number of different types of C operator: 12
		Number of operators: 19
		Number of different types of halstead operators: 12
		Number of halstead operators: 19
		Number of different halstead operands: 5
		Number of halstead operands: 26
		Number of 'operator' tokens: 0
	Function: arithmetic_init_add
		'sizeof' keyword/operator count: 0
		'sizeof' keyword/operator count (raw source): 0
		Function Calls: 0
		No. different functions called: 0
		Basic assignment operators: 1
		Addition operators: 1
		Addition with assign operators: 0
		Subtraction operators: 0
		Subtraction with assign operators: 0
		Unary plus operators: 0
		Unary minus operators: 0
		Multiplication operators: 0
		Multiplication with assignment operators: 0
		Division operators: 0
		Division with assign operators: 0
		Modulo operators: 0
		Modulo with assign operators: 0
		Pre-increment operators: 0
		Post-increment operators: 0
		Pre-decrement operators: 0
		Post-decrement operators: 0
		Left shift operators: 0
		Left shift with assign operators: 0
		Right shift operators: 0
		Right shift with assign operators: 0
		Less than operators: 0
		Greater than operators: 0
		Less than or equal operators: 0
		Greater than or equal operators: 0
		Equality operators: 0
		Inequality operators: 0
		Comma operators: 0
		TERNARY operators: 0
		Logical AND operators: 0
		Logical OR operators: 0
		Logical NOT operators: 0
		Bitwise AND operators: 0
		Bitwise AND with assign operators: 0
		Bitwise OR operators: 0
		Bitwise OR with assign operators: 0
		Bitwise XOR operators: 0
		Bitwise XOR with assign operators: 0
		Bitwise NOT operators: 0
		Address of operators: 0
		Dereference operators: 0
		Array subscript operators: 0
		Member access (direct) operators: 0
		Member access (via pointer) operators: 0
		Alignof operators: 0
		Cast operators: 0
		Number of different types of C operator: 2
		Number of operators: 2
		Number of different types of halstead operators: 2
		Number of halstead operators: 2
		Number of different halstead operands: 3
		Number of halstead operands: 4
		Number of 'operator' tokens: 0
	Function: arithmetic_init_div
		'sizeof' keyword/operator count: 0
		'sizeof' keyword/operator count (raw source): 0
		Function Calls: 0
		No. different functions called: 0
		Basic assignment operators: 1
		Addition operators: 0
		Addition with assign operators: 0
		Subtraction operators: 0
		Subtraction with assign operators: 0
		Unary plus operators: 0
		Unary minus operators: 0
		Multiplication operators: 0
		Multiplication with assignment operators: 0
		Division operators: 1
		Division with assign operators: 0
		Modulo operators: 0
		Modulo with assign operators: 0
		Pre-increment operators: 0
		Post-increment operators: 0
		Pre-decrement operators: 0
		Post-decrement operators: 0
		Left shift operators: 0
		Left shift with assign operators: 0
		Right shift operators: 0
		Right shift with assign operators: 0
		Less than operators: 0
		Greater than operators: 0
		Less than or equal operators: 0
		Greater than or equal operators: 0
		Equality operators: 0
		Inequality operators: 0
		Comma operators: 0
		TERNARY operators: 0
		Logical AND operators: 0
		Logical OR operators: 0
		Logical NOT operators: 0
		Bitwise AND operators: 0
		Bitwise AND with assign operators: 0
		Bitwise OR operators: 0
		Bitwise OR with assign operators: 0
		Bitwise XOR operators: 0
		Bitwise XOR with assign operators: 0
		Bitwise NOT operators: 0
		Address of operators: 0
		Dereference operators: 0
		Array subscript operators: 0
		Member access (direct) operators: 0
		Member access (via pointer) operators: 0
		Alignof operators: 0
		Cast operators: 0
		Number of different types of C operator: 2
		Number of operators: 2
		Number of different types of halstead operators: 2
		Number of halstead operators: 2
		Number of different halstead operands: 3
		Number of halstead operands: 4
		Number of 'operator' tokens: 0
	Function: arithmetic_init_log_not
		'sizeof' keyword/operator count: 0
		'sizeof' keyword/operator count (raw source): 0
		Function Calls: 0
		No. different functions called: 0
		Basic assignment operators: 1
		Addition operators: 0
		Addition with assign operators: 0
		Subtraction operators: 0
		Subtraction with assign operators: 0
		Unary plus operators: 0
		Unary minus operators: 0
		Multiplication operators: 0
		Multiplication with assignment operators: 0
		Division operators: 0
		Division with assign operators: 0
		Modulo operators: 0
		Modulo with assign operators: 0
		Pre-increment operators: 0
		Post-increment operators: 0
		Pre-decrement operators: 0
		Post-decrement operators: 0
		Left shift operators: 0
		Left shift with assign operators: 0
		Right shift operators: 0
		Right shift with assign operators: 0
		Less than operators: 0
		Greater than operators: 0
		Less than or equal operators: 0
		Greater than or equal operators: 0
		Equality operators: 0
		Inequality operators: 0
		Comma operators: 0
		TERNARY operators: 0
		Logical AND operators: 0
		Logical OR operators: 0
		Logical NOT operators: 1
		Bitwise AND operators: 0
		Bitwise AND with assign operators: 0
		Bitwise OR operators: 0
		Bitwise OR with assign operators: 0
		Bitwise XOR operators: 0
		Bitwise XOR with assign operators: 0
		Bitwise NOT operators: 0
		Address of operators: 0
		Dereference operators: 0
		Array subscript operators: 0
		Member access (direct) operators: 0
		Member access (via pointer) operators: 0
		Alignof operators: 0
		Cast operators: 0
		Number of different types of C operator: 2
		Number of operators: 2
		Number of different types of halstead operators: 2
		Number of halstead operators: 2
		Number of different halstead operands: 3
		Number of halstead operands: 3
		Number of 'operator' tokens: 0
	Function: arithmetic_init_mod
		'sizeof' keyword/operator count: 0
		'sizeof' keyword/operator count (raw source): 0
		Function Calls: 0
		No. different functions called: 0
		Basic assignment operators: 1
		Addition operators: 0
		Addition with assign operators: 0
		Subtraction operators: 0
		Subtraction with assign operators: 0
		Unary plus operators: 0
		Unary minus operators: 0
		Multiplication operators: 0
		Multiplication with assignment operators: 0
		Division operators: 0
		Division with assign operators: 0
		Modulo operators: 1
		Modulo with assign operators: 0
		Pre-increment operators: 0
		Post-increment operators: 0
		Pre-decrement operators: 0
		Post-decrement operators: 0
		Left shift operators: 0
		Left shift with assign operators: 0
		Right shift operators: 0
		Right shift with assign operators: 0
		Less than operators: 0
		Greater than operators: 0
		Less than or equal operators: 0
		Greater than or equal operators: 0
		Equality operators: 0
		Inequality operators: 0
		Comma operators: 0
		TERNARY operators: 0
		Logical AND operators: 0
		Logical OR operators: 0
		Logical NOT operators: 0
		Bitwise AND operators: 0
		Bitwise AND with assign operators: 0
		Bitwise OR operators: 0
		Bitwise OR with assign operators: 0
		Bitwise XOR operators: 0
		Bitwise XOR with assign operators: 0
		Bitwise NOT operators: 0
		Address of operators: 0
		Dereference operators: 0
		Array subscript operators: 0
		Member access (direct) operators: 0
		Member access (via pointer) operators: 0
		Alignof operators: 0
		Cast operators: 0
		Number of different types of C operator: 2
		Number of operators: 2
		Number of different types of halstead operators: 2
		Number of halstead operators: 2
		Number of different halstead operands: 3
		Number of halstead operands: 4
		Number of 'operator' tokens: 0
	Function: arithmetic_init_mul
		'sizeof' keyword/operator count: 0
		'sizeof' keyword/operator count (raw source): 0
		Function Calls: 0
		No. different functions called: 0
		Basic assignment operators: 1
		Addition operators: 0
		Addition with assign operators: 0
		Subtraction operators: 0
		Subtraction with assign operators: 0
		Unary plus operators: 0
		Unary minus operators: 0
		Multiplication operators: 1
		Multiplication with assignment operators: 0
		Division operators: 0
		Division with assign operators: 0
		Modulo operators: 0
		Modulo with assign operators: 0
		Pre-increment operators: 0
		Post-increment operators: 0
		Pre-decrement operators: 0
		Post-decrement operators: 0
		Left shift operators: 0
		Left shift with assign operators: 0
		Right shift operators: 0
		Right shift with assign operators: 0
		Less than operators: 0
		Greater than operators: 0
		Less than or equal operators: 0
		Greater than or equal operators: 0
		Equality operators: 0
		Inequality operators: 0
		Comma operators: 0
		TERNARY operators: 0
		Logical AND operators: 0
		Logical OR operators: 0
		Logical NOT operators: 0
		Bitwise AND operators: 0
		Bitwise AND with assign operators: 0
		Bitwise OR operators: 0
		Bitwise OR with assign operators: 0
		Bitwise XOR operators: 0
		Bitwise XOR with assign operators: 0
		Bitwise NOT operators: 0
		Address of operators: 0
		Dereference operators: 0
		Array subscript operators: 0
		Member access (direct) operators: 0
		Member access (via pointer) operators: 0
		Alignof operators: 0
		Cast operators: 0
		Number of different types of C operator: 2
		Number of operators: 2
		Number of different types of halstead operators: 2
		Number of halstead operators: 2
		Number of different halstead operands: 3
		Number of halstead operands: 4
		Number of 'operator' tokens: 0
	Function: arithmetic_init_sizeof
		'sizeof' keyword/operator count: 1
		'sizeof' keyword/operator count (raw source): 1
		Function Calls: 0
		No. different functions called: 0
		Basic assignment operators: 1
		Addition operators: 0
		Addition with assign operators: 0
		Subtraction operators: 0
		Subtraction with assign operators: 0
		Unary plus operators: 0
		Unary minus operators: 0
		Multiplication operators: 0
		Multiplication with assignment operators: 0
		Division operators: 0
		Division with assign operators: 0
		Modulo operators: 0
		Modulo with assign operators: 0
		Pre-increment operators: 0
		Post-increment operators: 0
		Pre-decrement operators: 0
		Post-decrement operators: 0
		Left shift operators: 0
		Left shift with assign operators: 0
		Right shift operators: 0
		Right shift with assign operators: 0
		Less than operators: 0
		Greater than operators: 0
		Less than or equal operators: 0
		Greater than or equal operators: 0
		Equality operators: 0
		Inequality operators: 0
		Comma operators: 0
		TERNARY operators: 0
		Logical AND operators: 0
		Logical OR operators: 0
		Logical NOT operators: 0
		Bitwise AND operators: 0
		Bitwise AND with assign operators: 0
		Bitwise OR operators: 0
		Bitwise OR with assign operators: 0
		Bitwise XOR operators: 0
		Bitwise XOR with assign operators: 0
		Bitwise NOT operators: 0
		Address of operators: 0
		Dereference operators: 0
		Array subscript operators: 0
		Member access (direct) operators: 0
		Member access (via pointer) operators: 0
		Alignof operators: 0
		Cast operators: 0
		Number of different types of C operator: 2
		Number of operators: 2
		Number of different types of halstead operators: 2
		Number of halstead operators: 2
		Number of different halstead operands: 2
		Number of halstead operands: 3
		Number of 'operator' tokens: 0
	Function: arithmetic_init_sub
		'sizeof' keyword/operator count: 0
		'sizeof' keyword/operator count (raw source): 0
		Function Calls: 0
		No. different functions called: 0
		Basic assignment operators: 1
		Addition operators: 0
		Addition with assign operators: 0
		Subtraction operators: 1
		Subtraction with assign operators: 0
		Unary plus operators: 0
		Unary minus operators: 0
		Multiplication operators: 0
		Multiplication with assignment operators: 0
		Division operators: 0
		Division with assign operators: 0
		Modulo operators: 0
		Modulo with assign operators: 0
		Pre-increment operators: 0
		Post-increment operators: 0
		Pre-decrement operators: 0
		Post-decrement operators: 0
		Left shift operators: 0
		Left shift with assign operators: 0
		Right shift operators: 0
		Right shift with assign operators: 0
		Less than operators: 0
		Greater than operators: 0
		Less than or equal operators: 0
		Greater than or equal operators: 0
		Equality operators: 0
		Inequality operators: 0
		Comma operators: 0
		TERNARY operators: 0
		Logical AND operators: 0
		Logical OR operators: 0
		Logical NOT operators: 0
		Bitwise AND operators: 0
		Bitwise AND with assign operators: 0
		Bitwise OR operators: 0
		Bitwise OR with assign operators: 0
		Bitwise XOR operators: 0
		Bitwise XOR with assign operators: 0
		Bitwise NOT operators: 0
		Address of operators: 0
		Dereference operators: 0
		Array subscript operators: 0
		Member access (direct) operators: 0
		Member access (via pointer) operators: 0
		Alignof operators: 0
		Cast operators: 0
		Number of different types of C operator: 2
		Number of operators: 2
		Number of different types of halstead operators: 2
		Number of halstead operators: 2
		Number of different halstead operands: 3
		Number of halstead operands: 4
		Number of 'operator' tokens: 0
	Function: array
		'sizeof' keyword/operator count: 0
		'sizeof' keyword/operator count (raw source): 0
		Function Calls: 0
		No. different functions called: 0
		Basic assignment operators: 3
		Addition operators: 0
		Addition with assign operators: 0
		Subtraction operators: 0
		Subtraction with assign operators: 0
		Unary plus operators: 0
		Unary minus operators: 0
		Multiplication operators: 0
		Multiplication with assignment operators: 0
		Division operators: 0
		Division with assign operators: 0
		Modulo operators: 0
		Modulo with assign operators: 0
		Pre-increment operators: 0
		Post-increment operators: 0
		Pre-decrement operators: 0
		Post-decrement operators: 0
		Left shift operators: 0
		Left shift with assign operators: 0
		Right shift operators: 0
		Right shift with assign operators: 0
		Less than operators: 0
		Greater than operators: 0
		Less than or equal operators: 0
		Greater than or equal operators: 0
		Equality operators: 0
		Inequality operators: 0
		Comma operators: 0
		TERNARY operators: 0
		Logical AND operators: 0
		Logical OR operators: 0
		Logical NOT operators: 0
		Bitwise AND operators: 0
		Bitwise AND with assign operators: 0
		Bitwise OR operators: 0
		Bitwise OR with assign operators: 0
		Bitwise XOR operators: 0
		Bitwise XOR with assign operators: 0
		Bitwise NOT operators: 0
		Address of operators: 1
		Dereference operators: 1
		Array subscript operators: 3
		Member access (direct) operators: 0
		Member access (via pointer) operators: 0
		Alignof operators: 0
		Cast operators: 0
		Number of different types of C operator: 4
		Number of operators: 8
		Number of different types of halstead operators: 4
		Number of halstead operators: 8
		Number of different halstead operands: 6
		Number of halstead operands: 14
		Number of 'operator' tokens: 0
	Function: assign
		'sizeof' keyword/operator count: 0
		'sizeof' keyword/operator count (raw source): 0
		Function Calls: 0
		No. different functions called: 0
		Basic assignment operators: 1
		Addition operators: 0
		Addition with assign operators: 1
		Subtraction operators: 0
		Subtraction with assign operators: 1
		Unary plus operators: 0
		Unary minus operators: 0
		Multiplication operators: 0
		Multiplication with assignment operators: 1
		Division operators: 0
		Division with assign operators: 1
		Modulo operators: 0
		Modulo with assign operators: 1
		Pre-increment operators: 0
		Post-increment operators: 0
		Pre-decrement operators: 0
		Post-decrement operators: 0
		Left shift operators: 0
		Left shift with assign operators: 1
		Right shift operators: 0
		Right shift with assign operators: 1
		Less than operators: 0
		Greater than operators: 0
		Less than or equal operators: 0
		Greater than or equal operators: 0
		Equality operators: 0
		Inequality operators: 0
		Comma operators: 0
		TERNARY operators: 0
		Logical AND operators: 0
		Logical OR operators: 0
		Logical NOT operators: 0
		Bitwise AND operators: 0
		Bitwise AND with assign operators: 1
		Bitwise OR operators: 0
		Bitwise OR with assign operators: 1
		Bitwise XOR operators: 0
		Bitwise XOR with assign operators: 1
		Bitwise NOT operators: 0
		Address of operators: 0
		Dereference operators: 0
		Array subscript operators: 0
		Member access (direct) operators: 0
		Member access (via pointer) operators: 0
		Alignof operators: 0
		Cast operators: 0
		Number of different types of C operator: 11
		Number of operators: 11
		Number of different types of halstead operators: 11
		Number of halstead operators: 11
		Number of different halstead operands: 4
		Number of halstead operands: 23
		Number of 'operator' tokens: 0
	Function: bitwise
		'sizeof' keyword/operator count: 0
		'sizeof' keyword/operator count (raw source): 0
		Function Calls: 0
		No. different functions called: 0
		Basic assignment operators: 7
		Addition operators: 0
		Addition with assign operators: 0
		Subtraction operators: 0
		Subtraction with assign operators: 0
		Unary plus operators: 0
		Unary minus operators: 0
		Multiplication operators: 0
		Multiplication with assignment operators: 0
		Division operators: 0
		Division with assign operators: 0
		Modulo operators: 0
		Modulo with assign operators: 0
		Pre-increment operators: 0
		Post-increment operators: 0
		Pre-decrement operators: 0
		Post-decrement operators: 0
		Left shift operators: 1
		Left shift with assign operators: 0
		Right shift operators: 1
		Right shift with assign operators: 0
		Less than operators: 0
		Greater than operators: 0
		Less than or equal operators: 0
		Greater than or equal operators: 0
		Equality operators: 0
		Inequality operators: 0
		Comma operators: 0
		TERNARY operators: 0
		Logical AND operators: 0
		Logical OR operators: 0
		Logical NOT operators: 0
		Bitwise AND operators: 1
		Bitwise AND with assign operators: 0
		Bitwise OR operators: 1
		Bitwise OR with assign operators: 0
		Bitwise XOR operators: 1
		Bitwise XOR with assign operators: 0
		Bitwise NOT operators: 1
		Address of operators: 0
		Dereference operators: 0
		Array subscript operators: 0
		Member access (direct) operators: 0
		Member access (via pointer) operators: 0
		Alignof operators: 0
		Cast operators: 0
		Number of different types of C operator: 7
		Number of operators: 13
		Number of different types of halstead operators: 7
		Number of halstead operators: 13
		Number of different halstead operands: 6
		Number of halstead operands: 20
		Number of 'operator' tokens: 0
	Function: comp
		'sizeof' keyword/operator count: 0
		'sizeof' keyword/operator count (raw source): 0
		Function Calls: 0
		No. different functions called: 0
		Basic assignment operators: 7
		Addition operators: 0
		Addition with assign operators: 0
		Subtraction operators: 0
		Subtraction with assign operators: 0
		Unary plus operators: 0
		Unary minus operators: 0
		Multiplication operators: 0
		Multiplication with assignment operators: 0
		Division operators: 0
		Division with assign operators: 0
		Modulo operators: 0
		Modulo with assign operators: 0
		Pre-increment operators: 0
		Post-increment operators: 0
		Pre-decrement operators: 0
		Post-decrement operators: 0
		Left shift operators: 0
		Left shift with assign operators: 0
		Right shift operators: 0
		Right shift with assign operators: 0
		Less than operators: 1
		Greater than operators: 1
		Less than or equal operators: 1
		Greater than or equal operators: 1
		Equality operators: 1
		Inequality operators: 1
		Comma operators: 0
		TERNARY operators: 0
		Logical AND operators: 0
		Logical OR operators: 0
		Logical NOT operators: 0
		Bitwise AND operators: 0
		Bitwise AND with assign operators: 0
		Bitwise OR operators: 0
		Bitwise OR with assign operators: 0
		Bitwise XOR operators: 0
		Bitwise XOR with assign operators: 0
		Bitwise NOT operators: 0
		Address of operators: 0
		Dereference operators: 0
		Array subscript operators: 0
		Member access (direct) operators: 0
		Member access (via pointer) operators: 0
		Alignof operators: 0
		Cast operators: 0
		Number of different types of C operator: 7
		Number of operators: 13
		Number of different types of halstead operators: 7
		Number of halstead operators: 13
		Number of different halstead operands: 4
		Number of halstead operands: 23
		Number of 'operator' tokens: 0
	Function: fn_call
		'sizeof' keyword/operator count: 0
		'sizeof' keyword/operator count (raw source): 0
		Function Calls: 1
		No. different functions called: 1
		Basic assignment operators: 0
		Addition operators: 0
		Addition with assign operators: 0
		Subtraction operators: 0
		Subtraction with assign operators: 0
		Unary plus operators: 0
		Unary minus operators: 0
		Multiplication operators: 0
		Multiplication with assignment operators: 0
		Division operators: 0
		Division with assign operators: 0
		Modulo operators: 0
		Modulo with assign operators: 0
		Pre-increment operators: 0
		Post-increment operators: 0
		Pre-decrement operators: 0
		Post-decrement operators: 0
		Left shift operators: 0
		Left shift with assign operators: 0
		Right shift operators: 0
		Right shift with assign operators: 0
		Less than operators: 0
		Greater than operators: 0
		Less than or equal operators: 0
		Greater than or equal operators: 0
		Equality operators: 0
		Inequality operators: 0
		Comma operators: 0
		TERNARY operators: 0
		Logical AND operators: 0
		Logical OR operators: 0
		Logical NOT operators: 0
		Bitwise AND operators: 0
		Bitwise AND with assign operators: 0
		Bitwise OR operators: 0
		Bitwise OR with assign operators: 0
		Bitwise XOR operators: 0
		Bitwise XOR with assign operators: 0
		Bitwise NOT operators: 0
		Address of operators: 0
		Dereference operators: 0
		Array subscript operators: 0
		Member access (direct) operators: 0
		Member access (via pointer) operators: 0
		Alignof operators: 0
		Cast operators: 0
		Number of different types of C operator: 1
		Number of operators: 1
		Number of different types of halstead operators: 1
		Number of halstead operators: 1
		Number of different halstead operands: 1
		Number of halstead operands: 1
		Number of 'operator' tokens: 0
	Function: logic
		'sizeof' keyword/operator count: 0
		'sizeof' keyword/operator count (raw source): 0
		Function Calls: 0
		No. different functions called: 0
		Basic assignment operators: 4
		Addition operators: 0
		Addition with assign operators: 0
		Subtraction operators: 0
		Subtraction with assign operators: 0
		Unary plus operators: 0
		Unary minus operators: 0
		Multiplication operators: 0
		Multiplication with assignment operators: 0
		Division operators: 0
		Division with assign operators: 0
		Modulo operators: 0
		Modulo with assign operators: 0
		Pre-increment operators: 0
		Post-increment operators: 0
		Pre-decrement operators: 0
		Post-decrement operators: 0
		Left shift operators: 0
		Left shift with assign operators: 0
		Right shift operators: 0
		Right shift with assign operators: 0
		Less than operators: 0
		Greater than operators: 0
		Less than or equal operators: 0
		Greater than or equal operators: 0
		Equality operators: 0
		Inequality operators: 0
		Comma operators: 0
		TERNARY operators: 0
		Logical AND operators: 1
		Logical OR operators: 1
		Logical NOT operators: 1
		Bitwise AND operators: 0
		Bitwise AND with assign operators: 0
		Bitwise OR operators: 0
		Bitwise OR with assign operators: 0
		Bitwise XOR operators: 0
		Bitwise XOR with assign operators: 0
		Bitwise NOT operators: 0
		Address of operators: 0
		Dereference operators: 0
		Array subscript operators: 0
		Member access (direct) operators: 0
		Member access (via pointer) operators: 0
		Alignof operators: 0
		Cast operators: 0
		Number of different types of C operator: 4
		Number of operators: 7
		Number of different types of halstead operators: 4
		Number of halstead operators: 7
		Number of different halstead operands: 4
		Number of halstead operands: 11
		Number of 'operator' tokens: 0
	Function: main
		'sizeof' keyword/operator count: 0
		'sizeof' keyword/operator count (raw source): 0
		Function Calls: 0
		No. different functions called: 0
		Basic assignment operators: 0
		Addition operators: 0
		Addition with assign operators: 0
		Subtraction operators: 0
		Subtraction with assign operators: 0
		Unary plus operators: 0
		Unary minus operators: 0
		Multiplication operators: 0
		Multiplication with assignment operators: 0
		Division operators: 0
		Division with assign operators: 0
		Modulo operators: 0
		Modulo with assign operators: 0
		Pre-increment operators: 0
		Post-increment operators: 0
		Pre-decrement operators: 0
		Post-decrement operators: 0
		Left shift operators: 0
		Left shift with assign operators: 0
		Right shift operators: 0
		Right shift with assign operators: 0
		Less than operators: 0
		Greater than operators: 0
		Less than or equal operators: 0
		Greater than or equal operators: 0
		Equality operators: 0
		Inequality operators: 0
		Comma operators: 0
		TERNARY operators: 0
		Logical AND operators: 0
		Logical OR operators: 0
		Logical NOT operators: 0
		Bitwise AND operators: 0
		Bitwise AND with assign operators: 0
		Bitwise OR operators: 0
		Bitwise OR with assign operators: 0
		Bitwise XOR operators: 0
		Bitwise XOR with assign operators: 0
		Bitwise NOT operators: 0
		Address of operators: 0
		Dereference operators: 0
		Array subscript operators: 0
		Member access (direct) operators: 0
		Member access (via pointer) operators: 0
		Alignof operators: 0
		Cast operators: 0
		Number of different types of C operator: 0
		Number of operators: 0
		Number of different types of halstead operators: 0
		Number of halstead operators: 0
		Number of different halstead operands: 0
		Number of halstead operands: 0
		Number of 'operator' tokens: 0
	Function: misc
		'sizeof' keyword/operator count: 1
		'sizeof' keyword/operator count (raw source): 1
		Function Calls: 0
		No. different functions called: 0
		Basic assignment operators: 2
		Addition operators: 0
		Addition with assign operators: 0
		Subtraction operators: 0
		Subtraction with assign operators: 0
		Unary plus operators: 0
		Unary minus operators: 0
		Multiplication operators: 0
		Multiplication with assignment operators: 0
		Division operators: 0
		Division with assign operators: 0
		Modulo operators: 0
		Modulo with assign operators: 0
		Pre-increment operators: 0
		Post-increment operators: 2
		Pre-decrement operators: 0
		Post-decrement operators: 0
		Left shift operators: 0
		Left shift with assign operators: 0
		Right shift operators: 0
		Right shift with assign operators: 0
		Less than operators: 0
		Greater than operators: 0
		Less than or equal operators: 0
		Greater than or equal operators: 0
		Equality operators: 0
		Inequality operators: 0
		Comma operators: 1
		TERNARY operators: 1
		Logical AND operators: 0
		Logical OR operators: 0
		Logical NOT operators: 0
		Bitwise AND operators: 0
		Bitwise AND with assign operators: 0
		Bitwise OR operators: 0
		Bitwise OR with assign operators: 0
		Bitwise XOR operators: 0
		Bitwise XOR with assign operators: 0
		Bitwise NOT operators: 0
		Address of operators: 0
		Dereference operators: 0
		Array subscript operators: 0
		Member access (direct) operators: 0
		Member access (via pointer) operators: 0
		Alignof operators: 0
		Cast operators: 1
		Number of different types of C operator: 6
		Number of operators: 8
		Number of different types of halstead operators: 6
		Number of halstead operators: 8
		Number of different halstead operands: 4
		Number of halstead operands: 10
		Number of 'operator' tokens: 0
	Function: structs
		'sizeof' keyword/operator count: 0
		'sizeof' keyword/operator count (raw source): 0
		Function Calls: 0
		No. different functions called: 0
		Basic assignment operators: 3
		Addition operators: 0
		Addition with assign operators: 0
		Subtraction operators: 0
		Subtraction with assign operators: 0
		Unary plus operators: 0
		Unary minus operators: 0
		Multiplication operators: 0
		Multiplication with assignment operators: 0
		Division operators: 0
		Division with assign operators: 0
		Modulo operators: 0
		Modulo with assign operators: 0
		Pre-increment operators: 0
		Post-increment operators: 0
		Pre-decrement operators: 0
		Post-decrement operators: 0
		Left shift operators: 0
		Left shift with assign operators: 0
		Right shift operators: 0
		Right shift with assign operators: 0
		Less than operators: 0
		Greater than operators: 0
		Less than or equal operators: 0
		Greater than or equal operators: 0
		Equality operators: 0
		Inequality operators: 0
		Comma operators: 0
		TERNARY operators: 0
		Logical AND operators: 0
		Logical OR operators: 0
		Logical NOT operators: 0
		Bitwise AND operators: 0
		Bitwise AND with assign operators: 0
		Bitwise OR operators: 0
		Bitwise OR with assign operators: 0
		Bitwise XOR operators: 0
		Bitwise XOR with assign operators: 0
		Bitwise NOT operators: 0
		Address of operators: 1
		Dereference operators: 0
		Array subscript operators: 0
		Member access (direct) operators: 1
		Member access (via pointer) operators: 1
		Alignof operators: 0
		Cast operators: 0
		Number of different types of C operator: 4
		Number of operators: 6
		Number of different types of halstead operators: 6
		Number of halstead operators: 8
		Number of different halstead operands: 6
		Number of halstead operands: 14
		Number of 'operator' tokens: 0
EOF
)

@test "Operator Counts" {
  run $CCSM $TEST_SOURCE_DIR/oper_cnt.c  $TEST_SOURCE_DIR/empty.c --output-metrics=OP_CNT,OP_TYPES_CNT,OP_* --
  assert_success
  assert_ccsm_output "$EXPECTED"
}