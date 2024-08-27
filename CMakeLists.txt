cmake_minimum_required(VERSION 3.24)
project(qbtools)
enable_testing()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#THPG
# add_definitions(-DISTREAM_PAK_ENDIAN=ISTREAM_BIG_ENDIAN)
# add_definitions(-DISTREAM_SYMBOL_ENDIAN=ISTREAM_BIG_ENDIAN)
# add_definitions(-DISTREAM_TOKEN_ENDIAN=ISTREAM_LITTLE_ENDIAN)
# add_definitions(-DSYMBOL_STRUCT_FLAG=0x80)
# add_definitions(-DSYMBOL_ROOT_FLAG=0x20)
# add_definitions(-DSYMBOL_ISREF_FLAG=0x10)
# add_definitions(-DARRAY_HEADER_VALUE=1)
# add_definitions(-DSYMBOL_STRUCT_TYPE_ANDMASK=0x0F)
# add_definitions(-DSYMBOL_STRUCT_TYPE_RSHIFTMASK=0)
# add_definitions(-DWIDE_LOCALSTRING=1)
# add_definitions(-DPAK_FLAGS_HAS_FILENAME=0x20)

#THAW PC
# add_definitions(-DISTREAM_PAK_ENDIAN=ISTREAM_LITTLE_ENDIAN)
# add_definitions(-DISTREAM_SYMBOL_ENDIAN=ISTREAM_LITTLE_ENDIAN)
# add_definitions(-DISTREAM_TOKEN_ENDIAN=ISTREAM_LITTLE_ENDIAN)
# add_definitions(-DSYMBOL_STRUCT_FLAG=0x01)
# add_definitions(-DSYMBOL_ROOT_FLAG=0x04)
# add_definitions(-DSYMBOL_ISREF_FLAG=0x80)
# add_definitions(-DSYMBOL_STRUCT_TYPE_ANDMASK=0x1E)
# add_definitions(-DSYMBOL_STRUCT_TYPE_RSHIFTMASK=1)
# add_definitions(-DPAK_FLAGS_HAS_FILENAME=0x20)
# add_definitions(-DPAK_INCLUDE_FILENAME)

#GH5
add_definitions(-DISTREAM_PAK_ENDIAN=ISTREAM_BIG_ENDIAN)
add_definitions(-DISTREAM_SYMBOL_ENDIAN=ISTREAM_BIG_ENDIAN)
add_definitions(-DISTREAM_TOKEN_ENDIAN=ISTREAM_LITTLE_ENDIAN)
add_definitions(-DSYMBOL_STRUCT_FLAG=0x01)
add_definitions(-DSYMBOL_ROOT_FLAG=0x20)
add_definitions(-DSYMBOL_ISREF_FLAG=0x10)
add_definitions(-DARRAY_HEADER_VALUE=1)
add_definitions(-DSYMBOL_STRUCT_TYPE_ANDMASK=0x0F)
add_definitions(-DSYMBOL_STRUCT_TYPE_RSHIFTMASK=0)
add_definitions(-DWIDE_LOCALSTRING=1)
add_definitions(-DPAB_ABSOLUTE_OFFSET)
add_definitions(-DSYMBOL_STRUCT_SEPERATE_FLAGS)
add_definitions(-DSYMBOL_STRUCT_REQUIRED_PARAM_FLAG=0x20)
add_definitions(-DWOR_SYMBOLDUMP_HEADER)
add_definitions(-DPAK_FLAGS_HAS_FILENAME=0x20)


#GH:WoR PS3
# add_definitions(-DISTREAM_PAK_ENDIAN=ISTREAM_BIG_ENDIAN)
# add_definitions(-DISTREAM_SYMBOL_ENDIAN=ISTREAM_BIG_ENDIAN)
# add_definitions(-DISTREAM_TOKEN_ENDIAN=ISTREAM_LITTLE_ENDIAN)
# add_definitions(-DSYMBOL_STRUCT_FLAG=0x01)
# add_definitions(-DSYMBOL_ROOT_FLAG=0x20)
# add_definitions(-DSYMBOL_ISREF_FLAG=0x10)
# add_definitions(-DARRAY_HEADER_VALUE=1)
# add_definitions(-DSYMBOL_STRUCT_TYPE_ANDMASK=0x0F)
# add_definitions(-DSYMBOL_STRUCT_TYPE_RSHIFTMASK=0)
# add_definitions(-DWIDE_LOCALSTRING=1)
# add_definitions(-DPAB_ABSOLUTE_OFFSET)
# add_definitions(-DSYMBOL_STRUCT_SEPERATE_FLAGS)
# add_definitions(-DSYMBOL_STRUCT_REQUIRED_PARAM_FLAG=0x20)
# add_definitions(-DWOR_SYMBOLDUMP_HEADER)
# add_definitions(-DPAK_FLAGS_HAS_FILENAME=0x20)

