add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE search)
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")

add_executable(indexing main2.cpp)
target_link_libraries(indexing PRIVATE index)
target_include_directories(indexing PUBLIC "${PROJECT_SOURCE_DIR}/include")