load test_helper.bash

EXPECTED=$(cat <<EOF
Global
File-scope total local variable declarations: 5
File-scope static variable declarations: 0
File-scope external variable declarations: 1
Function-scope total local variable declarations: 1
Function-scope static variable declarations: 0
Function-scope register variable declarations: 0
Function-scope auto variable declarations: 0
Function-scope external variable declarations: 1
File: src\second_include.c
	File-scope total local variable declarations: 0
	File-scope static variable declarations: 0
	File-scope external variable declarations: 0
File: src\simple_include.c
	File-scope total local variable declarations: 3
	File-scope static variable declarations: 0
	File-scope external variable declarations: 0
	Function: main
		Function-scope total local variable declarations: 0
		Function-scope static variable declarations: 0
		Function-scope register variable declarations: 0
		Function-scope auto variable declarations: 0
		Function-scope external variable declarations: 0
File: src\simple_include.h
	File-scope total local variable declarations: 2
	File-scope static variable declarations: 0
	File-scope external variable declarations: 1
	Function: safe
		Function-scope total local variable declarations: 1
		Function-scope static variable declarations: 0
		Function-scope register variable declarations: 0
		Function-scope auto variable declarations: 0
		Function-scope external variable declarations: 1
	Function: simple
		Function-scope total local variable declarations: 0
		Function-scope static variable declarations: 0
		Function-scope register variable declarations: 0
		Function-scope auto variable declarations: 0
		Function-scope external variable declarations: 0
EOF
)

@test "Same header file included in 2 source files" {
  run $CCSM $TEST_SOURCE_DIR/simple_include.c  $TEST_SOURCE_DIR/second_include.c --output-metrics=VAR_FILE_LOC_CNT,VAR_FILE_LOC_STATIC_CNT,VAR_FILE_EXT_CNT,VAR_FN_LOC_CNT,VAR_FN_LOC_STATIC_CNT,VAR_FN_LOC_REG_CNT,VAR_FN_LOC_AUTO_CNT,VAR_FN_EXT_CNT --
  assert_success
  assert_ccsm_output "$EXPECTED"
}