add_subdirectory(cpu)
add_subdirectory(disk)
add_subdirectory(gpu)
add_subdirectory(network)
add_subdirectory(ram)

file(GLOB SRC "*/*.cpp")

add_library(tasks SHARED ${SRC})
target_link_libraries(tasks PUBLIC utils AES libzstd_static benchmark miss-opencl)

add_library(tasks_static STATIC ${SRC})
target_link_libraries(tasks_static PUBLIC utils_static AES_static libzstd_static benchmark_static miss-opencl)