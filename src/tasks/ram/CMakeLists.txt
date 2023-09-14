file(GLOB SRC "*.cpp")

add_library(ram_tasks ${SRC})
target_link_libraries(ram_tasks PUBLIC benchmark)