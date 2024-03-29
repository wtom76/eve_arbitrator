﻿# CMakeList.txt : CMake project for eve_arbitrator, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "-std=c++17 ${CMAKE_CXX_FLAGS}")

project("eve_arbitrator")

find_package(CURL REQUIRED)

# Add source to this project's executable.
add_executable(eve_arbitrator
	"pch.h"
	"util.h"	"util.cpp"
	"handles.h"	"handles.cpp"
	"type.h"	"type.cpp"
	"system.h"	"system.cpp"
	"route.h"	"route.cpp"
	"order.h"	"order.cpp"
	"agent.h"	"agent.cpp"
	"task.h"	"task.cpp"
	"db.h"		"db.cpp"
	"task_load_regions.h"	"task_load_regions.cpp"
	"task_load_orders.h"	"task_load_orders.cpp"
	"task_load_type.h"		"task_load_type.cpp"
	"task_load_system.h"	"task_load_system.cpp"
	"task_load_route.h"		"task_load_route.cpp"
	"anomaly_sensor.h"		"anomaly_sensor.cpp"
	"context.h"	"context.cpp"
	"main.cpp")

# TODO: Add tests and install targets if needed.

# added
include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(eve_arbitrator ${CURL_LIBRARIES} pqxx pq)

if(MSVC)
  target_compile_options(eve_arbitrator PRIVATE /W4 /WX)
else()
  target_compile_options(eve_arbitrator PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
