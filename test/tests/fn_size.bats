load test_helper.bash

EXPECTED=$(cat <<EOL
Global
File: src\fn_size.c
	Function: func0
		Function (definition) size in lines: 1
		Function (body) size in lines: 1
	Function: func1
		Function (definition) size in lines: 2
		Function (body) size in lines: 2
	Function: func2
		Function (definition) size in lines: 3
		Function (body) size in lines: 2
	Function: func3
		Function (definition) size in lines: 8
		Function (body) size in lines: 7
EOL
)

@test "Function size in lines" {
  run $CCSM_EXE "$TEST_SOURCE_DIR/fn_size.c" --output-metrics=FUNC_BODY_LINE_CNT,FUNC_DEFINITION_LINE_CNT --
  assert_success
  assert_ccsm_output "$EXPECTED"
}