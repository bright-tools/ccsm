load test_helper.bash

EXPECTED=$(cat <<EOF
Global
File: src\while_zero.c
	Function: foo
		Number of paths through the function: 1
EOF
)

@test "Path count for non-looping do() and while()" {
  run $CCSM_EXE $TEST_SOURCE_DIR/while_zero.c --output-metrics=FUNC_PATHS --
  assert_success
  assert_ccsm_output "$EXPECTED"
}