file(GLOB SRC "*.cpp")

add_library(gpu_tasks SHARED ${SRC})
target_link_libraries(gpu_tasks PUBLIC benchmark miss-opencl)

add_library(gpu_tasks_static STATIC ${SRC})
target_link_libraries(gpu_tasks_static PUBLIC benchmark_static miss-opencl)