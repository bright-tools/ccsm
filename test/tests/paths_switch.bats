load test_helper.bash

EXPECTED=$(cat <<EOF
Global
File: src\empty.c
File: src\paths_switch.c
	Function: path_switch_001_expected_3
		Number of paths through the function: 3
	Function: path_switch_002_expected_1
		Number of paths through the function: 1
	Function: path_switch_003_expected_1
		Number of paths through the function: 1
	Function: path_switch_004_expected_2
		Number of paths through the function: 2
	Function: path_switch_005_expected_3
		Number of paths through the function: 3
EOF
)

@test "Path counting - switch" {
  run $CCSM $TEST_SOURCE_DIR/paths_switch.c $TEST_SOURCE_DIR/empty.c $TEST_SOURCE_DIR/empty.c --output-metrics=HIS_PATH --
  assert_success
  assert_ccsm_output "$EXPECTED"
}
