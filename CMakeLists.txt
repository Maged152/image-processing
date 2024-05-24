cmake_minimum_required (VERSION 3.22)
project (cv_Example) 

add_subdirectory (shakhbat_cv)

add_executable(run run.cpp)
target_link_libraries(run PUBLIC shakhbat_cv test_cv)
