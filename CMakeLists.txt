project(qbtools)
enable_testing()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)


file (GLOB QDUMP_SRCS "src/tools/qdump/*.cpp")
add_executable (qdump ${QDUMP_SRCS})
target_link_libraries(qdump)