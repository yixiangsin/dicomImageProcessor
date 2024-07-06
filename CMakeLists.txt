# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project (DicomImageProcessor CXX)

ENABLE_TESTING()

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_subdirectory ("DicomImageProcessor" "test")




# Include sub-projects.

