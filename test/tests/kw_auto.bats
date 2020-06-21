load test_helper.bash

EXPECTED=$(cat <<EOF
Global
'auto' keyword count: 7
'auto' keyword count (raw source): 4
Function-scope auto variable declarations: 7
File: src\empty.c
	'auto' keyword count: 0
	'auto' keyword count (raw source): 0
File: src\kw_auto.c
	'auto' keyword count: 7
	'auto' keyword count (raw source): 4
	Function: main
		'auto' keyword count: 3
		'auto' keyword count (raw source): 3
		Function-scope auto variable declarations: 3
	Function: v
		'auto' keyword count: 4
		'auto' keyword count (raw source): 1
		Function-scope auto variable declarations: 4
EOF
)

@test "Keyword token count: auto" {
  run $CCSM_EXE $TEST_SOURCE_DIR/kw_auto.c $TEST_SOURCE_DIR/empty.c --output-metrics=KW_AUTO_CNT,RAW_KW_AUTO_CNT,VAR_FN_LOC_AUTO_CNT --
  assert_success
  assert_ccsm_output "$EXPECTED"
}