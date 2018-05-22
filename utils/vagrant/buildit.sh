#!/bin/bash

svn co http://llvm.org/svn/llvm-project/llvm/trunk llvm
cd llvm/tools
svn co http://llvm.org/svn/llvm-project/cfe/trunk clang
echo "set(CCSM_DIR /ccsm)" >> clang/tools/CMakeLists.txt
echo "add_subdirectory(\${CCSM_DIR}/SRC \${CMAKE_CURRENT_BINARY_DIR}/ccsm)" >> clang/tools/CMakeLists.txt
cd ../..
mkdir build
cd build
cmake -G "Unix Makefiles" ../llvm
make