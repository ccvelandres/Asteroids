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
# ##### easy_profiler (static)

set(easy_profiler_shared ON)
set(easy_profiler_prefix ${CMAKE_CURRENT_BINARY_DIR}/lib/easy_profiler)
ExternalProject_Add(easy_profiler_project
    PREFIX ${easy_profiler_prefix}
    DOWNLOAD_COMMAND ""
    UPDATE_COMMAND ""
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/easy_profiler
    INSTALL_DIR ${easy_profiler_prefix}/install
    CMAKE_ARGS
        -DSDLSOUND_BUILD_TEST=OFF
        -DBUILD_SHARED_LIBS=${easy_profiler_shared}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>)
ExternalProject_Get_Property(easy_profiler_project INSTALL_DIR)

if(easy_profiler_shared)
    add_library(easy_profiler SHARED IMPORTED)
    if(UNIX)
        set_target_properties(easy_profiler PROPERTIES IMPORTED_LOCATION ${INSTALL_DIR}/lib/libeasy_profiler.so)
    elseif(WIN32)
        set_target_properties(easy_profiler PROPERTIES 
            IMPORTED_LOCATION ${INSTALL_DIR}/bin/libeasy_profiler.dll
            IMPORTED_IMPLIB ${INSTALL_DIR}/lib/libeasy_profiler.dll.a)
    endif()
else()
    add_library(easy_profiler STATIC IMPORTED)
    if(UNIX)
        set_target_properties(easy_profiler PROPERTIES IMPORTED_LOCATION ${INSTALL_DIR}/lib/libeasy_profiler.a)
    elseif(WIN32)
        set_target_properties(easy_profiler PROPERTIES 
            IMPORTED_LOCATION ${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}easy_profiler${CMAKE_STATIC_LIBRARY_SUFFIX})
    endif()
endif()
add_dependencies(easy_profiler easy_profiler_project)
file(MAKE_DIRECTORY ${INSTALL_DIR}/include/easy ${INSTALL_DIR}/include)
target_compile_definitions(easy_profiler INTERFACE BUILD_WITH_EASY_PROFILER)
target_include_directories(easy_profiler INTERFACE ${INSTALL_DIR}/include)
target_link_libraries(${CORE_TARGET} PUBLIC $<$<BOOL:$<TARGET_PROPERTY:CONFIG_CORE_PROFILER_ENABLE>>:easy_profiler>)


# ######################################################
# ##### Assimp

set(assimp_shared ON)
set(assimp_prefix ${CMAKE_CURRENT_BINARY_DIR}/lib/assimp)
ExternalProject_Add(assimp_project
    PREFIX ${assimp_prefix}
    DOWNLOAD_COMMAND ""
    UPDATE_COMMAND ""
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/assimp
    INSTALL_DIR ${assimp_prefix}/install
    CMAKE_ARGS
        -DASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT=OFF
        -DASSIMP_BUILD_TESTS=OFF
        -DASSIMP_INSTALL=OFF
        -DASSIMP_INJECT_DEBUG_POSTFIX=OFF
        -DASSIMP_NO_EXPORT=OFF
        -DASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT=OFF
        -DASSIMP_BUILD_OBJ_IMPORTER=ON
        -DBUILD_SHARED_LIBS=${assimp_shared}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>)
ExternalProject_Get_Property(assimp_project INSTALL_DIR)
ExternalProject_Get_Property(assimp_project SOURCE_DIR)

if(assimp_shared)
    add_library(assimp SHARED IMPORTED)
    if(UNIX)
        set_target_properties(assimp PROPERTIES IMPORTED_LOCATION ${INSTALL_DIR}/lib/libassimp-5.so)
    elseif(WIN32)
        set_target_properties(assimp PROPERTIES 
        IMPORTED_LOCATION ${INSTALL_DIR}/bin/libassimp-5.dll
        IMPORTED_IMPLIB ${INSTALL_DIR}/lib/libassimp.dll.a)
    endif()
