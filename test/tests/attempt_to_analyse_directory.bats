load test_helper.bash

EXPECTED=$(cat <<EOF
No compile jobs identified.  Were valid file(s) specified for analysis?
EOF
)

@test "Specify directory for analysis" {
  run $CCSM_EXE $TEST_SOURCE_DIR --
  assert_failure
  assert_ccsm_output "$EXPECTED"
}