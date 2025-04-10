# 声明库
add_library(CoreParallelLib FCTreeBuilderCoreParallelByK.cpp FCTreeBuilderCoreParallel.cpp FCCoreTree.cpp FCSyncLeft.cpp)

# 包含目录
target_include_directories(CoreParallelLib PUBLIC ${CMAKE_SOURCE_DIR})

if(OpenMP_CXX_FOUND)
    target_link_libraries(CoreParallelLib PUBLIC OpenMP::OpenMP_CXX)
endif()