else()
    add_library(assimp STATIC IMPORTED)
    if(UNIX)
        set_target_properties(assimp PROPERTIES IMPORTED_LOCATION ${INSTALL_DIR}/lib/libassimp.a)
    elseif(WIN32)
        set_target_properties(assimp PROPERTIES 
            IMPORTED_LOCATION ${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}assimp${CMAKE_STATIC_LIBRARY_SUFFIX})
    endif()
endif()
add_dependencies(assimp assimp_project)
file(MAKE_DIRECTORY ${INSTALL_DIR}/include)
target_include_directories(assimp INTERFACE ${INSTALL_DIR}/include)
target_link_libraries(${CORE_TARGET} PUBLIC assimp)


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

set(spdlog_shared ON)
set(spdlog_prefix ${CMAKE_CURRENT_BINARY_DIR}/lib/spdlog)
ExternalProject_Add(spdlog_project
    PREFIX ${spdlog_prefix}
    DOWNLOAD_COMMAND ""
    UPDATE_COMMAND ""
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/spdlog
    INSTALL_DIR ${spdlog_prefix}/install
    CMAKE_ARGS
        -DSPDLOG_BUILD_BENCH=OFF
        -DSPDLOG_BUILD_EXAMPLE=OFF
        -DSPDLOG_BUILD_TESTS=OFF
        -DSPDLOG_FMT_EXTERNAL=ON
        -DBUILD_SHARED_LIBS=${spdlog_shared}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>)
ExternalProject_Get_Property(spdlog_project INSTALL_DIR)
ExternalProject_Get_Property(spdlog_project SOURCE_DIR)

if(spdlog_shared)
    add_library(spdlog SHARED IMPORTED)
    if(UNIX)
        set_target_properties(spdlog PROPERTIES IMPORTED_LOCATION ${INSTALL_DIR}/lib/libspdlog-5.so)
    elseif(WIN32)
        set_target_properties(spdlog PROPERTIES 
            IMPORTED_LOCATION ${INSTALL_DIR}/bin/libspdlog.dll
            IMPORTED_LOCATION_DEBUG ${INSTALL_DIR}/bin/libspdlogd.dll
            IMPORTED_IMPLIB ${INSTALL_DIR}/lib/libspdlog.dll.a
            IMPORTED_IMPLIB_DEBUG ${INSTALL_DIR}/lib/libspdlogd.dll.a)
    endif()
else()
    add_library(spdlog STATIC IMPORTED)
    if(UNIX)
        set_target_properties(spdlog PROPERTIES IMPORTED_LOCATION ${INSTALL_DIR}/lib/libspdlog.a)
    elseif(WIN32)
        set_target_properties(spdlog PROPERTIES 
            IMPORTED_LOCATION ${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}spdlog${CMAKE_STATIC_LIBRARY_SUFFIX}
            IMPORTED_LOCATION_DEBUG ${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}spdlogd${CMAKE_STATIC_LIBRARY_SUFFIX})
    endif()
endif()
add_dependencies(spdlog spdlog_project)
file(MAKE_DIRECTORY ${INSTALL_DIR}/include)
target_compile_definitions(spdlog INTERFACE SPDLOG_COMPILED_LIB SPDLOG_FMT_EXTERNAL)
target_include_directories(spdlog INTERFACE ${INSTALL_DIR}/include)
target_link_libraries(${CORE_TARGET} PUBLIC spdlog)

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

set(SDL2_sound_shared ON)
set(SDL2_sound_prefix ${CMAKE_CURRENT_BINARY_DIR}/lib/SDL_sound)
ExternalProject_Add(SDL2_sound_project
    PREFIX ${SDL2_sound_prefix}
    DOWNLOAD_COMMAND ""
    UPDATE_COMMAND ""
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/SDL_sound
    INSTALL_DIR ${SDL2_sound_prefix}/install
    CMAKE_ARGS
        -DSDLSOUND_BUILD_TEST=OFF
        -DBUILD_SHARED_LIBS=${SDL2_sound_shared}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>)
ExternalProject_Get_Property(SDL2_sound_project INSTALL_DIR)
ExternalProject_Get_Property(SDL2_sound_project SOURCE_DIR)

