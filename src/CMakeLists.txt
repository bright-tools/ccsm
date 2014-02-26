set(LLVM_LINK_COMPONENTS support)
set(LLVM_USED_LIBS clangTooling clangBasic clangAST)

add_clang_executable(ccsm
  ccsm.cpp
  MetricMatcher.cpp
  MetricUnit.cpp
  )
target_link_libraries(ccsm
  clangTooling
  clangBasic
  clangASTMatchers
  )

