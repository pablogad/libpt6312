cmake_minimum_required (VERSION 2.6)

project (pt6312)
set (SRC_LIB gpio.cc vfd_interface.cc display_def.cc display_ani.cc base_ani.cc rebound_ani.cc)

set (CMAKE_BUILD_TYPE Debug)

add_library(${PROJECT_NAME} STATIC ${SRC_LIB})

add_executable (test_pt6312 test.cpp)
target_link_libraries (test_pt6312 pt6312)

add_executable (create_disp create_disp.cc)
target_link_libraries (create_disp pt6312)
