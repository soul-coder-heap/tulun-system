cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
set(VCPKG_ROOT "/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE PATH 
    "Choose vcpkg path"
    FORCE)

set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT} CACHE STRING
    "Choose compile toolchain"
    FORCE)

set( CMAKE_CXX_COMPILER "/opt/rh/devtoolset-7/root/usr/bin/g++" CACHE PATH
    "Choose g++ version"
    FORCE)

set( CMAKE_C_COMPILER "/opt/rh/devtoolset-7/root/usr/bin/gcc" CACHE PATH
    "Choose gcc version"
    FORCE)

set( CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo Asan Pprof."
    FORCE )
message("* Current build type is : ${CMAKE_BUILD_TYPE}")

set( CMAKE_CXX_FLAGS_ASAN "-O2 -g -fsanitize=address -fno-omit-frame-pointer" CACHE STRING
    "Flags used by the C++ compiler during asan builds."
    FORCE )
set( CMAKE_C_FLAGS_ASAN "-O2 -g -fsanitize=address -fno-omit-frame-pointer" CACHE STRING
    "Flags used by the C compiler during asan builds."
    FORCE )
set( CMAKE_EXE_LINKER_FLAGS_ASAN
    "-static-libasan" CACHE STRING
    "Flags used for linking binaries during asan builds."
    FORCE )
set( CMAKE_SHARED_LINKER_FLAGS_ASAN
    "-static-libasan" CACHE STRING
    "Flags used for linking shared libraries during asan builds."
    FORCE )
project(TULUN_BLACKLIST LANGUAGES C CXX)
mark_as_advanced(
    CMAKE_CXX_FLAGS_ASAN
    CMAKE_C_FLAGS_ASAN
    CMAKE_EXE_LINKER_FLAGS_ASAN
    CMAKE_SHARED_LINKER_FLAGS_ASAN)

#make install
#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/built/bin)

set(TL_TOP_DIR ${PROJECT_SOURCE_DIR})
#add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/cmake/tl/component)
#add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/cmake/tl/server)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/tl/component/generate.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/tl/server/generate.cmake)
# ut
option (TL_COMPONENT_ENABLE_TESTS "If OFF, used to disable testing altogether." ON)
if(${TL_COMPONENT_ENABLE_TESTS})
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/tl_test.cmake)
endif()
