add_executable(BlitzBench main.cpp)
target_link_libraries(BlitzBench PRIVATE taskbench::taskbench_static)