load test_helper.bash

EXPECTED=$(cat <<EOF
Global
Number of instances of numeric constants: 0
Number of unique numerical constant tokens: 0
Number of string literal tokens: 0
Number of unique string literal tokens: 0
Number of character constants: 0
Number of unique character constants: 0
File: src\vocf1.c
	Number of instances of numeric constants(local): 0
	Number of instances of numeric constants(cumulative): 0
	Number of unique numerical constant tokens(local): 0
	Number of unique numerical constant tokens(cumulative): 0
	Number of string literal tokens(local): 0
	Number of string literal tokens(cumulative): 0
	Number of unique string literal tokens(local): 0
	Number of unique string literal tokens(cumulative): 0
	Number of character constants: 0
	Number of unique character constants: 0
	Function: arith
		Number of instances of numeric constants: 0
		Number of unique numerical constant tokens: 0
		Number of string literal tokens: 0
		Number of unique string literal tokens: 0
		Number of character constants: 0
		Number of unique character constants: 0
		Number of (unreserved) identifier tokens (within function body): 25
		Number of unique (unreserved) identifier tokens (within function body): 3
		VOCF: 4.666
		Number of different types of halstead operators: 5
		Number of halstead operators: 16
		Number of different halstead operands: 4
		Number of halstead operands: 26
	Function: arith_assign
		Number of instances of numeric constants: 0
		Number of unique numerical constant tokens: 0
		Number of string literal tokens: 0
		Number of unique string literal tokens: 0
		Number of character constants: 0
		Number of unique character constants: 0
		Number of (unreserved) identifier tokens (within function body): 18
		Number of unique (unreserved) identifier tokens (within function body): 3
		VOCF: 3.111
		Number of different types of halstead operators: 5
		Number of halstead operators: 9
		Number of different halstead operands: 4
		Number of halstead operands: 19
	Function: empty
		Number of instances of numeric constants: 0
		Number of unique numerical constant tokens: 0
		Number of string literal tokens: 0
		Number of unique string literal tokens: 0
		Number of character constants: 0
		Number of unique character constants: 0
		Number of (unreserved) identifier tokens (within function body): 0
		Number of unique (unreserved) identifier tokens (within function body): 0
		VOCF: N/A
		Number of different types of halstead operators: 0
		Number of halstead operators: 0
		Number of different halstead operands: 0
		Number of halstead operands: 0
	Function: qual
		Number of instances of numeric constants: 0
		Number of unique numerical constant tokens: 0
		Number of string literal tokens: 0
		Number of unique string literal tokens: 0
		Number of character constants: 0
		Number of unique character constants: 0
		Number of (unreserved) identifier tokens (within function body): 2
		Number of unique (unreserved) identifier tokens (within function body): 2
		VOCF: 1.2
		Number of different types of halstead operators: 2
		Number of halstead operators: 2
		Number of different halstead operands: 3
		Number of halstead operands: 4
	Function: storage
		Number of instances of numeric constants: 0
		Number of unique numerical constant tokens: 0
		Number of string literal tokens: 0
		Number of unique string literal tokens: 0
		Number of character constants: 0
		Number of unique character constants: 0
		Number of (unreserved) identifier tokens (within function body): 5
		Number of unique (unreserved) identifier tokens (within function body): 5
		VOCF: 1.363
		Number of different types of halstead operators: 5
		Number of halstead operators: 5
		Number of different halstead operands: 6
		Number of halstead operands: 10
EOF
)

@test "VOCF" {
  run $CCSM_EXE $TEST_SOURCE_DIR/vocf1.c --output-metrics=NUMERIC_CONST_CNT,NUMERIC_CONST_UNIQ,STRING_LITERALS,STRING_LITERALS_UNIQ,CHAR_CONSTS,CHAR_CONSTS_UNIQ,BODY_UNRESERVED_IDENTIFIERS,BODY_UNRESERVED_IDENTIFIERS_UNIQ,HALSTEAD_OPERATOR_UNIQUE_CNT,HALSTEAD_OPERATOR_CNT,HIS_VOCF,HALSTEAD_OPERAND_UNIQUE_CNT,HALSTEAD_OPERAND_CNT --
  assert_success
  assert_ccsm_output "$EXPECTED"
}