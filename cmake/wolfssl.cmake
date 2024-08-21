include(FetchContent)
set(WOLFSSL "wolfssl")
set(WOLFSSL_URL "https://sources.buildroot.net/wolfssl/wolfssl-5.7.2.tar.gz")
FetchContent_Declare(
    ${WOLFSSL}
    URL ${WOLFSSL_URL}
    CONFIGURE_COMMAND "" 
    BUILD_COMMAND ""
)
FetchContent_GetProperties(${WOLFSSL})
if(NOT ${WOLFSSL}_POPULATED)
    FetchContent_Populate(${WOLFSSL})
endif()

execute_process(
    COMMAND ./autogen.sh
    WORKING_DIRECTORY ${wolfssl_SOURCE_DIR}
)
execute_process(
    COMMAND ${wolfssl_SOURCE_DIR}/configure --prefix=${CMAKE_CURRENT_BINARY_DIR} --enable-keygen --enable-wolfssh --enable-static CC=${CMAKE_C_COMPILER} CFLAGS=${CMAKE_C_FLAGS}
    WORKING_DIRECTORY ${wolfssl_BINARY_DIR} 
)
execute_process(
    COMMAND make -j4
    WORKING_DIRECTORY ${wolfssl_BINARY_DIR} 
)
execute_process(
    COMMAND make install
    WORKING_DIRECTORY ${wolfssl_BINARY_DIR} 
)