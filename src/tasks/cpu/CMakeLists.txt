file(GLOB SRC "*.cpp")

add_library(cpu_tasks STATIC ${SRC})
target_link_libraries(cpu_tasks PUBLIC utils AES libzstd_static)