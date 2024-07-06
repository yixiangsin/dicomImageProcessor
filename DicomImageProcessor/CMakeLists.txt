# CMakeList.txt : CMake project for DicomImageProcessor, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
include(FetchContent)
FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
)
FetchContent_MakeAvailable(argparse)

include_directories(${OpenCV_INCLUDE_DIRS})

add_library(DicomImageProcessorLib
    DicomImageProcessor.cpp
    DicomImageProcessor.h
)

# Include directories for the library
target_include_directories(DicomImageProcessorLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable (DicomImageProcessor "DicomImageProcessor.cpp" "DicomImageProcessor.h" "main.cpp")
target_link_libraries(DicomImageProcessor ${OpenCV_LIBS} argparse)

# TODO: Add tests and install targets if needed.
