cmake_minimum_required(VERSION 3.22)
project (shakhbat_cv CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Source files
file(GLOB_RECURSE SOURCES source/*.cpp)

# Create the static library
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include dependencies)

# Check if testing is enabled
if(SHAKHBAT_CV_BUILD_TESTING AND BUILD_TESTING)
    message(STATUS "Testing for ${PROJECT_NAME} is enabled")
    add_subdirectory(test)
endif()

# install