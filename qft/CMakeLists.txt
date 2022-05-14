cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(apsp)

list(APPEND CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/libtorch/)
find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

add_executable(apsp apsp.c++)
target_link_libraries(apsp "${TORCH_LIBRARIES}")
set_property(TARGET apsp PROPERTY CXX_STANDARD 14)
