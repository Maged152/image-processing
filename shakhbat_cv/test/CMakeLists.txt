cmake_minimum_required(VERSION 3.22)

set(PARENT_PROJECT_NAME "${PROJECT_NAME}")
project (test_cv CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Source files
file(GLOB_RECURSE SOURCES source/*.cpp)

# Create the static library
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PARENT_PROJECT_NAME})
add_library(qlm::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
