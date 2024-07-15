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

file (GLOB QDUMP_SRCS "src/tools/qsymboldump/*.cpp" "src/tools/qsymboldump/symbol/*.cpp")
include_directories("src/libs/qtoken")
add_executable (qsymboldump ${QDUMP_SRCS})
target_link_libraries(qsymboldump qtoken)