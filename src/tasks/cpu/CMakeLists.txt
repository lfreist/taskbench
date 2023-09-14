file(GLOB SRC "*.cpp")

add_library(cpu_tasks SHARED ${SRC})
target_link_libraries(cpu_tasks PUBLIC AES libzstd_static benchmark)

add_library(cpu_tasks_static STATIC ${SRC})
target_link_libraries(cpu_tasks_static PUBLIC AES_static libzstd_static benchmark_static)