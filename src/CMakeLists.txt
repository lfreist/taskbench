add_library(benchmark benchmark.cpp)
target_link_libraries(benchmark PUBLIC utils)

add_subdirectory(utils)
add_subdirectory(tasks)