include_directories("src/include" "src/libs/qtoken" "src/libs/qsymbol" "src/libs/pak" "src/libs/dbginfo")


file (GLOB QTOKEN_SRCS "src/libs/qtoken/*.cpp" "src/libs/qtoken/*.cpp")
add_library(qtoken ${QTOKEN_SRCS})
target_link_libraries(qtoken qsymbol)


file (GLOB QSYMBOL_SRCS "src/libs/qsymbol/*.cpp" "src/libs/qsymbol/*.cpp")
add_library(qsymbol ${QSYMBOL_SRCS})
target_link_libraries(qsymbol)

file (GLOB QDUMP_SRCS "src/tools/qdump/*.cpp")
add_executable (qdump ${QDUMP_SRCS})
target_link_libraries(qdump qtoken)

file (GLOB QDEOPT_SRCS "src/tools/qdeopt/*.cpp")
source_group("Sources" FILES ${QDEOPT_SRCS})
add_executable (qdeopt ${QDEOPT_SRCS})
target_link_libraries(qdeopt qtoken qsymbol dbginfo pak)

file (GLOB QOPT_SRCS "src/tools/qopt/*.cpp")
source_group("Sources" FILES ${QOPT_SRCS})
add_executable (qopt ${QOPT_SRCS})
target_link_libraries(qopt qtoken qsymbol dbginfo pak)

file (GLOB LIBPAK_SRCS "src/libs/pak/*.cpp")
include_directories("src/libs/pak")
add_library(pak ${LIBPAK_SRCS})
target_link_libraries(pak qtoken)

file (GLOB LIBDBGINFO_SRCS "src/libs/dbginfo/*.cpp")
add_library(dbginfo ${LIBDBGINFO_SRCS})
target_link_libraries(dbginfo pak)

file (GLOB UNPAK_SRCS "src/tools/unpak/*.cpp")
source_group("Sources" FILES ${UNPAK_SRCS})
add_executable (unpak ${UNPAK_SRCS})
target_link_libraries(unpak pak qtoken qsymbol dbginfo)


file (GLOB PAK_SRCS "src/tools/pak/*.cpp")
source_group("Sources" FILES ${PAK_SRCS})
add_executable (pakdir ${PAK_SRCS})
target_link_libraries(pakdir pak qtoken qsymbol dbginfo)

file (GLOB QRESOLVE_SRCS "src/tools/qresolve/*.cpp")
source_group("Sources" FILES ${QRESOLVE_SRCS})
add_executable (qresolve ${QRESOLVE_SRCS})
target_link_libraries(qresolve qtoken qsymbol pak dbginfo)


file (GLOB QGENNAME_SRCS "src/tools/qgenname/*.cpp")
source_group("Sources" FILES ${QGENNAME_SRCS})
add_executable (qgenname ${QGENNAME_SRCS})
target_link_libraries(qgenname qtoken)

file (GLOB QOFFDUMP_SYM_SRCS "src/tools/qoffdump/qoffdump_symbol.cpp" "src/tools/qoffdump/shared.cpp")
add_executable (qoffdump_symbol ${QOFFDUMP_SYM_SRCS})
target_link_libraries(qoffdump_symbol qtoken)

file (GLOB QOFFDUMP_TOKEN_SRCS "src/tools/qoffdump/qoffdump_token.cpp" "src/tools/qoffdump/shared.cpp")
add_executable (qoffdump_token ${QOFFDUMP_TOKEN_SRCS})
target_link_libraries(qoffdump_token qtoken qsymbol)


file (GLOB UNCHUNK_SRCS "src/tools/unchunk/*.cpp")
add_executable (unchunk ${UNCHUNK_SRCS})
target_link_libraries(unchunk qtoken z)

file (GLOB CHUNK_SRCS "src/tools/chunk/*.cpp")
add_executable (chunk ${CHUNK_SRCS})
target_link_libraries(chunk qtoken z)

file (GLOB FASTDUMPAPPEND_SRCS "src/tools/fastdump_append/*.cpp")
source_group("Sources" FILES ${FASTDUMPAPPEND_SRCS})
add_executable (fastdump_append ${FASTDUMPAPPEND_SRCS})
target_link_libraries(fastdump_append dbginfo)