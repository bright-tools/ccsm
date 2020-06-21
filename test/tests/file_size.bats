load test_helper.bash

EXPECTED=$(cat <<EOF
Global
Size in bytes: 124
Size in lines: 8
File: src\file_size.c
	Size in bytes: 43
	Size in lines: 3
File: src\file_size.h
	Size in bytes: 58
	Size in lines: 4
File: src\file_size2.c
	Size in bytes: 23
	Size in lines: 1
EOF
)

@test "File size and line count metrics" {
  run $CCSM_EXE $TEST_SOURCE_DIR/file_size.c $TEST_SOURCE_DIR/file_size2.c --output-metrics=FILE_SIZE,FILE_LINE_CNT --
  assert_success
  assert_ccsm_output "$EXPECTED"
}