enable_testing ()

find_package (GTest REQUIRED)
include (GoogleTest)

add_executable (unit_tests LIRS_test.cpp)
target_link_libraries (unit_tests PRIVATE cmmn_lib GTest::gtest_main)
gtest_discover_tests (unit_tests)