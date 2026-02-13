# CMakeList.txt : CMake project for GraphBackend, include source and define
# project specific logic here.
#

# Add source to this project's executable.
cmake_minimum_required(VERSION 4.1)
add_executable (DChart "DChart.cpp" "DChart.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET DChart PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
 