cmake_minimum_required(VERSION 3.21)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/kconfig-cmake;${CMAKE_MODULE_PATH}")
include(Kconfig)
enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# SET(CMAKE_SKIP_BUILD_RPATH FALSE)
# SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
# SET(CMAKE_INSTALL_RPATH "$\{ORIGIN\}")

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
# set(CMAKE_DOCS_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/docs")

set(CMAKE_C_STANDARD 99) # Requires C11 standard
set(CMAKE_CXX_STANDARD 17) # Requires C++20 standard

if(UNIX)
    set(BUILD_SHARED_LIBS ON CACHE INTERNAL "")
elseif(WIN32)
    set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
    # set(CMAKE_REQUIRED_LIBRARIES -static-libgcc -static-libstdc++ -lwsock32 -lws2_32)
    # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -mconsole -Wl,-allow-multiple-definition  -Wl,-Bstatic,--whole-archive")
    # add_link_options(-static -mconsole)
    # set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CSS_STANDARD_LIBRARIES}")
    # set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")
endif()
# set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CSS_STANDARD_LIBRARIES}")
# set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")
# set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CXX_STANDARD_LIBRARIES}")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -Wl,-allow-multiple-definition")

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

# ################################################################
# # Libraries
# add_subdirectory(lib)
# include(lib/CMakeLists.txt)

# ################################################################
# # Core
add_subdirectory(core)

# ################################################################
# # Logic
add_subdirectory(logic)

# ################################################################
# # Examples
# if(CONFIG_CORE_BUILD_EXAMPLES)
#     add_subdirectory(examples)
# endif()

# ################################################################
# # References
# if(CONFIG_CORE_BUILD_REFERENCES)
#     add_subdirectory(references)
# endif()

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
