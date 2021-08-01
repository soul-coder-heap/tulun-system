project(tl_component VERSION 1.0.0.0 LANGUAGES C CXX)
add_library(tl_component STATIC
    ${TL_TOP_DIR}/src/tl/component/logger.cpp
    ${TL_TOP_DIR}/src/tl/component/threadpool.cpp
)

find_package(spdlog REQUIRED CONFIG)
target_link_libraries(tl_component PRIVATE spdlog::spdlog)
find_package(unofficial-concurrentqueue CONFIG REQUIRED)
target_link_libraries(tl_component PRIVATE unofficial::concurrentqueue::concurrentqueue -lpthread)
find_package(Boost 1.75.0 REQUIRED)
target_link_libraries(tl_component PRIVATE ${Boost_LIBRARIES})



target_include_directories(tl_component PRIVATE ${TL_TOP_DIR}/include)
target_include_directories(tl_component PUBLIC ${Boost_INCLUDE_DIRS})
target_include_directories(tl_component PRIVATE ${TL_TOP_DIR}/src)
target_include_directories(tl_component INTERFACE $<BUILD_INTERFACE:${TL_TOP_DIR}/include>)
target_include_directories(tl_component INTERFACE $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>)

if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.0)
    target_compile_options(tl_component PRIVATE -std=c++11)
else()
    target_compile_options(tl_component PRIVATE -std=c++17)
endif()
add_library(tl::component ALIAS tl_component)

#static library don't need install
#set_target_properties(tl_component PROPERTIES EXPORT_NAME component)
#install(TARGETS tl_component
#        EXPORT tl_component_targets
#        RUNTIME DESTINATION bin
#        LIBRARY DESTINATION lib
#        ARCHIVE DESTINATION lib)
#export install
#include(${CMAKE_CURRENT_LIST_DIR}/tl_component_export.cmake)



