include(FetchContent)
set(WOLFSSH "wolfssh")
set(WOLFSSH_URL "https://github.com/wolfSSL/wolfssh/archive/refs/tags/v1.4.17-stable.tar.gz")
FetchContent_Declare(
    ${WOLFSSH}
    URL ${WOLFSSH_URL}
    CONFIGURE_COMMAND "" 
    BUILD_COMMAND ""
)
FetchContent_GetProperties(${WOLFSSH})
if(NOT ${WOLFSSH}_POPULATED)
    FetchContent_Populate(${WOLFSSH})
endif()

execute_process(
    COMMAND ./autogen.sh
    WORKING_DIRECTORY ${wolfssh_SOURCE_DIR}
)
execute_process(
    COMMAND ${wolfssh_SOURCE_DIR}/configure --prefix=${CMAKE_CURRENT_BINARY_DIR} --enable-all --enable-static --enable-shared=no --with-wolfssl=${CMAKE_CURRENT_BINARY_DIR} CC=${CMAKE_C_COMPILER} CFLAGS=${CMAKE_C_FLAGS}
    WORKING_DIRECTORY ${wolfssh_BINARY_DIR} 
)
execute_process(
    COMMAND make -j4
    WORKING_DIRECTORY ${wolfssh_BINARY_DIR} 
)
execute_process(
    COMMAND make install
    WORKING_DIRECTORY ${wolfssh_BINARY_DIR} 
)