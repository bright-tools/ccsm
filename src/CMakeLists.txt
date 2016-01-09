set(LLVM_LINK_COMPONENTS support)
set(LLVM_USED_LIBS clangTooling clangBasic clangAST)

set(SOURCES 
  ccsm.cpp
  ccsm_ver.h
  CcsmOptionsHandler.hpp
  CcsmOptionsHandler.cpp
  MetricMatcher.hpp
  MetricMatcher.cpp
  MetricUnit.hpp
  MetricUnit.cpp
  MetricUtils.hpp
  MetricUtils.cpp
  MetricDumper.hpp
  MetricDumper.cpp
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
  MetricSrcExpandedLexer.cpp
  MetricSrcExpandedLexer.hpp
  MetricSrcUnexpandedLexer.cpp
  MetricSrcUnexpandedLexer.hpp
  MetricFrontendActors.hpp
  MetricFrontendActors.cpp
  FunctionLocator.hpp
  FunctionLocator.cpp
  MetricLinkageResolver.hpp
  MetricLinkageResolver.cpp
)

add_clang_executable(ccsm ${SOURCES})

target_link_libraries(ccsm
  clangTooling
  clangBasic
  clangASTMatchers
  )

add_custom_command( TARGET ccsm
    PRE_BUILD
    COMMAND ${CCSM_DIR}\\externals\\git-vs-versioninfo-gen\\GIT-VS-VERSION-GEN.bat "${CCSM_DIR}" "${CCSM_DIR}\\src\\ccsm_ver.h" 
    WORKING_DIRECTORY ${CCSM_DIR}
)
