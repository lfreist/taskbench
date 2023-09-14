add_library(benchmark SHARED benchmark.cpp)
target_link_libraries(benchmark PUBLIC utils)

add_library(benchmark_static STATIC benchmark.cpp)
target_link_libraries(benchmark_static PUBLIC utils_static)

add_subdirectory(utils)
add_subdirectory(tasks)