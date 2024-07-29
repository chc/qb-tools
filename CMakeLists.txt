cmake_minimum_required(VERSION 3.24)
project(qbtools)
enable_testing()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file (GLOB QTOKEN_SRCS "src/libs/qtoken/*.cpp" "src/libs/qtoken/*.cpp")
add_library(qtoken ${QTOKEN_SRCS})
target_link_libraries(qtoken)


file (GLOB QSYMBOL_SRCS "src/libs/qsymbol/*.cpp" "src/libs/qsymbol/*.cpp")
add_library(qsymbol ${QSYMBOL_SRCS})
target_link_libraries(qsymbol)

file (GLOB QDUMP_SRCS "src/tools/qdump/*.cpp")
include_directories("src/libs/qtoken")
add_executable (qdump ${QDUMP_SRCS})
target_link_libraries(qdump qtoken)

file (GLOB QSYMDUMP_SRCS "src/tools/qsymboldump/*.cpp")
include_directories("src/libs/qtoken" "src/libs/qsymbol")
source_group("Sources" FILES ${QSYMDUMP_SRCS})
add_executable (qsymboldump ${QSYMDUMP_SRCS})
target_link_libraries(qsymboldump qtoken qsymbol)

file (GLOB QSYMDUMP_SRCS "src/tools/qdeopt/*.cpp")
include_directories("src/libs/qtoken" "src/libs/qsymbol")
source_group("Sources" FILES ${QSYMDUMP_SRCS})
add_executable (qdeopt ${QSYMDUMP_SRCS})
target_link_libraries(qdeopt qtoken qsymbol)