file(GLOB SRC "*.cpp")

add_library(utils SHARED ${SRC})
target_link_libraries(utils PUBLIC fmt::fmt)

add_library(utils_static STATIC ${SRC})
target_link_libraries(utils_static PUBLIC fmt::fmt-header-only)