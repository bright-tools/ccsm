load test_helper.bash

EXPECTED=$(cat <<EOF
Global
File: src\empty.c
File: src\paths_while.c
	Function: path_while_001_expected_2
		Number of paths through the function: 2
	Function: path_while_002_expected_1
		Number of paths through the function: 1
	Function: path_while_003_expected_2_raw_1ca
		Number of paths through the function: 1
	Function: path_while_004_expected_3
		Number of paths through the function: 3
	Function: path_while_005_expected_3raw_1ca
		Number of paths through the function: 1
	Function: path_while_006_expected_3raw_2ca
		Number of paths through the function: 2
	Function: path_while_007_expected_2
		Number of paths through the function: 2
	Function: path_while_008_expected_1
		Number of paths through the function: 1
	Function: path_while_009_expected_3raw_1ca
		Number of paths through the function: 1
EOF
)

@test "Path counting - while (with condition analysis)" {
  run $CCSM $TEST_SOURCE_DIR/paths_while.c $TEST_SOURCE_DIR/empty.c $TEST_SOURCE_DIR/empty.c --output-metrics=HIS_PATH --enable-condition-analysis --
  assert_success
  assert_ccsm_output "$EXPECTED"
}
