cmake_minimum_required (VERSION 2.6)

project (pt6312)
set (SRC_LIB pt6312.c display_def.cpp display_ani.cpp base_ani.cpp rebound_ani.cpp)

set (CMAKE_BUILD_TYPE Debug)

add_library(${PROJECT_NAME} STATIC ${SRC_LIB})

add_executable (test_pt6312 test.cpp)
target_link_libraries (test_pt6312 pt6312 bcm2835)

add_executable (create_disp create_disp.c)
target_link_libraries (create_disp pt6312 bcm2835)
