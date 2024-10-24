# 声明库
add_library(CoreIndexParallelLib CoreIndex.cpp)

# 包含目录
target_include_directories(CoreIndexParallelLib PUBLIC ${CMAKE_SOURCE_DIR})

if(OpenMP_CXX_FOUND)
    target_link_libraries(CoreIndexParallelLib PUBLIC OpenMP::OpenMP_CXX)
endif()