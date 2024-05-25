cmake_minimum_required(VERSION 3.22)
project (shakhbat_cv CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Source files
file(GLOB_RECURSE SOURCES source/*.cpp)

# Create the static library
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include dependencies)

option(BUILD_TEST "Enable testing" ON)

# Check if testing is enabled
if(${BUILD_TEST})
    add_subdirectory(test)
endif()

# install