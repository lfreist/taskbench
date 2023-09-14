file(GLOB SRC "*.cpp")

add_library(network_tasks SHARED ${SRC})

add_library(network_tasks_static STATIC ${SRC})