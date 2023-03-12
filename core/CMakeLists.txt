cmake_minimum_required(VERSION 3.21)

include(Kconfig)
include(ExternalProject)
include(FetchContent)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake;${CMAKE_MODULE_PATH}")

message(STATUS "-------- CORE ---------")
kconfig_add_kconfig("${CMAKE_CURRENT_LIST_DIR}/include/core/Kconfig")

message(STATUS "Adding sources for ${CORE_TARGET}")
set(SRC_CORE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/game.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/time.cpp)

set(CORE_TARGET "core" CACHE INTERNAL "")
add_library(${CORE_TARGET} SHARED ${SRC_CORE})
set_target_properties(${CORE_TARGET} PROPERTIES VERSION 0.1.0)

if(WIN32)
    # Link static libs
    target_link_options(${CORE_TARGET} PRIVATE 
        "-static-libgcc"
        "-Wl,--push-state,-Bstatic,-lstdc++,--pop-state"
        "-Wl,--push-state,-Bstatic,-lwinpthread,--pop-state")
endif()

kconfig_add_target(${CORE_TARGET})
add_subdirectory(src)

target_include_directories(${CORE_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${CORE_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/core)
target_include_directories(${CORE_TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CORE_CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake CACHE INTERNAL "")

include(${CORE_CMAKE_MODULE_PATH}/assets.cmake)
core_set_asset_directory(${CMAKE_CURRENT_BINARY_DIR}/assets)
core_add_asset_directory(${CMAKE_CURRENT_LIST_DIR}/assets)

# #####################################################
# Link Libraries

# ######################################################
# ##### Threads

find_package(Threads REQUIRED)
target_link_libraries(${CORE_TARGET} PRIVATE Threads::Threads)

# ######################################################
# #### SDL2

find_package(SDL2 REQUIRED)
if(TARGET SDL2::SDL2)
    message(STATUS "Found library: SDL2")
    target_link_libraries(${CORE_TARGET} PRIVATE SDL2::SDL2)
else()
    message(FATAL_ERROR "SDL2 not found")
endif()

# ######################################################
# ##### SDL2_image

find_package(SDL_image QUIET)
if(SDL_image_FOUND)
    message(STATUS "Found library: SDL_image")
    target_link_libraries(${CORE_TARGET} PRIVATE SDL_image::SDL_image)
else()
    find_package(SDL2_image QUIET)
    if(SDL2_image_FOUND)
        message(STATUS "Found library: SDL2_image")
        target_link_libraries(${CORE_TARGET} PRIVATE SDL2_image::SDL2_image)
    else()
        message(FATAL_ERROR "SDL2_image not found")
    endif()
endif()

# ######################################################
# ##### SDL2_sound (shared library)

FetchContent_Declare( SDL2_sound_project
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/SDL_sound)

set(SDLSOUND_BUILD_TEST OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(SDL2_sound_project)
target_link_libraries(${CORE_TARGET} PRIVATE SDL2_sound::SDL2_sound)

# ######################################################
# ##### easy_profiler (static)

FetchContent_Declare( easy_profiler_project
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/easy_profiler)

set(SDLSOUND_BUILD_TEST OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(easy_profiler_project)
target_link_libraries(${CORE_TARGET} PRIVATE $<$<BOOL:$<TARGET_PROPERTY:CONFIG_CORE_PROFILER_ENABLE>>:easy_profiler>)

# ######################################################
# ##### Assimp

find_package(assimp QUIET)
if(assimp_FOUND)
    target_link_libraries(${CORE_TARGET} PRIVATE assimp::assimp)
else()
    FetchContent_Declare( assimp_project
        SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/assimp
        GIT_REPOSITORY https://github.com/assimp/assimp
        GIT_TAG v5.2.5
        UPDATE_DISCONNECTED ON)

    set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE INTERNAL "")
    set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "")
    set(ASSIMP_INSTALL OFF CACHE INTERNAL "")
    set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE INTERNAL "")
    set(ASSIMP_NO_EXPORT OFF CACHE INTERNAL "")
    set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE INTERNAL "")
    set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE INTERNAL "")

    FetchContent_MakeAvailable(assimp_project)
    target_link_libraries(${CORE_TARGET} PRIVATE assimp::assimp)
endif()

# ######################################################
# ##### FreeType

find_package(Freetype REQUIRED)
if(Freetype_FOUND)
    message(STATUS "Found Freetype")
else()
    message(FATAL_ERROR "Freetype not found")
endif()
target_link_libraries(${CORE_TARGET} PRIVATE Freetype::Freetype)

# ######################################################
# ##### JSON

find_package(nlohmann_json QUIET)
if(nlohmann_json_FOUND)
    message(STATUS "Found nlohmann_json: ${nlohmann_json_VERSION}")
    target_link_libraries(${CORE_TARGET} PRIVATE nlohmann_json::nlohmann_json)
else()
    message(FATAL_ERROR "Library not found: nlohmann_json")
