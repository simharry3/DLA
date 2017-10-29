cmake_minimum_required (VERSION 3.5)
project (DLA)
set (DLA_VERSION_MAJOR 1)
set (DLA_VERSION_MINOR 0)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenMP REQUIRED)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread -fopenmp")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -pthread")
include_directories(include)

add_executable(simulation
                src/simulation.cpp
                src/types.cpp
                src/visualizer.cpp
                src/utilities.cpp)
target_link_libraries(simulation
                        ${CMAKE_THREADS_LIB_INIT}
                        ${OPENGL_LIBRARIES}
                        ${GLUT_glut_LIBRARY})


# add_executable(tests
#                 tests/tests.cpp
#                 src/types.cpp
#                 src/visualizer.cpp
#                 src/utilities.cpp)
# target_link_libraries(tests
#                         ${CMAKE_THREADS_LIB_INIT}
#                         ${OPENGL_LIBRARIES}
#                         ${GLUT_glut_LIBRARY})
