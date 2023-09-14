file(GLOB SRC "*.cpp")

add_library(ram_tasks SHARED ${SRC})
target_link_libraries(ram_tasks PUBLIC benchmark)

add_library(ram_tasks_static STATIC ${SRC})
target_link_libraries(ram_tasks_static PUBLIC benchmark_static)