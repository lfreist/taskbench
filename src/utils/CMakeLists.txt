file(GLOB SRC "*.cpp")

add_library(utils ${SRC})
target_link_libraries(utils PUBLIC fmt::fmt)