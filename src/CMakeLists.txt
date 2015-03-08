set(LLVM_LINK_COMPONENTS support)
set(LLVM_USED_LIBS clangTooling clangBasic clangAST)

set(SOURCES 
  ccsm.cpp
  MetricMatcher.hpp
  MetricMatcher.cpp
  MetricUnit.hpp
  MetricUnit.cpp
  MetricUtils.hpp
  MetricUtils.cpp
  MetricOptions.hpp
  MetricOptions.cpp
  MetricPPIncludeHandler.hpp
  MetricPPIncludeHandler.cpp
  MetricPPCustomer.hpp
  MetricPPCustomer.cpp
  MetricASTConsumer.hpp
  MetricASTConsumer.cpp
  MetricSrcLexer.cpp
  MetricSrcLexer.hpp
  MetricFrontendActors.hpp
  MetricFrontendActors.cpp
  FunctionLocator.hpp
  FunctionLocator.cpp
)

add_clang_executable(ccsm ${SOURCES})

target_link_libraries(ccsm
  clangTooling
  clangBasic
  clangASTMatchers
  )

