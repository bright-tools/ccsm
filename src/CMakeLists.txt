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

# Make ccsm_ver.h depend on a fake dependency so that it's always re-built
set_property(SOURCE ccsm_ver.h APPEND PROPERTY OBJECT_DEPENDS ccsm_ver.h_fake)

add_custom_command( OUTPUT "${CCSM_DIR}/src/ccsm_ver.h_fake"
    COMMAND ${CCSM_DIR}/utils/gen_ver/gen_ver.py "${CCSM_DIR}" > "${CCSM_DIR}/src/ccsm_ver.h"
    WORKING_DIRECTORY ${CCSM_DIR}
)
