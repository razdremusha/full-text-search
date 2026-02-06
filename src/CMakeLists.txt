add_library(index  IndexHandler.cpp)
add_library(search Searcher.cpp ArgParser.cpp LexAnalyzer.cpp LexParser.cpp)
target_include_directories(search PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(index PUBLIC "${PROJECT_SOURCE_DIR}/include")