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

# #####################################################
# Link Libraries

# ######################################################
# ##### easy_profiler
set(EASY_OPTION_LISTEN ON CACHE BOOL "")
set(EASY_PROFILER_NO_SAMPLES ON CACHE BOOL "")

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/lib/easy_profiler)
target_link_libraries(${CORE_TARGET} PUBLIC $<$<BOOL:$<TARGET_PROPERTY:CONFIG_CORE_PROFILER_ENABLE>>:easy_profiler>)

# ######################################################
# ##### Assimp
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE INTERNAL "")
set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "")
add_subdirectory(lib/assimp)
target_link_libraries(${CORE_TARGET} PUBLIC assimp::assimp)

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
find_package(spdlog QUIET)

if(NOT spdlog_FOUND)
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
    message(STATUS "Found spdlog: ${spdlog_VERSION}")
endif()

target_link_libraries(${CORE_TARGET} PUBLIC $<$<BOOL:$<TARGET_PROPERTY:CONFIG_CORE_LOG_ENABLE>>:spdlog>)

# ######################################################
# #### SDL2
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

# ######################################################
# ##### SDL2_image
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

# ######################################################
# ##### SDL2_sound
set(SDLSOUND_BUILD_TEST OFF CACHE INTERNAL "")

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/lib/SDL_sound)
target_link_libraries(${CORE_TARGET} PUBLIC SDL2_sound)
# For some reason, can't inherit include directories
target_include_directories(${CORE_TARGET} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/lib/SDL_sound/src)

# ######################################################
# ##### OpenAL-soft
set(ALSOFT_EXAMPLES OFF CACHE INTERNAL "")
set(ALSOFT_UTILS OFF CACHE INTERNAL "")
set(ALSOFT_INSTALL_EXAMPLES OFF CACHE INTERNAL "")
set(ALSOFT_INSTALL_UTILS OFF CACHE INTERNAL "")
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/lib/openal-soft)
target_link_libraries(${CORE_TARGET} PUBLIC $<$<BOOL:$<TARGET_PROPERTY:CONFIG_CORE_AUDIO_USE_OPENAL>>:OpenAL>)

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

target_link_libraries(${CORE_TARGET} PUBLIC glm::glm)

# ######################################################
# ##### OpenGL
find_package(OpenGL)

if(NOT OpenGL_FOUND)
    message(WARNING "OpenGL not found")
endif()

target_link_libraries(${CORE_TARGET} PUBLIC $<$<BOOL:$<TARGET_PROPERTY:CONFIG_CORE_RENDERER_OPENGL>>:OpenGL::GL>)

# ######################################################
# ##### GLEW
find_package(GLEW QUIET)

if(NOT GLEW_FOUND)
    message(STATUS "Fetching glew...")
    FetchContent_Declare(
        GLEW_project
        GIT_REPOSITORY git@github.com:nigels-com/glew.git
        GIT_TAG glew-2.2.0
        GIT_SHALLOW TRUE
        SOURCE_DIR ${PROJECT_LIBRARY_SOURCE_DIR}/glew
        UPDATE_DISCONNECTED ON
    )
    FetchContent_MakeAvailable(GLEW_project)
else()
    message(STATUS "Found GLEW: ${GLEW_VERSION}")
endif()

target_link_libraries(${CORE_TARGET} PUBLIC $<$<BOOL:$<TARGET_PROPERTY:CONFIG_CORE_RENDERER_OPENGL>>:GLEW::glew>)

# ######################################################
# ##### libfmt
find_package(fmt QUIET)

if(NOT fmt_FOUND)
    message(STATUS "Fetching fmt...")
    FetchContent_Declare(
        fmt_project
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 8.1.1
        GIT_SHALLOW TRUE
        SOURCE_DIR ${PROJECT_LIBRARY_SOURCE_DIR}/fmt
        UPDATE_DISCONNECTED ON
    )
    FetchContent_MakeAvailable(fmt_project)
else()
    message(STATUS "Found fmt: ${fmt_VERSION}")
endif()

target_link_libraries(${CORE_TARGET} PUBLIC fmt)

# #####################################################
# Post-commands

# TODO: add post command to copy shared libs (so/dll) post build
message(STATUS "------ END CORE -------")
