load test_helper.bash

# TODO: Fix code for this test case - body has >0 lines

EXPECTED=$(cat <<EOL
Global
File: src\nested_macros.h
	Function: this_is_a_prefix_major
		Function (definition) size in lines: 0
		Function (body) size in lines: 0
EOL
)

@test "Nested macros - torture case" {
  run $CCSM_EXE "$TEST_SOURCE_DIR/nested_macros.h" --output-metrics=FUNC_BODY_LINE_CNT,FUNC_DEFINITION_LINE_CNT --
  assert_success
  assert_ccsm_output "$EXPECTED"
}