cmake_minimum_required(VERSION 3.30)

project(orderBookEngine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-std=c++2a -Wall -Wextra -Werror -Wpedantic")

set(ORDERBOOK_SRC ${PROJECT_SOURCE_DIR}/src/cpp/*)

#add_library(orderBookEngine STATIC 
#${PROJECT_SOURCE_DIR}/src/cpp/SortedOrderList.cpp)

#target_include_directories(orderBookEngine PRIVATE /src/include)