# 声明库
add_library(CoreParallelNewLib CoreParallelNew.cpp)

# 包含目录
target_include_directories(CoreParallelNewLib PUBLIC ${CMAKE_SOURCE_DIR})

if(OpenMP_CXX_FOUND)
    target_link_libraries(CoreParallelNewLib PUBLIC OpenMP::OpenMP_CXX)
endif()