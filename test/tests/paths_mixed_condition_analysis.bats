load test_helper.bash

EXPECTED=$(cat <<EOF
Global
File: src\empty.c
File: src\paths_mixed.c
	Function: path_mixed_001_expected_1
		Number of paths through the function: 1
	Function: path_mixed_002_expected_6
		Number of paths through the function: 6
	Function: path_mixed_003_expected_5
		Number of paths through the function: 5
	Function: path_mixed_004_expected_5
		Number of paths through the function: 5
	Function: path_mixed_005_expected_3
		Number of paths through the function: 3
	Function: path_mixed_006_expected_8
		Number of paths through the function: 8
	Function: path_mixed_007_expected_5
		Number of paths through the function: 5
	Function: path_mixed_008_expected_7
		Number of paths through the function: 7
	Function: path_mixed_009_expected_4
		Number of paths through the function: 4
	Function: path_mixed_010_expected_7
		Number of paths through the function: 7
	Function: path_mixed_011_expected_7
		Number of paths through the function: 7
	Function: path_mixed_012_expected_10
		Number of paths through the function: 10
	Function: path_mixed_013_expected_3
		Number of paths through the function: 3
	Function: path_mixed_014_expected_5
		Number of paths through the function: 5
	Function: path_mixed_015_expected_5
		Number of paths through the function: 5
	Function: path_mixed_016_expected_9
		Number of paths through the function: 9
	Function: path_mixed_017_expected_24
		Number of paths through the function: 24
	Function: path_mixed_018_expected_6
		Number of paths through the function: 6
	Function: path_mixed_019_expected_2
		Number of paths through the function: 2
	Function: path_mixed_020_expected_2
		Number of paths through the function: 2
	Function: path_mixed_021_expected_3
		Number of paths through the function: 3
	Function: path_mixed_022_expected_2
		Number of paths through the function: 2
	Function: path_mixed_023_expected_3raw_1ca
		Number of paths through the function: 1
	Function: path_mixed_024_expected_5raw_4ca
		Number of paths through the function: 4
	Function: path_mixed_025_expected_5raw_4ca
		Number of paths through the function: 4
	Function: path_mixed_026_expected_5raw_1ca
		Number of paths through the function: 1
EOF
)

@test "Path counting - mixed (with condition analysis)" {
  run $CCSM $TEST_SOURCE_DIR/paths_mixed.c $TEST_SOURCE_DIR/empty.c $TEST_SOURCE_DIR/empty.c --output-metrics=HIS_PATH --enable-condition-analysis --
  assert_success
  assert_ccsm_output "$EXPECTED"
}