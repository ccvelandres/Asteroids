cmake_minimum_required(VERSION 3.21)

include(Kconfig)
include(ExternalProject)
include(FetchContent)
include(FindPkgConfig)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake;${CMAKE_MODULE_PATH}")

message(STATUS "-------- CORE ---------")
kconfig_add_kconfig("${CMAKE_CURRENT_LIST_DIR}/include/core/Kconfig")

message(STATUS "Adding sources for ${CORE_TARGET}")
set(SRC_CORE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/game.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/time.cpp)

add_library(${CORE_TARGET} ${SRC_CORE})
set_target_properties(${CORE_TARGET} PROPERTIES VERSION 0.1.0)

kconfig_add_target(${CORE_TARGET})
add_subdirectory(src)

target_include_directories(${CORE_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${CORE_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/core)
target_include_directories(${CORE_TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CORE_LIBRARY_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/install)
message(STATUS "CORE_LIBRARY_INSTALL_DIR: ${CORE_LIBRARY_INSTALL_DIR}")

# #####################################################
# Link Libraries

# ######################################################
# ##### Threads

find_package(Threads REQUIRED)
target_link_libraries(${CORE_TARGET} PUBLIC Threads::Threads)

# ######################################################
# ##### easy_profiler (static)

FetchContent_Declare( easy_profiler_project
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/easy_profiler)

set(SDLSOUND_BUILD_TEST OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(easy_profiler_project)
target_link_libraries(${CORE_TARGET} PUBLIC $<$<BOOL:$<TARGET_PROPERTY:CONFIG_CORE_PROFILER_ENABLE>>:easy_profiler>)

# ######################################################
# ##### Assimp

FetchContent_Declare( assimp_project
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/assimp)

set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE INTERNAL "")
set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "")
set(ASSIMP_INSTALL OFF CACHE INTERNAL "")
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE INTERNAL "")
set(ASSIMP_NO_EXPORT OFF CACHE INTERNAL "")
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE INTERNAL "")
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE INTERNAL "")

FetchContent_MakeAvailable(assimp_project)
target_link_libraries(${CORE_TARGET} PUBLIC assimp::assimp)

# ######################################################
# ##### FreeType
find_package(Freetype REQUIRED)

if(NOT FREETYPE_FOUND)
    message(STATUS "Fetching spdlog...")
    FetchContent_Declare(
        spdlog_project
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.10.0
        SOURCE_DIR ${PROJECT_LIBRARY_SOURCE_DIR}/spdlog
        UPDATE_DISCONNECTED ON
    )
    FetchContent_MakeAvailable(spdlog_project)
else()
    message(STATUS "Found library: Freetype")
    message(STATUS "    Library: ${FREETYPE_LIBRARIES}")
    message(STATUS "    Include: ${FREETYPE_INCLUDE_DIRS}")
    message(STATUS "    Version: ${FREETYPE_VERSION_STRING}")
endif()

target_link_libraries(${CORE_TARGET} PUBLIC Freetype::Freetype)

# ######################################################
# ##### JSON
find_package(nlohmann_json QUIET)

if(NOT nlohmann_json_FOUND)
    message(STATUS "Fetching nlohmann_json...")
    FetchContent_Declare(
        nlohmann_json_project
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.2
        GIT_SHALLOW TRUE
        SOURCE_DIR ${PROJECT_LIBRARY_SOURCE_DIR}/json
        UPDATE_DISCONNECTED ON
    )
    FetchContent_MakeAvailable(nlohmann_json_project)
else()
    message(STATUS "Found nlohmann_json: ${nlohmann_json_VERSION}")
endif()

target_link_libraries(${CORE_TARGET} PUBLIC nlohmann_json::nlohmann_json)

# ######################################################
# ##### spdlog


FetchContent_Declare( spdlog_project
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/spdlog)

set(SPDLOG_BUILD_BENCH OFF CACHE INTERNAL "")
set(SPDLOG_BUILD_EXAMPLE OFF CACHE INTERNAL "")
set(SPDLOG_BUILD_TESTS OFF CACHE INTERNAL "")
set(SPDLOG_FMT_EXTERNAL ON CACHE INTERNAL "")

FetchContent_MakeAvailable(spdlog_project)
target_link_libraries(${CORE_TARGET} PUBLIC spdlog::spdlog)

# ######################################################
# #### SDL2

find_package(SDL2 REQUIRED)
if(SDL2_FOUND)
    message(STATUS "Found library: SDL2")
    message(STATUS "    Library: ${SDL2_LIBRARIES}")
    message(STATUS "    Link:    ${SDL2_LINK_LIBRARIES}")
    message(STATUS "    Include: ${SDL2_INCLUDE_DIRS}")
    message(STATUS "    Version: ${SDL2_VERSION}")
