# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project (DicomImageProcessor CXX)

ENABLE_TESTING()

# Add GoogleTest as a subdirectory so it can be built along with your project
add_subdirectory(extern/googletest)

# Include the GoogleTest headers
include_directories(extern/googletest/googletest/include)

find_package(OpenCV REQUIRED)


include_directories(${OpenCV_INCLUDE_DIRS})

# Adjust runtime library settings
if(MSVC)
    # Replace default flags with desired flags
    string(REPLACE "/MDd" "/MTd" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
endif()


add_subdirectory ("DicomImageProcessor")

add_subdirectory ( "UnitTest")

