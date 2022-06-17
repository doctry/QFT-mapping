cmake_minimum_required(VERSION 3.0.0)
project(Duostra VERSION 1.0.0)

include(CMakePrintHelpers)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -DDEBUG")
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    cmake_print_variables(CMAKE_CXX_FLAGS_DEBUG)
endif()
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra -DNDEBUG")
if(CMAKE_BUILD_TYPE MATCHES "Release")
    cmake_print_variables(CMAKE_CXX_FLAGS_RELEASE)
endif()

include(CTest)
enable_testing()

# source files
file (GLOB DUOSTRA_DEP
    "src/*.cpp"
    "include/*.hpp"
)
add_executable(Duostra ${DUOSTRA_DEP})
include_directories(include/)

# pytorch
find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")
target_link_libraries(Duostra "${TORCH_LIBRARIES}")
set_property(TARGET Duostra PROPERTY CXX_STANDARD 14)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
