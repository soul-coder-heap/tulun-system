aux_source_directory(. SRC_LIST)

add_executable(loadBalanceServer ${SRC_LIST})

target_link_libraries(loadBalanceServer pthread json11 event crypto)
