cmake_minimum_required(VERSION 3.22)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.15.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

# Find all test source files
file(GLOB_RECURSE TEST_SOURCES *.cpp)



# Create a target for each test file
foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_compile_definitions(${TEST_NAME} PRIVATE LIBRARY_PATH="${CMAKE_SOURCE_DIR}")
    target_link_libraries(${TEST_NAME} qlm::shakhbat_cv GTest::gtest_main)
    target_include_directories(${TEST_NAME} PRIVATE "include")

    gtest_discover_tests(${TEST_NAME} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endforeach()