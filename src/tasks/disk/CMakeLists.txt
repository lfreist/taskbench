file(GLOB SRC "*.cpp")

add_library(disk_tasks SHARED ${SRC})

add_library(disk_tasks_static STATIC ${SRC})