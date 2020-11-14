load test_helper.bash

EXPECTED=$(cat <<EOF
error: no input files
EOF
)

@test "Non-existent file for analysis" {
  run $CCSM_EXE i_do_not_exist.c --
  assert_failure
  assert_ccsm_output "$EXPECTED"
}