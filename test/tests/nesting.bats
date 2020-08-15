load test_helper.bash

EXPECTED=$(cat <<EOF
Global
File: src\nesting.c
	Function: elifer
		Nesting Level: 2
	Function: elser
		Nesting Level: 4
	Function: singleIf
		Nesting Level: 1
File: src\nesting.h
File: src\nesting2.c
	Function: something
		Nesting Level: 7
EOF
)

@test "Function Nesting" {
  run $CCSM_EXE $TEST_SOURCE_DIR/nesting.c $TEST_SOURCE_DIR/nesting2.c --output-metrics=FUNC_NESTING --
  assert_success
  assert_ccsm_output "$EXPECTED"
}