add_library(benchmark SHARED benchmark.cpp)
target_link_libraries(benchmark PUBLIC utils)

add_library(benchmark_static STATIC benchmark.cpp)
target_link_libraries(benchmark_static PUBLIC utils_static)

add_subdirectory(utils)
add_subdirectory(tasks)

add_library(taskbench SHARED taskbench.cpp)
target_link_libraries(taskbench PUBLIC tasks)

add_library(taskbench_static STATIC taskbench.cpp)
target_link_libraries(taskbench_static PUBLIC tasks_static)

add_library(${PROJECT_NAME}::taskbench ALIAS taskbench)
add_library(${PROJECT_NAME}::taskbench_static ALIAS taskbench_static)