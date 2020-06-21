load test_helper.bash

EXPECTED=$(cat <<EOF
Global
Number of (unreserved) identifier tokens: 20
Number of unique (unreserved) identifier tokens: 10
Number of (unreserved) identifier tokens (raw source): 17
Number of unique (unreserved) identifier tokens (raw source): 11
File: src\empty.c
	Number of (unreserved) identifier tokens: 0
	Number of unique (unreserved) identifier tokens: 0
	Number of (unreserved) identifier tokens (raw source): 0
	Number of unique (unreserved) identifier tokens (raw source): 0
File: src\unreserved_idents.c
	Number of (unreserved) identifier tokens: 20
	Number of unique (unreserved) identifier tokens: 10
	Number of (unreserved) identifier tokens (raw source): 17
	Number of unique (unreserved) identifier tokens (raw source): 11
	Function: func_a
		Number of (unreserved) identifier tokens: 1
		Number of unique (unreserved) identifier tokens: 1
		Number of (unreserved) identifier tokens (raw source): 1
		Number of unique (unreserved) identifier tokens (raw source): 1
	Function: func_b
		Number of (unreserved) identifier tokens: 3
		Number of unique (unreserved) identifier tokens: 3
		Number of (unreserved) identifier tokens (raw source): 2
		Number of unique (unreserved) identifier tokens (raw source): 2
	Function: func_c
		Number of (unreserved) identifier tokens: 3
		Number of unique (unreserved) identifier tokens: 3
		Number of (unreserved) identifier tokens (raw source): 3
		Number of unique (unreserved) identifier tokens (raw source): 3
	Function: func_d
		Number of (unreserved) identifier tokens: 6
		Number of unique (unreserved) identifier tokens: 5
		Number of (unreserved) identifier tokens (raw source): 5
		Number of unique (unreserved) identifier tokens (raw source): 4
	Function: func_e
		Number of (unreserved) identifier tokens: 3
		Number of unique (unreserved) identifier tokens: 3
		Number of (unreserved) identifier tokens (raw source): 2
		Number of unique (unreserved) identifier tokens (raw source): 2
EOF
)

@test "Unreserved identifiers" {
  run $CCSM_EXE $TEST_SOURCE_DIR/unreserved_idents.c  $TEST_SOURCE_DIR/empty.c --output-metrics=UNRESERVED_IDENTIFIERS,UNRESERVED_IDENTIFIERS_UNIQ,RAW_UNRESERVED_IDENTIFIERS,RAW_UNRESERVED_IDENTIFIERS_UNIQ --
  assert_success
  assert_ccsm_output "$EXPECTED"
}