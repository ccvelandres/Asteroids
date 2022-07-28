
if (TARGET Vulkan::Vulkan)
    message(STATUS "Adding Core: Vulkan Renderer")
    file(GLOB_RECURSE SRC_R_VK ${CMAKE_CURRENT_SOURCE_DIR}/vulkan/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/vulkan/*.c)
    target_compile_definitions(${CORE_TARGET} PRIVATE CORE_RENDERER_VULKAN)
    target_sources(${CORE_TARGET} PRIVATE ${SRC_R_VK})
endif()

if (TARGET OpenGL::GL)
    message(STATUS "Adding Core: OpenGL Renderer")
    file(GLOB_RECURSE SRC_R_GL ${CMAKE_CURRENT_SOURCE_DIR}/opengl/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/opengl/*.c)
    target_compile_definitions(${CORE_TARGET} PRIVATE CORE_RENDERER_OPENGL)
    target_sources(${CORE_TARGET} PRIVATE ${SRC_R_GL})
endif()