cmake_minimum_required(VERSION 3.28)
set(PROJECT "sshclient")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/tc-x86_64-w64-mingw32.cmake" CACHE STRING "toolchain file")
project(${PROJECT} LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)


##############

set(CMAKE_C_FLAGS "-fpic -ffunction-sections -fdata-sections")
set(CMAKE_C_FLAGS_DEBUG "-Og -g")
set(CMAKE_C_FLAGS_RELEASE "-Os")

set(CMAKE_CXX_FLAGS "-fpic -fno-rtti -ffunction-sections -fdata-sections -fno-exceptions")
set(CMAKE_CXX_FLAGS_DEBUG "-Og -g")
set(CMAKE_CXX_FLAGS_RELEASE "-Os")

set(CMAKE_C_LINKER_FLAGS "-Wl,--gc-sections --static -fPIE")
set(CMAKE_C_LINKER_FLAGS_RELEASE "-s")

set(CMAKE_CXX_LINKER_FLAGS "-Wl,--gc-sections --static -fPIE")
set(CMAKE_CXX_LINKER_FLAGS_RELEASE "-s")

##############

set(BUILD_SHARED_LIBS OFF)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
add_executable(${PROJECT} ${SOURCES})
target_link_directories(${PROJECT} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/lib)
include_directories(${PROJECT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${PROJECT} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/include)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/wolfssl.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/wolfssh.cmake)

target_link_libraries(${PROJECT} PUBLIC wolfssh wolfssl)
