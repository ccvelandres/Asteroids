
get_filename_component(CURRENT_FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_executable(glm-sample ${CMAKE_CURRENT_SOURCE_DIR}/glm-sample.cpp)
target_link_libraries(glm-sample PUBLIC glm::glm spdlog)

set_target_properties(glm-sample 
PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/references/${CURRENT_FOLDER_NAME}"
    BUILD_RPATH "${BUILD_RPATH};${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
    INSTALL_RPATH "${INSTALL_RPATH};${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")