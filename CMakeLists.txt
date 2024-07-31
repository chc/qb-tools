cmake_minimum_required(VERSION 3.24)
project(qbtools)
enable_testing()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file (GLOB QTOKEN_SRCS "src/libs/qtoken/*.cpp" "src/libs/qtoken/*.cpp")
include_directories("src/libs/qsymbol")
add_library(qtoken ${QTOKEN_SRCS})
target_link_libraries(qtoken qsymbol)


file (GLOB QSYMBOL_SRCS "src/libs/qsymbol/*.cpp" "src/libs/qsymbol/*.cpp")
add_library(qsymbol ${QSYMBOL_SRCS})
target_link_libraries(qsymbol)

file (GLOB QDUMP_SRCS "src/tools/qdump/*.cpp")
include_directories("src/libs/qtoken")
add_executable (qdump ${QDUMP_SRCS})
target_link_libraries(qdump qtoken)

file (GLOB QDEOPT_SRCS "src/tools/qdeopt/*.cpp")
include_directories("src/libs/qtoken" "src/libs/qsymbol")
source_group("Sources" FILES ${QDEOPT_SRCS})
add_executable (qdeopt ${QDEOPT_SRCS})
target_link_libraries(qdeopt qtoken qsymbol)