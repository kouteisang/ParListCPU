# 声明库
add_library(CoreParallelLib FCTreeBuilderCoreParallel.cpp FCCoreTree.cpp)

# 包含目录
target_include_directories(CoreParallelLib PUBLIC ${CMAKE_SOURCE_DIR})
