add_executable(BenchBlitz main.cpp)
target_link_libraries(BenchBlitz PRIVATE taskbench::taskbench_static)