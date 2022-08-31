load test_helper.bash

EXPECTED=$(cat <<EOL
Global
Comment density: 1
Comments in bytes: 80
Comment count: 4
File: src\comments.c
	Comment density: 1
	Comments in bytes: 80
	Comment count: 4
	Function: comment_fn_1
		Comment density: 1
		Comments in bytes: 80
		Comment count: 4
		Function: comment_fn_1
	File: src\comments.h
		Comment density: N/A
		Comments in bytes: 23
		Comment count: 1
EOL
)

@test "Comments" {
  run $CCSM_EXE "$TEST_SOURCE_DIR/comments.c" --output-metrics=COMMENT_HIS_COMF,COMMENT_BYTE_CNT,COMMENT_CNT --
  assert_success
  assert_ccsm_output "$EXPECTED"
}