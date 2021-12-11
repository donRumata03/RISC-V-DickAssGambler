# 'Google_test' is the subproject name
project(Google_tests)

# 'lib' is the folder with Google Test sources
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# 'Google_Tests_run' is the target name
add_executable(Google_Tests_run bit_utils_tests.cpp e_ident_test.cpp filesystem_tests.cpp detect_locations.h)
target_link_libraries(Google_Tests_run gtest gtest_main)