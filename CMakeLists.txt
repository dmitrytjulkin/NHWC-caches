cmake_minimum_required(VERSION 3.14)
project(2Q_Cache)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(COMMON_SOURCES
    get_params.cpp
    slow_get_page.cpp
)

add_executable(main_app
    main.cpp
    ${COMMON_SOURCES}
)
target_include_directories(main_app PRIVATE .)

add_executable(run_tests
    test_2q.cpp
    ${COMMON_SOURCES}
)
target_link_libraries(run_tests PRIVATE GTest::gtest_main)
target_include_directories(run_tests PRIVATE .)