cmake_minimum_required(VERSION 3.18.0)
project(tulun)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/client)
include_directories(${PROJECT_SOURCE_DIR}/include/server)
include_directories(${PROJECT_SOURCE_DIR}/include/db)
include_directories(${PROJECT_SOURCE_DIR}/include/loadBalanceServer)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty)

## event
include_directories(/data/code/ml-platform-thirdparty/libevent/include)
link_directories(/data/code/ml-platform-thirdparty/libevent/lib)
## json11
include_directories(/data/code/ml-platform-thirdparty/json11/include)
link_directories(/data/code/ml-platform-thirdparty/json11/lib)
## fix
add_subdirectory(src)
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(informationCollector)
add_subdirectory(loadBalanceServer)
add_subdirectory(db)
