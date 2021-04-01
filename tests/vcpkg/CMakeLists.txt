cmake_minimum_required(VERSION 3.19)
 
# 设置镜像VCPKG_ROOT所在的位置
set(VCPKG_ROOT "/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE PATH "")
set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT})
 
project(TEST_NOW)
 
find_package(GTest CONFIG REQUIRED)
find_package(benchmark CONFIG REQUIRED)

add_executable(string_test string_test.cpp)
target_link_libraries(string_test PRIVATE 
		GTest::gtest
		GTest::gtest_main 
		GTest::gmock 
		GTest::gmock_main
		benchmark::benchmark 
		benchmark::benchmark_main)
