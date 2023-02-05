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

set(CORE_TARGET "core" CACHE INTERNAL "")
add_library(${CORE_TARGET} ${SRC_CORE})
set_target_properties(${CORE_TARGET} PROPERTIES VERSION 0.1.0)

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
# find_package(Freetype REQUIRED)

# if(NOT FREETYPE_FOUND)
#     message(STATUS "Fetching spdlog...")
#     FetchContent_Declare(
#         spdlog_project
#         GIT_REPOSITORY https://github.com/gabime/spdlog.git
#         GIT_TAG v1.10.0
#         SOURCE_DIR ${PROJECT_LIBRARY_SOURCE_DIR}/spdlog
#         UPDATE_DISCONNECTED ON
#     )
#     FetchContent_MakeAvailable(spdlog_project)
# else()
#     message(STATUS "Found library: Freetype")
#     message(STATUS "    Library: ${FREETYPE_LIBRARIES}")
#     message(STATUS "    Include: ${FREETYPE_INCLUDE_DIRS}")
#     message(STATUS "    Version: ${FREETYPE_VERSION_STRING}")
# endif()

# target_link_libraries(${CORE_TARGET} PUBLIC Freetype::Freetype)


pkg_check_modules(Freetype2_lib REQUIRED IMPORTED_TARGET freetype2)

if(Freetype2_lib_FOUND)
    message(STATUS "Found library: Freetype2")
    message(STATUS "    Library: ${Freetype2_lib_LIBRARIES}")
    message(STATUS "    Link:    ${Freetype2_lib_LINK_LIBRARIES}")
    message(STATUS "    Include: ${Freetype2_lib_INCLUDE_DIRS}")
    message(STATUS "    Version: ${Freetype2_lib_VERSION}")
else()
    message(FATAL_ERROR "Freetype2 not found")
endif()
target_link_libraries(${CORE_TARGET} PUBLIC PkgConfig::Freetype2_lib)

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

find_package(spdlog QUIET)
if(spdlog_FOUND)    
    message(STATUS "Found spdlog: ${spdlog_VERSION}")
    target_link_libraries(${CORE_TARGET} PUBLIC $<$<BOOL:$<TARGET_PROPERTY:CONFIG_CORE_LOG_ENABLE>>:spdlog>)
else()
    FetchContent_Declare( spdlog_project
        SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_BRANCH v1.x
        GIT_SHALLOW ON
        UPDATE_DISCONNECTED ON)

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
endif()


# ######################################################
# #### SDL2

find_package(SDL2 QUIET)
if(TARGET SDL2::SDL2)
    message(STATUS "Found library: SDL2")
    target_link_libraries(${CORE_TARGET} PUBLIC SDL2::SDL2)
else()
    # Try pkg config
    pkg_search_module(SDL2_lib REQUIRED sdl2 IMPORTED_TARGET)
    if(SDL2_lib_FOUND)
        target_link_libraries(${CORE_TARGET} PUBLIC PkgConfig::SDL2_lib)
    else()
        message(FATAL_ERROR "SDL2 not found")
    endif()
endif()

# ######################################################
# ##### SDL2_image

find_package(SDL_image QUIET)
if(TARGET SDL2_image::SDL2_image)
    message(STATUS "Found library: SDL2_image")
    target_link_libraries(${CORE_TARGET} PUBLIC SDL2_image::SDL2_image)
else()
    # Try pkg config
    pkg_search_module(SDL2_image_lib REQUIRED SDL2_image IMPORTED_TARGET)
    if(SDL2_image_lib_FOUND)
        target_link_libraries(${CORE_TARGET} PUBLIC PkgConfig::SDL2_image_lib)
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
target_link_libraries(${CORE_TARGET} PUBLIC SDL2_sound::SDL2_sound)

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
if(UNIX)

elseif(WIN32)
    set(ALSOFT_REQUIRE_WASAPI ON CACHE INTERNAL "")
endif()
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
if(TARGET glm::glm)
    message(STATUS "Found library: SDL2")
    target_link_libraries(${CORE_TARGET} PUBLIC glm::glm)
else()
    # Try pkg config
    pkg_search_module(glm_lib REQUIRED glm IMPORTED_TARGET)
    if(glm_lib_FOUND)
        target_link_libraries(${CORE_TARGET} PUBLIC PkgConfig::glm_lib)
    else()
        message(FATAL_ERROR "glm not found")
    endif()
endif()


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
