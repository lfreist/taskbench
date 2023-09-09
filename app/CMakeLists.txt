add_executable(taskbench main.cpp)
target_link_libraries(taskbench PRIVATE cpu_tasks ram_tasks)