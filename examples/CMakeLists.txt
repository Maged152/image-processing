# Find all examples source files
file(GLOB_RECURSE EXAMPLE_SOURCES *.cpp)

foreach(EXAMPLE_SOURCE ${EXAMPLE_SOURCES})
    get_filename_component(EXAMPLE_NAME ${EXAMPLE_SOURCE} NAME_WE)
    add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCE})
    target_link_libraries(${EXAMPLE_NAME} PRIVATE qlm::shakhbat_cv)
    set_target_properties(${EXAMPLE_NAME} PROPERTIES
                          CXX_STANDARD 20
                          CXX_EXTENSIONS OFF)
endforeach()
