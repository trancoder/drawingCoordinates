cmake_minimum_required(VERSION 3.0)
project(TCP)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE Debug)

# Include Boost headers
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../dependencies/boost_1_75_0)

# Add your application files
add_executable(receiver receiver.cpp)


#find_package(Threads REQUIRED)
#target_link_libraries(receiver ${CMAKE_THREAD_LIBS_INIT})

# copy python script and json sample to build folder
#install(FILES sender.py DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/build)
#install(FILES sample.json DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/build)
