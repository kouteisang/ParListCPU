cmake_minimum_required(VERSION 3.10)
project(firmcore_baseline)

set(CMAKE_CXX_STANDARD 11)

# 设置 OpenMP 编译和链接选项
set(OpenMP_C_FLAGS "-Xpreprocessor -fopenmp")
set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp")
set(OpenMP_C_LIB_NAMES "omp")
set(OpenMP_CXX_LIB_NAMES "omp")
set(OpenMP_omp_LIBRARY "/opt/homebrew/opt/libomp/lib/libomp.dylib")

# 包含头文件目录
include_directories("/opt/homebrew/opt/libomp/include")
include_directories(${CMAKE_SOURCE_DIR})

# 设置编译和链接选项
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/opt/homebrew/opt/libomp/lib -lomp")

add_subdirectory(Core)
add_subdirectory(Graph)

add_executable(firmcore_baseline main.cpp)

target_link_libraries(firmcore_baseline CoreLib GraphLib ${OpenMP_omp_LIBRARY})
# target_link_libraries(firmcore_baseline  GraphLib ${OpenMP_omp_LIBRARY})