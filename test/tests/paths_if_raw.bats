load test_helper.bash

EXPECTED=$(cat <<EOF
Global
File: src\empty.c
File: src\paths_if.c
	Function: path_if_001_expected_2
		Number of paths through the function: 2
	Function: path_if_002_expected_4
		Number of paths through the function: 4
	Function: path_if_003_expected_3
		Number of paths through the function: 3
	Function: path_if_004_expected_4
		Number of paths through the function: 4
	Function: path_if_005_expected_5
		Number of paths through the function: 5
	Function: path_if_006_expected_6
		Number of paths through the function: 6
	Function: path_if_007_expected_8
		Number of paths through the function: 8
	Function: path_if_008_expected_4
		Number of paths through the function: 4
	Function: path_if_009_expected_4
		Number of paths through the function: 4
	Function: path_if_010_expected_2
		Number of paths through the function: 2
	Function: path_if_011_expected_3raw_1ca
		Number of paths through the function: 3
	Function: path_if_012_expected_3raw_1ca
		Number of paths through the function: 3
	Function: path_if_013_expected_2raw_1ca
		Number of paths through the function: 2
	Function: path_if_014_expected_2raw_1ca
		Number of paths through the function: 2
EOF
)

@test "Path counting - if (raw)" {
  run $CCSM $TEST_SOURCE_DIR/paths_if.c $TEST_SOURCE_DIR/empty.c $TEST_SOURCE_DIR/empty.c --output-metrics=HIS_PATH --
  assert_success
  assert_ccsm_output "$EXPECTED"
}