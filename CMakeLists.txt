cmake_minimum_required(VERSION 3.24)
project(qbtools)
enable_testing()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file (GLOB QTOKEN_SRCS "src/libs/qtoken/*.cpp" "src/libs/qtoken/*.cpp")
add_library(qtoken ${QTOKEN_SRCS})
target_link_libraries(qtoken)

file (GLOB QDUMP_SRCS "src/tools/qdump/*.cpp")
include_directories("src/libs/qtoken")
add_executable (qdump ${QDUMP_SRCS})
target_link_libraries(qdump qtoken)

file (GLOB QSYMDUMP_SRCS "src/tools/qsymboldump/*.cpp")
FILE (GLOB QSYMDUMP_SYMBOLS "src/tools/qsymboldump/symbol/*.cpp")
include_directories("src/libs/qtoken")
source_group("Sources" FILES ${QSYMDUMP_SRCS})
source_group("Symbols" FILES ${QSYMDUMP_SYMBOLS})
add_executable (qsymboldump ${QSYMDUMP_SRCS} ${QSYMDUMP_SYMBOLS})
target_link_libraries(qsymboldump qtoken)

file (GLOB QSYMPARSE_SRCS "src/tools/qsymparse/*.cpp")
include_directories("src/libs/qsymparse")
add_executable (qsymparse ${QSYMPARSE_SRCS})
target_link_libraries(qsymparse)