endif()


# ######################################################
# ##### libfmt

find_package(fmt REQUIRED)
if(fmt_FOUND)
    message(STATUS "Found library: fmt")
    message(STATUS "    Version: ${fmt_VERSION}")
else()
    message(FATAL_ERROR "fmt not found")
endif()
target_link_libraries(${CORE_TARGET} PUBLIC fmt::fmt)

# ######################################################
# ##### spdlog

FetchContent_Declare( spdlog_project
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/spdlog)

set(SPDLOG_BUILD_BENCH OFF CACHE INTERNAL "")
set(SPDLOG_BUILD_EXAMPLE OFF CACHE INTERNAL "")
set(SPDLOG_BUILD_TESTS OFF CACHE INTERNAL "")
if(TARGET fmt::fmt)
    set(SPDLOG_FMT_EXTERNAL ON CACHE INTERNAL "")
else()
    set(SPDLOG_FMT_EXTERNAL OFF CACHE INTERNAL "")
endif()

FetchContent_MakeAvailable(spdlog_project)
target_link_libraries(${CORE_TARGET} PUBLIC $<$<BOOL:$<TARGET_PROPERTY:CONFIG_CORE_LOG_ENABLE>>:spdlog::spdlog>)

# ######################################################
# ##### OpenAL-soft

find_package(OpenAL QUIET)
if(OpenAL_FOUND)
    target_link_libraries(${CORE_TARGET} PRIVATE OpenAL::OpenAL)
else()
    FetchContent_Declare( openal_soft_project
        SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/openal-soft
        GIR_REPOSITORY https://github.com/kcat/openal-soft
        GIT_TAG 1.22.2
        UPDATE_DISCONNECTED OFF)

    set(LIBTYPE SHARED CACHE INTERNAL "")
    set(ALSOFT_EXAMPLES OFF CACHE INTERNAL "")
    set(ALSOFT_UTILS OFF CACHE INTERNAL "")
    set(ALSOFT_INSTALL_EXAMPLES OFF CACHE INTERNAL "")
    set(ALSOFT_INSTALL_UTILS OFF CACHE INTERNAL "")
    set(ALSOFT_NO_CONFIG_UTIL OFF CACHE INTERNAL "")
    set(ALSOFT_RTKIT OFF CACHE INTERNAL "")
    # set(ALSOFT_STATIC_LIBGCC`ON CACHE INTERNAL "")
    # set(ALSOFT_STATIC_STDCXX`ON CACHE INTERNAL "")
    # set(ALSOFT_STATIC_WINPTHREAD ON CACHE INTERNAL "")
    # if(UNIX)

    # elseif(WIN32)
    #     set(ALSOFT_BUILD_ROUTER ON CACHE INTERNAL "")
    #     set(ALSOFT_REQUIRE_WASAPI ON CACHE INTERNAL "")
    # endif()

    FetchContent_MakeAvailable(openal_soft_project)
    target_link_libraries(${CORE_TARGET} PRIVATE OpenAL::OpenAL)
endif()

# ######################################################
# ##### Vulkan
find_package(Vulkan QUIET)

if(NOT Vulkan_FOUND)
    message(WARNING "Vulkan not found")
endif()

target_link_libraries(${CORE_TARGET} PRIVATE $<$<BOOL:$<TARGET_PROPERTY:CONFIG_CORE_RENDERER_VULKAN>>:Vulkan::Vulkan>)

# ######################################################
# ##### glm
find_package(glm REQUIRED)
if(TARGET glm)
    message(STATUS "Found library: glm")
    target_link_libraries(${CORE_TARGET} PUBLIC glm)
else()
    message(FATAL_ERROR "glm not found")
endif()

# ######################################################
# ##### OpenGL
find_package(OpenGL)

if(NOT OpenGL_FOUND)
    message(WARNING "OpenGL not found")
endif()

target_link_libraries(${CORE_TARGET} PRIVATE $<$<BOOL:$<TARGET_PROPERTY:CONFIG_CORE_RENDERER_OPENGL>>:OpenGL::GL>)

# ######################################################
# ##### GLEW

find_package(GLEW REQUIRED)
if(GLEW_FOUND)
    message(STATUS "Found library: GLEW")
    message(STATUS "    Version: ${GLEW_VERSION}")
else()
    message(FATAL_ERROR "GLEW not found")
endif()

target_link_libraries(${CORE_TARGET} PRIVATE $<$<BOOL:$<TARGET_PROPERTY:CONFIG_CORE_RENDERER_OPENGL>>:GLEW::glew>)

# #####################################################
# Install rules

install(TARGETS ${CORE_TARGET}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# #####################################################
# Tests
set(CORE_BUILD_TESTS ON)
if( CORE_BUILD_TESTS )
    add_subdirectory(test)
endif()

# TODO: add post command to copy shared libs (so/dll) post build
message(STATUS "------ END CORE -------")
