# 声明库
add_library(CoreLib FCTreeBuilder.cpp FCTreeDFS.cpp)

# 包含目录
target_include_directories(CoreLib PUBLIC ${CMAKE_SOURCE_DIR})
