cmake_minimum_required(VERSION 3.22)
project (shakhbat_cv CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Source files
file(GLOB_RECURSE SOURCES source/*.cpp)

# Create the static library
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Check if testing is enabled
include(CTest)
if(BUILD_TESTING)
    # Add subdirectory for tests
    add_subdirectory(test)
endif()

# install