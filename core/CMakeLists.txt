
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
    target_compile_definitions(${CORE_TARGET} PUBLIC CORE_PROFILER_ENABLE=1)
    message(STATUS "Linking: easy_profiler")
else()
    target_compile_definitions(${CORE_TARGET} PUBLIC CORE_PROFILER_ENABLE=0)
endif(TARGET easy_profiler)

## spdlog
if(TARGET spdlog)
    target_link_libraries(${CORE_TARGET} PUBLIC spdlog)
    message(STATUS "Linking: spdlog")
endif(TARGET spdlog)

## SDL2
if(TARGET SDL2)
    target_link_libraries(${CORE_TARGET} PUBLIC SDL2)
    message(STATUS "Linking: SDL2")
endif(TARGET SDL2)

## SDL2_image
if(TARGET SDL2_image)
    target_link_libraries(${CORE_TARGET} PUBLIC SDL2_image)
    message(STATUS "Linking: SDL2_image")
endif(TARGET SDL2_image)

if(TARGET Vulkan::Vulkan)
    target_link_libraries(${CORE_TARGET} PUBLIC Vulkan::Vulkan)
    message(STATUS "Linking: Vulkan::Vulkan")
endif(TARGET Vulkan::Vulkan)

## glm
if (TARGET glm::glm)
    target_link_libraries(${CORE_TARGET} PUBLIC glm::glm)
    message(STATUS "Linking: glm::glm")
elseif (TARGET glm)
    target_link_libraries(${CORE_TARGET} PUBLIC glm)
    message(STATUS "Linking: glm")
else()
    message(FATAL_ERROR "Could not find glm library")
endif()

## OpenGL
if(TARGET OpenGL::GL)
    target_link_libraries(${CORE_TARGET} PUBLIC OpenGL::GL)
    message(STATUS "Linking: OpenGL::GL")
endif(TARGET OpenGL::GL)

## Glew
if(TARGET GLEW::glew)
    target_link_libraries(${CORE_TARGET} PUBLIC GLEW::glew)
    message(STATUS "Linking: GLEW::glew")
endif(TARGET GLEW::glew)

######################################################
# Post-commands

# TODO: add post command to copy shared libs (so/dll) post build

message(STATUS "------ END CORE -------")