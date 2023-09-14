file(GLOB SRC "*.cpp")

add_library(gpu_tasks STATIC ${SRC})
target_link_libraries(gpu_tasks PUBLIC benchmark miss-opencl)