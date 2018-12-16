#!/usr/bin/env bash

set -e

if [ -z "$1" ]; then
    echo "Please supply 1 parameter, the path to CCSM checkout (dir which contains 'src')"
    exit 1
fi

if [[ ! -d "$1" ]]; then
    echo "'$1' doesn't seem to be a directory"
    exit 1
fi

if [[ ! -d "$1/src" ]]; then
    echo "I expected to see CCSM's 'src' directory in $1, but didn't"
    exit 1
fi

function basicToolCheck {
    TOOLS="cmake svn grep realpath nproc"
    for TOOL in $TOOLS; do
        echo "Checking for $TOOL"
        if [[ ! -x `which $TOOL` ]]; then
            echo "Didn't find $TOOL"
            exit 1
        fi
    done
}

basicToolCheck

LLVM_DIR=llvm

if [ -d $LLVM_DIR ]; then
    svn up $LLVM_DIR
else
    svn co http://llvm.org/svn/llvm-project/llvm/trunk $LLVM_DIR
fi

CLANG_DIR=$LLVM_DIR/tools/clang

if [ -d $CLANG_DIR ]; then
    svn up $CLANG_DIR
else
    svn co http://llvm.org/svn/llvm-project/cfe/trunk $CLANG_DIR
fi

CMAKE_FILE=$CLANG_DIR/tools/CMakeLists.txt

if [ $(grep -c CCSM_DIR $CMAKE_FILE) -lt 1 ]; then
    CCSM_PATH=$(realpath $1)
    echo "Adding CCSM to clang tools makefile"
    echo "set(CCSM_DIR $CCSM_PATH)" >> $CMAKE_FILE
    echo "add_subdirectory(\${CCSM_DIR}/SRC \${CMAKE_CURRENT_BINARY_DIR}/ccsm)" >> $CMAKE_FILE
else
    echo "CCSM already seems to be in the clang tools makefile"
fi

(mkdir -p build; cd build; cmake -G "Unix Makefiles" ../llvm)
make -C build/tools/clang/tools/ccsm -j $(nproc --all)

exit 0
