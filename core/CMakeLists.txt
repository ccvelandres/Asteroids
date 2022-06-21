
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


######################################################
# Link Libraries
## easy_profiler
if (TARGET easy_profiler)
    add_dependencies(${CORE_TARGET} easy_profiler_bin)
    target_link_libraries(${CORE_TARGET} PUBLIC easy_profiler)
else()
    target_compile_definitions(${CORE_TARGET} PUBLIC CORE_PROFILER_ENABLE=0)
endif()


## spdlog
target_link_libraries(${CORE_TARGET} PUBLIC spdlog)

## SDL2
target_link_libraries(${CORE_TARGET} PUBLIC SDL2)

## SDL2_image
target_link_libraries(${CORE_TARGET} PUBLIC ${SDL2_IMAGE_LIBRARY})
target_include_directories(${CORE_TARGET} PUBLIC ${SDL2_IMAGE_INCLUDE_DIR})

target_link_libraries(${CORE_TARGET} PUBLIC Vulkan::Vulkan)

if (TARGET glm::glm)
target_link_libraries(${CORE_TARGET} PUBLIC glm::glm)
else()
    message(ERROR "Could not find glm library")
endif()

target_link_libraries(${CORE_TARGET} PUBLIC OpenGL::GL)

target_link_libraries(${CORE_TARGET} PUBLIC GLEW::glew)


message(STATUS "------ END CORE -------")