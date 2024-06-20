cmake_minimum_required(VERSION 3.22)

# Find all test source files
file(GLOB_RECURSE TEST_SOURCES *.cpp)

# Create a list to store all test targets
set(TEST_TARGETS)

# Create a target for each test file
foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(${TEST_NAME} qlm::shakhbat_cv) 
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    list(APPEND TEST_TARGETS ${TEST_NAME})
endforeach()

# Create a target for Test_All
add_custom_target(Test_All
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS ${TEST_TARGETS}
)

