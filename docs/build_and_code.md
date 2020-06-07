Overview
========

CCSM is built as a [Clang](http://clang.llvm.org/) tool.  clang does the heavy
lifting of parsing the C/C++ into an [AST](https://en.wikipedia.org/wiki/Abstract_syntax_tree), 
then CCSM processes the AST to gather the metrics.

Building The Project
====================

Before starting, please ensure that you have a command-line version of git
installed.  If you are using Windows, you must ensure that git is available 
to Windows Command Prompt.

To build the project you will need to be able to compile Clang, including having
all dependencies installed.  See the 
[instructions](http://clang.llvm.org/get_started.html).  

Currently I use Visual Studio 2019 on Windows and GCC on
[Ubuntu Bionic Beaver](http://releases.ubuntu.com/18.04/) for builds.

Follow the instructions up to the point where you have checked out the LLVM project, 
then open the file `llvm-project\clang\tools\CMakeLists.txt` and add 
the following lines:

    set(CCSM_DIR F:/WORK/GIT/CCSM/)
    add_subdirectory(${CCSM_DIR}/src ${CMAKE_CURRENT_BINARY_DIR}/ccsm)

You will need to modify the path on the first line to point to wherever your clone
of the CCSM project is (it should point to the parent of the 'src' directory)

After that, follow the remainder of the instructions for building Clang.  CCSM
should be build as part of the complete Clang build.

Quick Start
-----------

A script within the repo may help get you to the stage where you can build the code:

```
git clone --recurse-submodules https://github.com/bright-tools/ccsm.git
./ccsm/utils/vagrant/buildit.sh ccsm
```

Code Structure
==============

File                  | Content
----------------------|--------
ccsm.cpp              | main() function to process command line options, set up clang and invoke the processing of the AST via a MetricMatcher
MetricASTConsumer.cpp | Class which received callbacks as new AST translation units are processed 
MetricUnit.cpp        | Class to support counting of the metrics, arranged th objects arranged in a hierachial structure
MetricUtils.cpp       | Support functions for doing any significant AST processing not supported by clang out-of-the-box
MetricMatcher.cpp     | AST visitor which deals with processing the AST call-backs and recording the appropriate metrics via MetricUnit objects
MetricPPConsumer.cpp  | Class to receive call-backs from the pre-processor in order to collect metrics relating to comments (not present in the AST tree)

TODO: Needs to be updated

