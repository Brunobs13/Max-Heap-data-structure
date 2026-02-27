cmake_minimum_required(VERSION 3.16)
project(max_heap_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(maxheap_core STATIC
    src/core/max_heap.cpp
)

target_include_directories(maxheap_core
    PUBLIC src/core/include
)

add_library(maxheap_shared SHARED
    src/core/max_heap.cpp
    src/core/c_api.cpp
)
set_target_properties(maxheap_shared PROPERTIES OUTPUT_NAME "maxheap")

target_include_directories(maxheap_shared
    PUBLIC src/core/include
)

add_executable(maxheap_cli
    src/apps/heap_cli.cpp
)
target_link_libraries(maxheap_cli PRIVATE maxheap_core)

enable_testing()

add_executable(maxheap_tests
    tests/cpp/test_max_heap.cpp
)
target_link_libraries(maxheap_tests PRIVATE maxheap_core)
add_test(NAME maxheap_tests COMMAND maxheap_tests)
