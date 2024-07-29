# 声明库
add_library(CoreLib FCPathLevelLeft.cpp FCTreeBuilderLeft.cpp FCTreeBuilderRight.cpp FCTreeDFS.cpp FCTree.cpp FCTreeBuilderPathParallelBylmd.cpp FCTreeBuilderPathParallelByk.cpp)

# 包含目录
target_include_directories(CoreLib PUBLIC ${CMAKE_SOURCE_DIR})

if(OpenMP_CXX_FOUND)
    target_link_libraries(CoreLib PUBLIC OpenMP::OpenMP_CXX)
endif()