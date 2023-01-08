include(Kconfig)
include(ExternalProject)
include(FetchContent)

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
######################################################
# Link Libraries

#######################################################
###### easy_profiler
set(EASY_OPTION_LISTEN ON CACHE BOOL "")
set(EASY_PROFILER_NO_SAMPLES ON CACHE BOOL "")

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/lib/easy_profiler)
target_link_libraries(${CORE_TARGET} PUBLIC $<$<BOOL:$<TARGET_PROPERTY:CONFIG_CORE_PROFILER_ENABLE>>:easy_profiler>)

#######################################################
###### Assimp

ExternalProject_Add(assimp_project
    DOWNLOAD_COMMAND ""
    UPDATE_COMMAND ""
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/assimp
    INSTALL_DIR ${CORE_LIBRARY_INSTALL_DIR}
    CMAKE_ARGS
        -DASSIMP_BUILD_TESTS=OFF
        -DASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT=OFF
        -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_INSTALL_PREFIX:PATH=${CORE_LIBRARY_INSTALL_DIR})
ExternalProject_Get_Property(assimp_project INSTALL_DIR)

find_package(assimp REQUIRED CONFIG
    PATHS ${CORE_LIBRARY_INSTALL_DIR})
if(NOT assimp_FOUND)
    message(WARNING "assimp not found")
endif()
target_link_libraries(${CORE_TARGET} PUBLIC assimp::assimp)

#######################################################
###### JSON

find_package(nlohmann_json QUIET)
if(NOT nlohmann_json_FOUND)
    message(STATUS "Fetching nlohmann_json...")
    FetchContent_Declare(
        nlohmann_json_project
        GIT_REPOSITORY      https://github.com/nlohmann/json.git
        GIT_TAG             v3.11.2
        GIT_SHALLOW         TRUE
        SOURCE_DIR          ${PROJECT_LIBRARY_SOURCE_DIR}/json
        UPDATE_DISCONNECTED ON
    )
    FetchContent_MakeAvailable(nlohmann_json_project)
else()
    message(STATUS "Found nlohmann_json: ${nlohmann_json_VERSION}")
endif()
target_link_libraries(${CORE_TARGET} PUBLIC nlohmann_json::nlohmann_json)

#######################################################
###### spdlog

find_package(spdlog QUIET)
if(NOT spdlog_FOUND)
    message(STATUS "Fetching spdlog...")
    FetchContent_Declare(
        spdlog_project
        GIT_REPOSITORY      https://github.com/gabime/spdlog.git
        GIT_TAG             v1.10.0
        SOURCE_DIR          ${PROJECT_LIBRARY_SOURCE_DIR}/spdlog
        UPDATE_DISCONNECTED ON
    )
    FetchContent_MakeAvailable(spdlog_project)
else()
    message(STATUS "Found spdlog: ${spdlog_VERSION}")
endif()
target_link_libraries(${CORE_TARGET} PUBLIC $<$<BOOL:$<TARGET_PROPERTY:CONFIG_CORE_LOG_ENABLE>>:spdlog>)

#######################################################
##### SDL2
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
    message(FATAL_ERROR "SDL2 not found")
endif()
target_link_libraries(${CORE_TARGET} PUBLIC SDL2)

#######################################################
###### SDL2_image

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
target_link_libraries(${CORE_TARGET} PUBLIC SDL2_image)

#######################################################
###### SDL2_mixer

# ExternalProject_Add(SDL2_mixer_project
#     DOWNLOAD_COMMAND ""
#     UPDATE_COMMAND ""
#     SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/SDL_mixer
#     INSTALL_DIR ${CORE_LIBRARY_INSTALL_DIR}
#     CMAKE_ARGS
#         -DSDL2MIXER_SAMPLES=OFF
#         -DSDL2MIXER_FLAC=OFF
#         -DSDL2MIXER_MOD=OFF
#         -DSDL2MIXER_MP3=OFF
#         -DSDL2MIXER_MIDI=OFF
#         -DSDL2MIXER_OPUS=OFF
#         -DSDL2MIXER_WAVE=OFF
#         -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
#         -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
#         -DCMAKE_INSTALL_PREFIX:PATH=${CORE_LIBRARY_INSTALL_DIR})
# ExternalProject_Get_Property(SDL2_mixer_project INSTALL_DIR)

# add_library(SDL2_mixer SHARED IMPORTED)
# add_dependencies(SDL2_mixer SDL2_mixer_project)
# set_target_properties(SDL2_mixer PROPERTIES IMPORTED_LOCATION ${INSTALL_DIR}/lib/libSDL2_mixer.so)
# file(MAKE_DIRECTORY ${INSTALL_DIR}/include/SDL2 ${INSTALL_DIR}/include)
# target_include_directories(SDL2_mixer INTERFACE ${INSTALL_DIR}/include/SDL2 ${INSTALL_DIR}/include)
# target_link_libraries(${CORE_TARGET} PUBLIC SDL2_mixer)

#######################################################
###### SDL2_sound

ExternalProject_Add(SDL2_sound_project
    DOWNLOAD_COMMAND ""
    UPDATE_COMMAND ""
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/SDL_sound
    INSTALL_DIR ${CORE_LIBRARY_INSTALL_DIR}
    CMAKE_ARGS
        -DSDLSOUND_BUILD_TEST=OFF
        -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_INSTALL_PREFIX:PATH=${CORE_LIBRARY_INSTALL_DIR})
