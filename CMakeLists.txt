cmake_minimum_required(VERSION 3.21)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/kconfig-cmake;${CMAKE_MODULE_PATH}")
include(Kconfig)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# SET(CMAKE_SKIP_BUILD_RPATH FALSE)
# SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
# SET(CMAKE_INSTALL_RPATH "$\{ORIGIN\}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
set(CMAKE_DOCS_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/docs")
set(BUILD_SHARED_LIBS ON)

set(CMAKE_C_STANDARD 99) # Requires C11 standard
set(CMAKE_CXX_STANDARD 17) # Requires C++20 standard

# ################################################################
# # Project Settings
get_filename_component(PROJECT_FOLDER_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" PROJECT_NAME ${PROJECT_FOLDER_NAME})
message("Project Name: ${PROJECT_NAME}")

project(${PROJECT_NAME} VERSION 0.1.0)
set(PROJECT_ROOT "${CMAKE_SOURCE_DIR}")
set(PROJECT_LIBRARY_SOURCE_DIR "${CMAKE_SOURCE_DIR}/lib")
set(PROJECT_LIBRARY_BINARY_DIR "${CMAKE_BINARY_DIR}/lib")
set(PROJECT_LIBRARY_INSTALL_DIR "${CMAKE_BINARY_DIR}/install")

if(WIN32)
    add_link_options(-static-libgcc -static-libstdc++)
endif()

# ################################################################
# # Libraries
# add_subdirectory(lib)
# include(lib/CMakeLists.txt)

# ################################################################
# # Core
set(CORE_TARGET "core")
add_subdirectory(core)

# ################################################################
# # Logic
add_subdirectory(logic)

# ################################################################
# # Shaders

list(APPEND PROJECT_ASSET_FOLDERS audio env models spritesheets textures)
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)

# Copy each asset subdirectory
foreach(assetDir ${PROJECT_ASSET_FOLDERS})
    # Copy asset directories to output path
    if(EXISTS assets/${assetDir})
        file(COPY assets/${assetDir} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)
    endif()
endforeach()

# Special handling for shader assets
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/shaders)
# TODO: needs fixing so shader target are rerun when shader source are modified
execute_process(COMMAND ${PROJECT_ROOT}/tools/scripts/compile_shaders.sh INPUTDIR=assets/shaders OUTDIR=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/shaders
    WORKING_DIRECTORY ${PROJECT_ROOT})

# ################################################################
# # Examples
if(CONFIG_CORE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# ################################################################
# # References
if(CONFIG_CORE_BUILD_REFERENCES)
    add_subdirectory(references)
endif()

# ################################################################
# # Documentation
if(CONFIG_CORE_BUILD_DOXYGEN)
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
