cmake_minimum_required(VERSION 3.16)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules;${CMAKE_MODULE_PATH}")
include(ExternalProject)

SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
SET(CMAKE_INSTALL_RPATH "$\{ORIGIN\}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
set(CMAKE_DOCS_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/docs")
set(BUILD_SHARED_LIBS ON)

set(CMAKE_C_STANDARD 99) # Requires C11 standard
set(CMAKE_CXX_STANDARD 17) # Requires C++20 standard
#################################################################
## Core Options

set(CORE_LOG_ENABLE_TAG OFF CACHE BOOL "Sets whether the tags are included in logs")
set(CORE_UTILS_QUEUE_ENABLE_STDMUTEX ON CACHE BOOL "Sets whether std_mutex queue implementation is enabled")
set(CORE_UTILS_QUEUE_ENABLE_LOCKFREE OFF CACHE BOOL "Sets whether lockfree queue implementation is enabled")
set(CORE_UTILS_QUEUE_IMPL "STD" CACHE STRING "Sets underlying implementation for core queue")
set_property(CACHE CORE_UTILS_QUEUE_IMPL PROPERTY STRINGS STDMUTEX LOCKFREE)
set(CORE_BUILD_EXAMPLES OFF CACHE BOOL "Build examples")
set(CORE_BUILD_REFERENCES OFF CACHE BOOL "Build reference binaries")
set(CORE_BUILD_DOXYGEN OFF CACHE BOOL "Build documentation")
set(CORE_BUILD_DOXYGEN_HTML ON CACHE BOOL "Generate HTML documentation")
set(CORE_BUILD_DOXYGEN_MAN ON CACHE BOOL "Generate MANPAGE documentation")

#################################################################
## Project Settings
get_filename_component(PROJECT_FOLDER_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" PROJECT_NAME ${PROJECT_FOLDER_NAME})
message("Project Name: ${PROJECT_NAME}")

project(${PROJECT_NAME} VERSION 0.1.0)
set(PROJECT_ROOT "${CMAKE_SOURCE_DIR}")
set(PROJECT_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/lib")

if(WIN32)
    add_link_options(-static-libgcc -static-libstdc++)
endif()

#################################################################
## Libraries
include(FindPkgConfig)
include(GNUInstallDirs)

### easy_profiler
if (UNIX)
    message(STATUS "Profiler: easy_profiler (source)")
    set(EASY_OPTION_LISTEN ON CACHE BOOL "Enable automatic startListen on startup" FORCE)
    add_subdirectory(${PROJECT_LIBRARY_DIR}/easy_profiler)
    add_custom_target(easy_profiler_bin DEPENDS easy_profiler
        COMMAND ${CMAKE_COMMAND} -E copy_directory  ${CMAKE_BINARY_DIR}/lib/easy_profiler/bin/ ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )
else()
    # @TODO: Fix enabling easy_profiler in win builds
    message(STATUS "Profiler: disabled")
endif()

### spdlog 
if (WIN32)
    message(STATUS "Logger: spdlog")
    find_package(spdlog REQUIRED)
else()
    message(STATUS "Logger: spdlog (source)")
    add_subdirectory(${PROJECT_LIBRARY_DIR}/spdlog)
endif()

### SDL2
# pkg-config would probably help here
find_library(SDL2_LIBRARIES
    NAMES SDL2 
    HINTS
        ENV SDL2_DIR
    PATHS ${SDL2_DIR})
find_path(SDL2_INCLUDE_DIR 
    NAMES SDL.h 
    HINTS
        ENV SDL2_DIR
    PATHS ${SDL2_DIR} 
    PATH_SUFFIXES SDL2)
if(SDL2_LIBRARIES AND SDL2_INCLUDE_DIR)
    message(STATUS "Found library: SDL2")
    message(STATUS "    Library: ${SDL2_LIBRARIES}")
    message(STATUS "    Include: ${SDL2_INCLUDE_DIR}")
    add_library(SDL2 UNKNOWN IMPORTED)
    set_target_properties(SDL2 PROPERTIES 
        IMPORTED_LOCATION "${SDL2_LIBRARIES}"
        INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "Could not find SDL2")
endif()

# SDL2_image
# pkg-config would probably help here
find_library(SDL2_IMAGE_LIBRARIES 
    NAMES SDL2_image
    HINTS
        ENV SDL2_DIR
        ENV SDL2_IMAGE_DIR
    PATHS ${SDL2_IMAGE_PATH})
find_path(SDL2_IMAGE_INCLUDE_DIR 
    NAMES SDL_image.h
    HINTS
        ENV SDL2_DIR
        ENV SDL2_IMAGE_DIR
    PATHS ${SDL2_IMAGE_PATH} 
    PATH_SUFFIXES SDL2)
if(SDL2_IMAGE_LIBRARIES AND SDL2_IMAGE_INCLUDE_DIR)
    message(STATUS "Found library: SDL2_image")
    message(STATUS "    Library: ${SDL2_IMAGE_LIBRARIES}")
    message(STATUS "    Include: ${SDL2_IMAGE_INCLUDE_DIR}")
    add_library(SDL2_image UNKNOWN IMPORTED)
    set_target_properties(SDL2_image PROPERTIES 
        IMPORTED_LOCATION "${SDL2_IMAGE_LIBRARIES}"
        INTERFACE_INCLUDE_DIRECTORIES "${SDL2_IMAGE_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "Could not find SDL2_image")
endif()

### Vulkan
find_package(Vulkan REQUIRED)
### glm
find_package(glm REQUIRED)
### OpenGL
find_package(OpenGL REQUIRED)
### GLEW
find_package(GLEW REQUIRED)

#################################################################
## Core

set(CORE_TARGET "core")
add_subdirectory(core)

# Add compile options as definitions
include(cmake/define_target_option.cmake)
define_target_option(${CORE_TARGET} ${CORE_LOG_ENABLE_TAG} CORE_LOG_ENABLE_TAG)
define_target_option(${CORE_TARGET} ${CORE_UTILS_QUEUE_ENABLE_STDMUTEX} CORE_UTILS_QUEUE_ENABLE_STDMUTEX)
define_target_option(${CORE_TARGET} ${CORE_UTILS_QUEUE_ENABLE_LOCKFREE} CORE_UTILS_QUEUE_ENABLE_LOCKFREE)
define_target_option(${CORE_TARGET} 1 CORE_UTILS_QUEUE_USE_${CORE_UTILS_QUEUE_IMPL})

#################################################################
## Logic
add_subdirectory(logic)

#################################################################
## Shaders

add_custom_target(Shaders ALL
    COMMAND ${PROJECT_ROOT}/tools/scripts/compile_shaders.sh OUTDIR=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders
    DEPENDS ${CORE_TARGET})

# Package assets with binary
file(COPY assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

#################################################################
## Examples

if(CORE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

#################################################################
## References

if(CORE_BUILD_REFERENCES)
    add_subdirectory(references)
endif()

#################################################################
## Documentation

if(CORE_BUILD_DOXYGEN)
    find_package(Doxygen
        REQUIRED dot
        OPTIONAL_COMPONENTS mscgen dia)

    if(DOXYGEN_FOUND)
        message(STATUS "Documentation: ON")

        set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_DOCS_OUTPUT_DIRECTORY})
        set(DOXYGEN_GENERATE_HTML ${CORE_BUILD_DOXYGEN_HTML})
        set(DOXYGEN_GENERATE_MAN ${CORE_BUILD_DOXYGEN_MAN})
        set(DOXYGEN_EXTRACT_ALL "YES")
        set(DOXYGEN_PREDEFINED "DOXYGEN=1")
        set(DOXYGEN_STRIP_FROM_PATH "include")

        doxygen_add_docs(docs
            ${CMAKE_SOURCE_DIR}/core
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/core
            COMMENT "Generate man pages")
    elseif()
        message(STATUS "Documentation: DOXYGEN_NOT_FOUND")
    endif()
endif()
