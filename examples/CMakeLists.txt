# Find all examples source files
file(GLOB_RECURSE EXAMPLE_SOURCES *.cpp)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets)
 
if (QT_FOUND)
    message(STATUS "Found Qt${QT_VERSION_MAJOR}")
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
endif()

foreach(EXAMPLE_SOURCE ${EXAMPLE_SOURCES})
    get_filename_component(EXAMPLE_NAME ${EXAMPLE_SOURCE} NAME_WE)
    
    # Check if the file name starts with "qt_"
    if (EXAMPLE_NAME MATCHES "^qt_")
        if (ON)
            add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCE})
            target_link_libraries(${EXAMPLE_NAME} PRIVATE qlm::shakhbat_cv Qt${QT_VERSION_MAJOR}::Widgets)
            set_target_properties(${EXAMPLE_NAME} PROPERTIES
                          CXX_STANDARD 20
                          CXX_EXTENSIONS OFF)
        else()
            message(WARNING "Skipping ${EXAMPLE_NAME}: Qt not found, cannot compile.")
        endif()
    else()
        add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCE})
        target_link_libraries(${EXAMPLE_NAME} PRIVATE qlm::shakhbat_cv)
        set_target_properties(${EXAMPLE_NAME} PROPERTIES
                          CXX_STANDARD 20
                          CXX_EXTENSIONS OFF)
    endif()

endforeach()