if(SDL2_sound_shared)
    add_library(SDL2_sound SHARED IMPORTED)
    if(UNIX)
        set_target_properties(SDL2_sound PROPERTIES IMPORTED_LOCATION ${INSTALL_DIR}/lib/libSDL2_sound.so)
    elseif(WIN32)
        set_target_properties(SDL2_sound PROPERTIES 
            IMPORTED_LOCATION ${INSTALL_DIR}/bin/SDL2_sound.dll
            IMPORTED_IMPLIB ${INSTALL_DIR}/lib/libSDL2_sound.dll.a)
    endif()
else()
    add_library(SDL2_sound STATIC IMPORTED)
    if(UNIX)
        set_target_properties(SDL2_sound PROPERTIES IMPORTED_LOCATION ${INSTALL_DIR}/lib/libSDL2_sound.a)
    elseif(WIN32)
        set_target_properties(SDL2_sound PROPERTIES 
            IMPORTED_LOCATION ${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}SDL2_sound${CMAKE_STATIC_LIBRARY_SUFFIX})
    endif()
endif()
add_dependencies(SDL2_sound SDL2_sound_project)
target_include_directories(SDL2_sound INTERFACE ${SOURCE_DIR}/src)
target_link_libraries(${CORE_TARGET} PUBLIC SDL2_sound)

# ######################################################
# ##### OpenAL-soft

set(openal_soft_shared ON)
set(openal_soft_prefix ${CMAKE_CURRENT_BINARY_DIR}/lib/openal-soft)
ExternalProject_Add(openal_soft_project
    PREFIX ${openal_soft_prefix}
    DOWNLOAD_COMMAND ""
    UPDATE_COMMAND ""
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/openal-soft
    INSTALL_DIR ${openal_soft_prefix}/install
    CMAKE_ARGS
        -DALSOFT_EXAMPLES=OFF
        -DALSOFT_UTILS=OFF
        -DALSOFT_INSTALL_EXAMPLES=OFF
        -DALSOFT_INSTALL_UTILS=OFF
        -DALSOFT_NO_CONFIG_UTIL=OFF
        -DLIBTYPE=$<IF:$<BOOL:${openal_soft_shared}>,SHARED,STATIC>
        -DBUILD_SHARED_LIBS=${openal_soft_shared}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>)
ExternalProject_Get_Property(openal_soft_project INSTALL_DIR)
ExternalProject_Get_Property(openal_soft_project SOURCE_DIR)

if(openal_soft_shared)
    add_library(openal_soft SHARED IMPORTED)
    if(UNIX)
        set_target_properties(openal_soft PROPERTIES IMPORTED_LOCATION ${INSTALL_DIR}/lib/libopenal_soft.so)
    elseif(WIN32)
        set_target_properties(openal_soft PROPERTIES 
            IMPORTED_LOCATION ${INSTALL_DIR}/bin/OpenAL32.dll
            IMPORTED_IMPLIB ${INSTALL_DIR}/lib/libOpenAL32.dll.a)
    endif()
else()
    add_library(openal_soft STATIC IMPORTED)
    if(UNIX)
        set_target_properties(openal_soft PROPERTIES 
            IMPORTED_LOCATION ${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}openal_soft${CMAKE_STATIC_LIBRARY_SUFFIX})
    elseif(WIN32)
        set_target_properties(openal_soft PROPERTIES 
            IMPORTED_LOCATION ${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}OpenAL32${CMAKE_STATIC_LIBRARY_SUFFIX})
    endif()
endif()
add_dependencies(openal_soft openal_soft_project)
file(MAKE_DIRECTORY ${INSTALL_DIR}/include)
target_include_directories(openal_soft INTERFACE ${INSTALL_DIR}/include)
target_link_libraries(${CORE_TARGET} PUBLIC openal_soft)

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

find_package(fmt REQUIRED)
if(fmt_FOUND)
    message(STATUS "Found library: fmt")
    message(STATUS "    Version: ${fmt_VERSION}")
else()
    message(FATAL_ERROR "fmt not found")
endif()
target_link_libraries(${CORE_TARGET} PUBLIC fmt::fmt)

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
