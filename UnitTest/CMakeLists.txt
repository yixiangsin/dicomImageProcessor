cmake_minimum_required (VERSION 3.8)
# Add your test executable

add_executable(DicomImageProcessorUnitTest DicomImageProcessorUnitTest.cpp)

# Link against GoogleTest and your project's library if needed
target_link_libraries(DicomImageProcessorUnitTest gtest gtest_main ${OpenCV_LIBS}  DicomImageProcessorLib )

# Add the tests to be run
add_test(NAME DicomImageProcessorUnitTest COMMAND DicomImageProcessorUnitTest)

