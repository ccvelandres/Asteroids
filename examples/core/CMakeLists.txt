

macro (add_example TARGET)
    get_filename_component(CURRENT_FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

    add_executable(${TARGET} ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET}.cpp)
    target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(${TARGET} PUBLIC core)

    set_target_properties(${TARGET} 
    PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/examples/core/${CURRENT_FOLDER_NAME}"
        BUILD_RPATH "${BUILD_RPATH};${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
        INSTALL_RPATH "${INSTALL_RPATH};${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
endmacro()

add_example(utils-queue)