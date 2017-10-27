cmake_minimum_required (VERSION 3.5)
project (DLA)
set (DLA_VERSION_MAJOR 1)
set (DLA_VERSION_MINOR 0)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}$/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}$)
file(GLOB SOURCES "src/*.cpp")
add_executable(simulation ${SOURCES})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(Threads REQUIRED)

target_link_libraries(simulation ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${THREADS_INCLUDE_DIR})
include_directories(include ${OPNGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})