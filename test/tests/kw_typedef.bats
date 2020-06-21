load test_helper.bash

EXPECTED=$(cat <<EOF
Global
'typedef' keyword count: 13
'typedef' keyword count (raw source): 8
File: src\empty.c
	'typedef' keyword count(local): 0
	'typedef' keyword count(cumulative): 0
	'typedef' keyword count (raw source)(local): 0
	'typedef' keyword count (raw source)(cumulative): 0
File: src\kw_typedef.c
	'typedef' keyword count(local): 6
	'typedef' keyword count(cumulative): 10
	'typedef' keyword count (raw source)(local): 2
	'typedef' keyword count (raw source)(cumulative): 5
	Function: example
		'typedef' keyword count: 2
		'typedef' keyword count (raw source): 2
	Function: sample
		'typedef' keyword count: 2
		'typedef' keyword count (raw source): 1
File: src\kw_typedef.h
	'typedef' keyword count(local): 3
	'typedef' keyword count(cumulative): 3
	'typedef' keyword count (raw source)(local): 3
	'typedef' keyword count (raw source)(cumulative): 3
EOF
)

@test "Keyword token count: typedef" {
  run $CCSM_EXE $TEST_SOURCE_DIR/kw_typedef.c $TEST_SOURCE_DIR/empty.c --output-metrics=KW_TYPEDEF_CNT,RAW_KW_TYPEDEF_CNT --
  assert_success
  assert_ccsm_output "$EXPECTED"
}