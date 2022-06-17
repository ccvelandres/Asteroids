
message(STATUS "-------- CORE ---------")

# file(GLOB_RECURSE SRC_C_CORE ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
# file(GLOB_RECURSE SRC_CPP_CORE ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
message(STATUS "Adding sources for ${CORE_TARGET}")
set(SRC_CORE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/game.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/time.cpp)

add_library(${CORE_TARGET} ${SRC_CORE})
add_subdirectory(src)

target_include_directories(${CORE_TARGET} PRIVATE ${CMAKE_SOURCE_DIR}/core/src)
target_include_directories(${CORE_TARGET} PUBLIC ${CMAKE_SOURCE_DIR}/core/include)

# Add dependencies
add_dependencies(${CORE_TARGET} easy_profiler_bin)

## Link Libraries
target_link_libraries(${CORE_TARGET} PUBLIC easy_profiler)
target_link_libraries(${CORE_TARGET} PUBLIC ${SDL2_LIBRARY})
target_include_directories(${CORE_TARGET} PUBLIC ${SDL2_INCLUDE_DIR})
target_link_libraries(${CORE_TARGET} PUBLIC ${SDL2_IMAGE_LIBRARY})
target_include_directories(${CORE_TARGET} PUBLIC ${SDL2_IMAGE_INCLUDE_DIR})
target_link_libraries(${CORE_TARGET} PUBLIC Vulkan::Vulkan)
target_link_libraries(${CORE_TARGET} PUBLIC spdlog)
target_link_libraries(${CORE_TARGET} PUBLIC glm::glm)
target_link_libraries(${CORE_TARGET} PUBLIC OpenGL::GL)
# target_link_libraries(${CORE_TARGET} PUBLIC OpenGL::GLU)
target_link_libraries(${CORE_TARGET} PUBLIC glew)


message(STATUS "------ END CORE -------")