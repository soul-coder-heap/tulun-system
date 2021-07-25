project(blacklist_service VERSION 1.0.0.0 LANGUAGES C CXX)
add_executable(blacklist_service 
    ${TL_TOP_DIR}/src/tl/server/main.cpp
)

target_link_libraries(blacklist_service PUBLIC tl::component)

target_include_directories(blacklist_service PRIVATE ${TL_TOP_DIR}/include)
target_include_directories(blacklist_service PRIVATE ${TL_TOP_DIR}/src)

if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.0)
    target_compile_options(blacklist_service PRIVATE -std=c++11)
else()
    target_compile_options(blacklist_service PRIVATE -std=c++17)
endif()

set_target_properties(blacklist_service PROPERTIES EXPORT_NAME blacklist_service)
install(TARGETS blacklist_service
        EXPORT tl_blacklist_service_targets
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)