else()
    message(FATAL_ERROR "SDL2 not found")
endif()
target_link_libraries(${CORE_TARGET} PUBLIC SDL2::SDL2)

# ######################################################
# ##### SDL2_image

find_package(SDL2_image REQUIRED)
if(SDL2_image_FOUND)
    message(STATUS "Found library: SDL2_image")
else()
    message(FATAL_ERROR "SDL2_image not found")
endif()
target_link_libraries(${CORE_TARGET} PUBLIC SDL2_image::SDL2_image)

# ######################################################
# ##### SDL2_sound (shared library)

FetchContent_Declare( SDL2_sound_project
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/SDL_sound)

set(SDLSOUND_BUILD_TEST OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(SDL2_sound_project)
target_link_libraries(${CORE_TARGET} PUBLIC SDL2_sound)

# ######################################################
# ##### OpenAL-soft

FetchContent_Declare( openal_soft_project
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/openal-soft)

set(ALSOFT_EXAMPLES OFF CACHE INTERNAL "")
set(ALSOFT_UTILS OFF CACHE INTERNAL "")
set(ALSOFT_INSTALL_EXAMPLES OFF CACHE INTERNAL "")
set(ALSOFT_INSTALL_UTILS OFF CACHE INTERNAL "")
set(ALSOFT_NO_CONFIG_UTIL OFF CACHE INTERNAL "")
set(ALSOFT_STATIC_LIBGCC`ON CACHE INTERNAL "")
set(ALSOFT_STATIC_STDCXX`ON CACHE INTERNAL "")
set(ALSOFT_REQUIRE_WASAPI ON CACHE INTERNAL "")
set(LIBTYPE SHARED CACHE INTERNAL "")

FetchContent_MakeAvailable(openal_soft_project)
target_link_libraries(${CORE_TARGET} PUBLIC OpenAL::OpenAL)

# ######################################################
# ##### Vulkan
find_package(Vulkan QUIET)

if(NOT Vulkan_FOUND)
    message(WARNING "Vulkan not found")
endif()

target_link_libraries(${CORE_TARGET} PUBLIC $<$<BOOL:$<TARGET_PROPERTY:CONFIG_CORE_RENDERER_VULKAN>>:Vulkan::Vulkan>)

# ######################################################
# ##### glm
find_package(glm QUIET)

if(NOT glm_FOUND)
    message(STATUS "Fetching glm...")
    FetchContent_Declare(
        glm_project
        GIT_REPOSITORY https://github.com/g-truc/glm
        GIT_TAG 0.9.9.8
        GIT_SHALLOW TRUE
        SOURCE_DIR ${PROJECT_LIBRARY_SOURCE_DIR}/glm
        UPDATE_DISCONNECTED ON
    )
    FetchContent_MakeAvailable(glm_project)
elseif(glm_VERSION VERSION_GREATER_EQUAL "0.9.9")
    message(STATUS "Found glm: ${glm_VERSION}")
else()
    message(FATAL_ERROR "glm not found")
endif()

target_link_libraries(${CORE_TARGET} PUBLIC glm)

# ######################################################
# ##### OpenGL
find_package(OpenGL)

if(NOT OpenGL_FOUND)
    message(WARNING "OpenGL not found")
endif()

target_link_libraries(${CORE_TARGET} PUBLIC $<$<BOOL:$<TARGET_PROPERTY:CONFIG_CORE_RENDERER_OPENGL>>:OpenGL::GL>)

# ######################################################
# ##### GLEW

find_package(GLEW REQUIRED)
if(GLEW_FOUND)
    message(STATUS "Found library: GLEW")
    message(STATUS "    Version: ${GLEW_VERSION}")
else()
    message(FATAL_ERROR "GLEW not found")
endif()

target_link_libraries(${CORE_TARGET} PUBLIC $<$<BOOL:$<TARGET_PROPERTY:CONFIG_CORE_RENDERER_OPENGL>>:GLEW::glew>)

# ######################################################
# ##### libfmt

# find_package(fmt REQUIRED)
# if(fmt_FOUND)
#     message(STATUS "Found library: fmt")
#     message(STATUS "    Version: ${fmt_VERSION}")
# else()
#     message(FATAL_ERROR "fmt not found")
# endif()
# target_link_libraries(${CORE_TARGET} PUBLIC fmt::fmt)

# #####################################################
# Post-commands


# #####################################################
# Tests
set(CORE_BUILD_TESTS ON)
if( CORE_BUILD_TESTS )
    add_subdirectory(test)
endif()

# TODO: add post command to copy shared libs (so/dll) post build
message(STATUS "------ END CORE -------")
