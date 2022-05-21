cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(apsp)

list(APPEND CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/libtorch/)


find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

add_library(
    apsp
    SHARED
    apsp.c++
)
# add_executable(apsp apsp.c++)

add_compile_options(-Wall -Wextra -pedantic -Werror)
target_link_libraries(apsp "${TORCH_LIBRARIES}")
set_property(TARGET apsp PROPERTY CXX_STANDARD 14)
