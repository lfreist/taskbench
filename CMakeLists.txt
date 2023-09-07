cmake_minimum_required(VERSION 3.9)
project(taskbench CXX)
set(CMAKE_CXX_STANDARD 20)

# set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories(include/)

# --- external include directories -------------------------------------------------------------------------------------
include_directories(external/zstd/lib)
include_directories(external/AES/src)
include_directories(external/fmt/include)
include_directories(external/miss-ocl/include)

# --- external builds --------------------------------------------------------------------------------------------------
add_subdirectory(external/zstd/build/cmake)
add_subdirectory(external/AES)
add_subdirectory(external/fmt)
add_subdirectory(external/miss-ocl)


add_subdirectory(src)

add_subdirectory(app)