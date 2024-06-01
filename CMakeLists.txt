cmake_minimum_required (VERSION 3.22)
project (cv_Example) 

# enable testing
if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
    include(CTest)
    option(SHAKHBAT_CV_BUILD_TESTING "Enable testing" ON)
endif()

add_subdirectory (shakhbat_cv)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(run run.cpp)
target_link_libraries(run PUBLIC shakhbat_cv test_cv)
