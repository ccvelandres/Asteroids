cmake_minimum_required(VERSION 3.16)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules;${CMAKE_MODULE_PATH}")
include(ExternalProject)

SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
SET(CMAKE_INSTALL_RPATH "$\{ORIGIN\}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
# set(BUILD_SHARED_LIBS OFF)

set(CMAKE_C_STANDARD 11) # Requires C11 standard
set(CMAKE_CXX_STANDARD 20) # Requires C++20 standard

#################################################################
## Core
get_filename_component(PROJECT_FOLDER_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" PROJECT_NAME ${PROJECT_FOLDER_NAME})
message("Project Name: ${PROJECT_NAME}")

project(${PROJECT_NAME} VERSION 0.1.0)

file(GLOB_RECURSE SRC_C_CORE ${CMAKE_SOURCE_DIR}/core/src/*.c)
file(GLOB_RECURSE SRC_CPP_CORE ${CMAKE_SOURCE_DIR}/core/src/*.cpp)

add_executable(${PROJECT_NAME} ${SRC_C_CORE} ${SRC_CPP_CORE})
# add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/core/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/core/include)

#################################################################
## SDL

set(PROJECT_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/lib")

# set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/lib/sdl2-cmake-scripts")
# set(SDL2_PATH "${CMAKE_SOURCE_DIR}/lib/SDL")
# message("CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}")
# message("SDL2_PATH ${SDL2_PATH}")

# set(SDL2_PATH "${CMAKE_SOURCE_DIR}/lib/SDL")
# ExternalProject_Add(sdl_project
# SOURCE_DIR ${SDL2_PATH}
#     BUILD_COMMAND $(MAKE)
#     INSTALL_COMMAND $(MAKE) DESTDIR=${EXTERNAL_INSTALL_DIR} install
#     CMAKE_ARGS
#         -DSDL_STATIC=OFF
#         -DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}
#     )
# set(SDL2_IMAGE_SRC "${CMAKE_SOURCE_DIR}/lib/SDL")

# SDL2 can be replaced with find_package
find_library(SDL2_LIBRARY NAME SDL2 PATHS ${SDL2_DIR})
find_path(SDL2_INCLUDE_DIR SDL.h PATHS ${SDL2_DIR} PATH_SUFFIXES SDL2)
target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL2_LIBRARY})
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIR})
message("SDL Library: ${SDL2_LIBRARY}")
message("SDL IncludeDir: ${SDL2_INCLUDE_DIR}")

find_library(SDL2_IMAGE_LIBRARY NAME SDL2_image PATHS ${SDL2_DIR})
find_path(SDL2_IMAGE_INCLUDE_DIR SDL_image.h PATHS ${SDL2_DIR} PATH_SUFFIXES SDL2)
target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL2_IMAGE_LIBRARY})
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_IMAGE_INCLUDE_DIR})
message("SDL_image Library: ${SDL2_IMAGE_LIBRARY}")
message("SDL_image IncludeDir: ${SDL2_IMAGE_INCLUDE_DIR}")

# fmt
# add_subdirectory(${PROJECT_LIBRARY_DIR}/fmt)
# target_link_libraries(${PROJECT_NAME} PUBLIC fmt)

# spdlog
add_subdirectory(${PROJECT_LIBRARY_DIR}/spdlog)
target_link_libraries(${PROJECT_NAME} PUBLIC spdlog)

# easy_profiler
ExternalProject_Add(proj_easy_profiler
    SOURCE_DIR ${PROJECT_LIBRARY_DIR}/easy_profiler
    INSTALL_COMMAND ""
    CMAKE_ARGS -DEASY_OPTION_LISTEN=ON
    )
ExternalProject_Get_Property(proj_easy_profiler BINARY_DIR)
# Copy outputs to output dir
add_custom_command(TARGET proj_easy_profiler POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory  ${BINARY_DIR}/bin/ ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

# message("BuildType: ${CMAKE_BUILD_TYPE}")
# if (CMAKE_BUILD_TYPE MATCHES "Debug")
#     message("Building engine with easy_profiler")
#     add_subdirectory(lib/easy_profiler)
#     target_compile_definitions(${LIBRARY_NAME} PUBLIC BUILD_WITH_EASY_PROFILER)
#     target_link_libraries(${LIBRARY_NAME} PUBLIC easy_profiler)
# endif()

#################################################################
## Core

## Link Libraries
# target_link_libraries(${PROJECT_NAME} PUBLIC SDL2main)
# target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_PATH}/include)

# Package assets with binary
file(COPY assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})