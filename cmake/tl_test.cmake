
macro(add_test_target TEST_TARGET)
    add_executable("${target}" "${PROJECT_SOURCE_DIR}/ut/${TEST_TARGET}_test.cpp")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.0)
        target_compile_options(${target} PRIVATE -std=c++11)
    else()
        target_compile_options(${target} PRIVATE -std=c++17)
    endif()
    #target_link_libraries(${target} PRIVATE ml::thirdparty)
endmacro(add_test_target)

function(add_test_library TEST_TARGET)  
    set(target "${TEST_TARGET}")
    add_executable("${target}" "${PROJECT_SOURCE_DIR}/ut/${TEST_TARGET}_test.cpp")
    target_compile_options(${target} PRIVATE -std=c++17)
    message("-- UT ARGC = ${ARGC} , ARGV = ${ARGV} , TEST_TARGET = ${TEST_TARGET}")
    set(INDEX 1)  
    while(INDEX LESS ${ARGC})  
        set(ag ${ARGV${INDEX}})
        if("${ag}" MATCHES "^-")
            target_compile_options(${target} PRIVATE  ${ag})
            message("-- UT COMPILE OPTIONS = ${ag}")  
        else()
            target_link_libraries(${target} PRIVATE ${ag})
            message("-- UT LINK LIBRARY = ${ag}")  
        endif()
        math(EXPR INDEX "${INDEX} + 1")  
    endwhile() 
endfunction(add_test_library)

add_test_library(spdlog tl_component)