cmake_minimum_required(VERSION 2.8.11)
#Add custom cmake scripts
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

project(MAD)

find_package(MySQL REQUIRED)
include_directories(${MYSQL_INCLUDE_DIR})
link_directories(${MYSQL_LIB_DIR})


ADD_LIBRARY(mad MODULE src/mad.cpp)
set_target_properties(mad PROPERTIES OUTPUT_NAME "mad_udf")

##use c++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
