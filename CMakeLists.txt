# CMakeList.txt : CMake project for eve_arbitrator, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)

project("eve_arbitrator")

# added
find_package(CURL REQUIRED)

# Add source to this project's executable.
add_executable(eve_arbitrator "handles.h" "handles.cpp" "task.h" "task.cpp" "context.h" "context.cpp" "main.cpp" "task_load_regions.h" "task_load_regions.cpp" "order.h" "order.cpp")

# TODO: Add tests and install targets if needed.

# added
include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(eve_arbitrator ${CURL_LIBRARIES})