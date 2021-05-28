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
    TOOLS="cmake git grep realpath nproc"
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
    git -C $LLVM_DIR pull
else
    git clone https://github.com/llvm/llvm-project.git $LLVM_DIR --branch release/12.x
fi

CLANG_DIR=$LLVM_DIR/clang
CMAKE_FILE=$CLANG_DIR/tools/CMakeLists.txt

if [ $(grep -c CCSM_DIR $CMAKE_FILE) -lt 1 ]; then
    CCSM_PATH=$(realpath $1)
    echo "Adding CCSM to clang tools makefile"
    echo "set(CCSM_DIR $CCSM_PATH)" >> $CMAKE_FILE
    echo "add_subdirectory(\${CCSM_DIR}/src \${CMAKE_CURRENT_BINARY_DIR}/ccsm)" >> $CMAKE_FILE
else
    echo "CCSM already seems to be in the clang tools makefile"
fi

(mkdir -p build; cd build; cmake -G "Unix Makefiles" ../llvm/llvm -DLLVM_ENABLE_PROJECTS='clang;clang-tools-extra')
make -C build/tools/clang/tools/ccsm -j $(nproc --all)

exit 0
