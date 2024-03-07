cmake_minimum_required(VERSION 3.0)
project(TCP)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE Debug)

# Find the GTKmm package
find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

# Include directories for GTKmm
include_directories(${GTKMM_INCLUDE_DIRS})

# Include Boost headers
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../dependencies/boost_1_75_0)

# Add your application files
add_executable(receiver receiver.cpp)
add_executable(plotData plotReceivedData.cpp)
add_executable(test test.cpp)

#find_package(Threads REQUIRED)
#target_link_libraries(receiver ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(plotData ${GTKMM_LIBRARIES})
target_link_libraries(test ${GTKMM_LIBRARIES})
target_link_libraries(receiver ${GTKMM_LIBRARIES})
# copy python script and json sample to build folder
#install(FILES sender.py DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/build)
#install(FILES sample.json DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/build)
