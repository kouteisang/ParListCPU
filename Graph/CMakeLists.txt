# 声明库
add_library(GraphLib Graph.cpp MultilayerGraph.cpp)

# 包含目录
target_include_directories(GraphLib PUBLIC ${CMAKE_SOURCE_DIR})
