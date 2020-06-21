Functional Tests
================

To run the tests you need to 
1. Have [BATS](https://github.com/bats-core/bats-core) installed.
1. Have compiled CCSM
1. Set the environment variable `CCSM_EXE` to point to the binary to be tested.  e.g. `export CCSM_EXE=~/build/bin/ccsm`

Once this is done, to run the test suite:

1. `cd tests`
1. `bats tests`