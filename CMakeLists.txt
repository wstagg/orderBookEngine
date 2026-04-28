cmake_minimum_required(VERSION 3.30)

project(orderBookEngineTopLevel)

set(GLOB_SRC ${PROJECT_SOURCE_DIR}/src/cpp)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-std=c++2a -Wall -Wextra -Werror -Wpedantic")

include(CTest)

add_subdirectory(orderBookEngine)
# FetchContent to load and build googletest
include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG main
)

#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(orderBookEngineTests 
tests/SortedOrderListTests.cpp
tests/MatchingEngineTests.cpp
tests/OrderTests.cpp
tests/MemoryPoolTests.cpp
tests/DynamicArrayTests.cpp
tests/ThreadSafeQueueTests.cpp)

target_include_directories(orderBookEngineTests PRIVATE ${PROJECT_SOURCE_DIR}/orderBookEngine/src/include)
target_link_libraries(orderBookEngineTests GTest::gtest_main orderBookEngine)

include(GoogleTest)
gtest_discover_tests(orderBookEngineTests)