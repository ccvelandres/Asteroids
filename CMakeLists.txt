cmake_minimum_required(VERSION 3.16)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules;${CMAKE_MODULE_PATH}")
include(ExternalProject)

SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
SET(CMAKE_INSTALL_RPATH "$\{ORIGIN\}")

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

# add_executable(${PROJECT_NAME} ${SRC_C_CORE} ${SRC_CPP_CORE})
add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/core/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/core/include)

# add_custom_command(TARGET ${PROJECT_NAME} POSTBUILD
#     COMMAND ldd Asteroids)

# set_target_properties(${PROJECT_NAME} )
#################################################################
## SDL

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

# SDL2
find_file(SDL2_INCLUDE_DIR NAME SDL.h HINTS SDL2 PATHS ${SDL2_DIR})
find_library(SDL2_LIBRARY NAME SDL2 PATHS ${SDL2_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL2_LIBRARY})
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIR})

# set(SDL_STATIC OFF)
# add_subdirectory(lib/SDL)
# message("SDL_LIBRARY ${SDL_LIBRARY}")
# add_subdirectory(lib/SDL_image)
# set(SDL_SHARED OFF)
# add_subdirectory(lib/SDL)
# target_link_libraries(${PROJECT_NAME} ${SDL_LIBRARIES})

# add_custom_command(OUTPUT SDL2_IMAGE_CONFIGURE
#     COMMAND ./configure
#     WORKING_DIRECTORY lib/SDL_image
#     )

# add_custom_target(SDL2_IMAGE_BUILD
#     COMMAND make
#     DEPENDS SDL2_IMAGE_CONFIGURE
#     )

# add_custom_target(SDL_image
#     COMMAND echo $(pwd)
#     DEPENDS SDL2
#     WORKING_DIRECTORY lib/SDL_image
#     )

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
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
file(COPY config DESTINATION ${CMAKE_BINARY_DIR})