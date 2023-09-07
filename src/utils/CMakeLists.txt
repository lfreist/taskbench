file(GLOB SRC "*.cpp")

add_library(utils STATIC ${SRC})
target_link_libraries(utils PUBLIC fmt::fmt)