cmake_minimum_required(VERSION 2.8.11)
#Add custom cmake scripts
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

project(mad_udf)
find_package(MySQL REQUIRED)
include_directories(${MYSQL_INCLUDE_DIR})
link_directories(${MYSQL_LIB_DIR})

include_directories("include")
ADD_LIBRARY(mad_udf MODULE src/mad_udf.cpp)

# Unit Tests
enable_testing()
macro (create_test target)
    add_test (NAME ${target} COMMAND $<TARGET_FILE:${target}>)
endmacro (create_test)

project(mad_tests)
ADD_EXECUTABLE(mad_tests src/mad_test.cpp)
include_directories("include")
create_test(mad_tests)

##use c++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