ExternalProject_Get_Property(SDL2_sound_project INSTALL_DIR)

add_library(SDL2_sound SHARED IMPORTED)
add_dependencies(SDL2_sound SDL2_sound_project)
set_target_properties(SDL2_sound PROPERTIES IMPORTED_LOCATION ${INSTALL_DIR}/lib/libSDL2_sound.so)
file(MAKE_DIRECTORY ${INSTALL_DIR}/include/SDL2 ${INSTALL_DIR}/include)
target_include_directories(SDL2_sound INTERFACE ${INSTALL_DIR}/include/SDL2 ${INSTALL_DIR}/include)
target_link_libraries(${CORE_TARGET} PUBLIC SDL2_sound)

#######################################################
###### OpenAL-soft

ExternalProject_Add(OpenAL_soft_project
    DOWNLOAD_COMMAND ""
    UPDATE_COMMAND ""
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/openal-soft
    INSTALL_DIR ${CORE_LIBRARY_INSTALL_DIR}
    CMAKE_ARGS
        -DALSOFT_EXAMPLES=OFF
        -DALSOFT_UTILS=OFF
        -DALSOFT_INSTALL_EXAMPLES=OFF
        -DALSOFT_INSTALL_UTILS=OFF
        -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_INSTALL_PREFIX:PATH=${CORE_LIBRARY_INSTALL_DIR})
ExternalProject_Get_Property(OpenAL_soft_project INSTALL_DIR)

add_library(OpenAL_soft SHARED IMPORTED)
add_dependencies(OpenAL_soft OpenAL_soft_project)
set_target_properties(OpenAL_soft PROPERTIES IMPORTED_LOCATION ${INSTALL_DIR}/lib/libopenal.so)
file(MAKE_DIRECTORY ${INSTALL_DIR}/include/AL ${INSTALL_DIR}/include)
target_include_directories(OpenAL_soft INTERFACE ${INSTALL_DIR}/include/AL ${INSTALL_DIR}/include)
target_link_libraries(${CORE_TARGET} PUBLIC $<$<BOOL:$<TARGET_PROPERTY:CONFIG_CORE_AUDIO_USE_OPENAL>>:OpenAL_soft>)

#######################################################
###### Vulkan

find_package(Vulkan QUIET)
if(NOT Vulkan_FOUND)
    message(WARNING "Vulkan not found")
endif()
target_link_libraries(${CORE_TARGET} PUBLIC $<$<BOOL:$<TARGET_PROPERTY:CONFIG_CORE_RENDERER_VULKAN>>:Vulkan::Vulkan>)

#######################################################
###### glm
find_package(glm QUIET)
if(NOT glm_FOUND)
    message(STATUS "Fetching glm...")
    FetchContent_Declare(
        glm_project
        GIT_REPOSITORY      https://github.com/g-truc/glm
        GIT_TAG             0.9.9.8
        GIT_SHALLOW         TRUE
        SOURCE_DIR          ${PROJECT_LIBRARY_SOURCE_DIR}/glm
        UPDATE_DISCONNECTED ON
    )
    FetchContent_MakeAvailable(glm_project)
elseif(glm_VERSION VERSION_GREATER_EQUAL "0.9.9")
    message(STATUS "Found glm: ${glm_VERSION}")
else()
    message(FATAL_ERROR "glm not found")
endif()
target_link_libraries(${CORE_TARGET} PUBLIC glm::glm)

#######################################################
###### OpenGL
find_package(OpenGL)
if(NOT OpenGL_FOUND)
    message(WARNING "OpenGL not found")
endif()
target_link_libraries(${CORE_TARGET} PUBLIC $<$<BOOL:$<TARGET_PROPERTY:CONFIG_CORE_RENDERER_OPENGL>>:OpenGL::GL>)

#######################################################
###### GLEW
find_package(GLEW QUIET)
if(NOT GLEW_FOUND)
    message(STATUS "Fetching glew...")
    FetchContent_Declare(
        GLEW_project
        GIT_REPOSITORY      git@github.com:nigels-com/glew.git
        GIT_TAG             glew-2.2.0
        GIT_SHALLOW         TRUE
        SOURCE_DIR          ${PROJECT_LIBRARY_SOURCE_DIR}/glew
        UPDATE_DISCONNECTED ON
    )
    FetchContent_MakeAvailable(GLEW_project)
else()
    message(STATUS "Found GLEW: ${GLEW_VERSION}")
endif()
target_link_libraries(${CORE_TARGET} PUBLIC $<$<BOOL:$<TARGET_PROPERTY:CONFIG_CORE_RENDERER_OPENGL>>:GLEW::glew>)

#######################################################
###### libfmt
find_package(fmt QUIET)
if(NOT fmt_FOUND)
    message(STATUS "Fetching fmt...")
    FetchContent_Declare(
        fmt_project
        GIT_REPOSITORY      https://github.com/fmtlib/fmt.git
        GIT_TAG             8.1.1
        GIT_SHALLOW         TRUE
        SOURCE_DIR          ${PROJECT_LIBRARY_SOURCE_DIR}/fmt
        UPDATE_DISCONNECTED ON
    )
    FetchContent_MakeAvailable(fmt_project)
else()
    message(STATUS "Found fmt: ${fmt_VERSION}")
endif()
target_link_libraries(${CORE_TARGET} PUBLIC fmt)

######################################################
# Post-commands

# TODO: add post command to copy shared libs (so/dll) post build

message(STATUS "------ END CORE -------")
