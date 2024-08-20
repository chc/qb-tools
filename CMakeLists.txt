cmake_minimum_required(VERSION 3.24)
project(qbtools)
enable_testing()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#THPG
# add_definitions(-DISTREAM_SYMBOL_ENDIAN=ISTREAM_BIG_ENDIAN)
# add_definitions(-DISTREAM_TOKEN_ENDIAN=ISTREAM_LITTLE_ENDIAN)
# add_definitions(-DSYMBOL_STRUCT_FLAG=0x80)
# add_definitions(-DSYMBOL_ROOT_FLAG=0x20)
# add_definitions(-DSYMBOL_ISREF_FLAG=0x10)
# add_definitions(-DARRAY_HEADER_VALUE=1)
# add_definitions(-DSYMBOL_STRUCT_TYPE_ANDMASK=0x0F)
# add_definitions(-DSYMBOL_STRUCT_TYPE_RSHIFTMASK=0)
# add_definitions(-DWIDE_LOCALSTRING=1)

add_definitions(-DISTREAM_SYMBOL_ENDIAN=ISTREAM_LITTLE_ENDIAN)
add_definitions(-DISTREAM_TOKEN_ENDIAN=ISTREAM_LITTLE_ENDIAN)
add_definitions(-DSYMBOL_STRUCT_FLAG=0x01)
add_definitions(-DSYMBOL_ROOT_FLAG=0x04)
add_definitions(-DSYMBOL_ISREF_FLAG=0x80)
add_definitions(-DSYMBOL_STRUCT_TYPE_ANDMASK=0x1E)
add_definitions(-DSYMBOL_STRUCT_TYPE_RSHIFTMASK=1)



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
target_link_libraries(qdeopt qtoken qsymbol dbginfo pak)

file (GLOB QOPT_SRCS "src/tools/qopt/*.cpp")
include_directories("src/libs/qtoken" "src/libs/qsymbol")
source_group("Sources" FILES ${QOPT_SRCS})
add_executable (qopt ${QOPT_SRCS})
target_link_libraries(qopt qtoken qsymbol dbginfo pak)

file (GLOB LIBPAK_SRCS "src/libs/pak/*.cpp")
include_directories("src/libs/pak")
add_library(pak ${LIBPAK_SRCS})

file (GLOB LIBDBGINFO_SRCS "src/libs/dbginfo/*.cpp")
include_directories("src/libs/dbginfo")
add_library(dbginfo ${LIBDBGINFO_SRCS})

file (GLOB UNPAK_SRCS "src/tools/unpak/*.cpp")
include_directories("src/libs/qtoken" "src/libs/qsymbol" "src/libs/pak")
source_group("Sources" FILES ${UNPAK_SRCS})
add_executable (unpak ${UNPAK_SRCS})
target_link_libraries(unpak qtoken qsymbol pak dbginfo)


file (GLOB PAK_SRCS "src/tools/pak/*.cpp")
include_directories("src/libs/qtoken" "src/libs/qsymbol" "src/libs/pak")
source_group("Sources" FILES ${PAK_SRCS})
add_executable (pakdir ${PAK_SRCS})
target_link_libraries(pakdir qtoken qsymbol pak dbginfo)

file (GLOB QRESOLVE_SRCS "src/tools/qresolve/*.cpp")
include_directories("src/libs/qtoken" "src/libs/qsymbol" "src/libs/pak" "src/libs/dbginfo")
source_group("Sources" FILES ${QRESOLVE_SRCS})
add_executable (qresolve ${QRESOLVE_SRCS})
target_link_libraries(qresolve qtoken qsymbol pak dbginfo)


file (GLOB QGENNAME_SRCS "src/tools/qgenname/*.cpp")
include_directories("src/libs/qtoken")
source_group("Sources" FILES ${QGENNAME_SRCS})
add_executable (qgenname ${QGENNAME_SRCS})
target_link_libraries(qgenname qtoken)

file (GLOB QOFFDUMP_SYM_SRCS "src/tools/qoffdump/qoffdump_symbol.cpp" "src/tools/qoffdump/shared.cpp")
include_directories("src/libs/qtoken")
add_executable (qoffdump_symbol ${QOFFDUMP_SYM_SRCS})
target_link_libraries(qoffdump_symbol qtoken)

file (GLOB QOFFDUMP_TOKEN_SRCS "src/tools/qoffdump/qoffdump_token.cpp" "src/tools/qoffdump/shared.cpp")
include_directories("src/libs/qtoken" "src/libs/qsymbol")
add_executable (qoffdump_token ${QOFFDUMP_TOKEN_SRCS})
target_link_libraries(qoffdump_token qtoken qsymbol)
