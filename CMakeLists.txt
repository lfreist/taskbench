cmake_minimum_required(VERSION 3.9)
project(taskbench CXX)
set(CMAKE_CXX_STANDARD 20)

# Windows specific option
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# for building shared objects
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/taskbench)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/taskbench/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/taskbench/lib)

include_directories(include/)

# --- external include directories -------------------------------------------------------------------------------------
include_directories(external/zstd/lib)
include_directories(external/AES/src)
include_directories(external/fmt/include)
include_directories(external/miss-ocl/include)
include_directories(external/json/include)
include_directories(external/miss-ocl/external/OpenCL/external/OpenCL-CLHPP/include)
include_directories(external/eigen)

# --- external builds --------------------------------------------------------------------------------------------------
add_subdirectory(external/zstd/build/cmake)
add_subdirectory(external/AES)
add_subdirectory(external/fmt EXCLUDE_FROM_ALL)
if (NOT TARGET miss-opencl_static)
    add_subdirectory(external/miss-ocl)
endif ()
add_subdirectory(external/json)

add_subdirectory(src)

add_subdirectory(app)