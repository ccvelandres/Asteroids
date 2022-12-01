


add_executable(rendering ${CMAKE_CURRENT_SOURCE_DIR}/rendering.cpp)
target_link_libraries(rendering PUBLIC SDL2 SDL2_image OpenGL::GL GLEW::glew spdlog core)

file(COPY assets DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/references/opengl")

set_target_properties(rendering 
PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/references/opengl"
    BUILD_RPATH "${BUILD_RPATH};${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
    INSTALL_RPATH "${INSTALL_RPATH};${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")