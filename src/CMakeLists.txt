cmake_minimum_required(VERSION 3.24)
project(search_engine VERSION 0.1)

configure_file(${MY_INCLUDE_DIR}/config.h.in ${MY_INCLUDE_DIR}/config.h @ONLY)
include_directories (search_engine PUBLIC $ {CMAKE_CURRENT_BINARY_DIR})

add_executable(search_engine main.cpp SearchServer.cpp ${MY_INCLUDE_DIR}/SearchServer.h InvertedIndex.cpp ${MY_INCLUDE_DIR}/InvertedIndex.h ConverterJSON.cpp ${MY_INCLUDE_DIR}/ConverterJSON.h)

target_include_directories(search_engine PUBLIC ${MY_INCLUDE_DIR})
target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)