load test_helper.bash

EXPECTED=$(cat <<EOF
Global
File: src\empty.c
File: src\paths_for.c
	Function: path_for_001_expected_2
		Number of paths through the function: 2
	Function: path_for_003_expected_3
		Number of paths through the function: 3
EOF
)

@test "Path counting - for" {
  run $CCSM $TEST_SOURCE_DIR/paths_for.c $TEST_SOURCE_DIR/empty.c $TEST_SOURCE_DIR/empty.c --output-metrics=HIS_PATH --
  assert_success
  assert_ccsm_output "$EXPECTED"
}