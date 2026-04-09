cmake_minimum_required(VERSION 3.30)

project(orderBookEngine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-std=c++2a -Wall -Wextra -Werror -Wpedantic")

set(GLOB_SRC ${PROJECT_SOURCE_DIR}/src/cpp)

include(CTest)

#add_executable(orderBookEngine ${GLOB_SRC})

#target_include_directories(orderBookEngine PRIVATE ${PROJECT_SOURCE_DIR}/src/include/)


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

add_executable(orderBookEngineTests ${PROJECT_SOURCE_DIR}/tests/sortedOrderListTests.cpp)

target_include_directories(orderBookEngineTests PRIVATE ${PROJECT_SOURCE_DIR}/src/include)
target_link_libraries(orderBookEngineTests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(orderBookEngineTests)