cmake_minimum_required(VERSION 3.16)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules;${CMAKE_MODULE_PATH}")
include(ExternalProject)

SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
SET(CMAKE_INSTALL_RPATH "$\{ORIGIN\}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
set(BUILD_SHARED_LIBS ON)

set(CMAKE_C_STANDARD 11) # Requires C11 standard
set(CMAKE_CXX_STANDARD 20) # Requires C++20 standard
#################################################################
## Core Options

set(CORE_LOG_ENABLE_TAG OFF CACHE BOOL "Sets whether the tags are included in logs")

#################################################################
## Project Settings
get_filename_component(PROJECT_FOLDER_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" PROJECT_NAME ${PROJECT_FOLDER_NAME})
message("Project Name: ${PROJECT_NAME}")

project(${PROJECT_NAME} VERSION 0.1.0)

#################################################################
## SDL

set(PROJECT_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/lib")

# SDL2 can be replaced with find_package
find_library(SDL2_LIBRARY NAME SDL2 PATHS ${SDL2_DIR})
find_path(SDL2_INCLUDE_DIR SDL.h PATHS ${SDL2_DIR} PATH_SUFFIXES SDL2)
message("SDL Library: ${SDL2_LIBRARY}")
message("SDL IncludeDir: ${SDL2_INCLUDE_DIR}")

find_library(SDL2_IMAGE_LIBRARY NAME SDL2_image PATHS ${SDL2_DIR})
find_path(SDL2_IMAGE_INCLUDE_DIR SDL_image.h PATHS ${SDL2_DIR} PATH_SUFFIXES SDL2)
message("SDL_image Library: ${SDL2_IMAGE_LIBRARY}")
message("SDL_image IncludeDir: ${SDL2_IMAGE_INCLUDE_DIR}")

# Vulkan
find_package(Vulkan REQUIRED)

# libglm (OpenGL Mathematics)
find_package(glm REQUIRED)

# OpenGL
find_package(OpenGL REQUIRED)

# GLEW
find_package(GLEW REQUIRED)
message("GLEW_VERSION: ${GLEW_VERSION}")

# fmt
# add_subdirectory(${PROJECT_LIBRARY_DIR}/fmt)
# target_link_libraries(${PROJECT_NAME} PUBLIC fmt)

# spdlog
add_subdirectory(${PROJECT_LIBRARY_DIR}/spdlog)

# easy_profiler
set(EASY_OPTION_LISTEN ON CACHE BOOL "Enable automatic startListen on startup" FORCE)
add_subdirectory(${PROJECT_LIBRARY_DIR}/easy_profiler)
add_custom_target(easy_profiler_bin DEPENDS easy_profiler
    COMMAND ${CMAKE_COMMAND} -E copy_directory  ${CMAKE_BINARY_DIR}/lib/easy_profiler/bin/ ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)
#################################################################
## Core
set(CORE_TARGET "core")

file(GLOB_RECURSE SRC_C_CORE ${CMAKE_SOURCE_DIR}/core/src/*.c)
file(GLOB_RECURSE SRC_CPP_CORE ${CMAKE_SOURCE_DIR}/core/src/*.cpp)

add_library(${CORE_TARGET} ${SRC_C_CORE} ${SRC_CPP_CORE})
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
target_link_libraries(${CORE_TARGET} PUBLIC GLEW::glew)

# Add compile options as definitions
include(cmake/define_target_option.cmake)
define_target_option(${CORE_TARGET} ${CORE_LOG_ENABLE_TAG} CORE_LOG_ENABLE_TAG)

#################################################################
## Logic

file(GLOB_RECURSE SRC_C_LOGIC ${CMAKE_SOURCE_DIR}/logic/src/*.c)
file(GLOB_RECURSE SRC_CPP_LOGIC ${CMAKE_SOURCE_DIR}/logic/src/*.cpp)

add_executable(${PROJECT_NAME} ${SRC_C_LOGIC} ${SRC_CPP_LOGIC})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/logic/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/logic/include)

target_link_libraries(${PROJECT_NAME} PUBLIC ${CORE_TARGET})

# Package assets with binary
file(COPY assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})