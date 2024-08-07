cmake_minimum_required(VERSION 3.10)
project(firmcore_baseline)

set(CMAKE_CXX_STANDARD 11)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")

find_package(OpenMP REQUIRED)

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(CoreParallel)
add_subdirectory(Core)
add_subdirectory(Graph)

add_executable(firmcore_baseline main.cpp)

target_link_libraries(firmcore_baseline PUBLIC CoreParallelLib CoreLib GraphLib)

if(OpenMP_CXX_FOUND)
    target_link_libraries(firmcore_baseline PUBLIC OpenMP::OpenMP_CXX)
endif()