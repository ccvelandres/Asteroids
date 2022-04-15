cmake_minimum_required(VERSION 3.10)

get_filename_component(PROJECT_FOLDER_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" PROJECT_FOLDER_NAME ${PROJECT_FOLDER_NAME})
project(${PROJECT_FOLDER_NAME} VERSION 0.1.0)
message("Project Name: ${PROJECT_NAME}")

find_package(SDL2 REQUIRED)
find_package(Threads REQUIRED)

set(CMAKE_C_STANDARD 11) # Requires C11 standard
set(CMAKE_CXX_STANDARD 20) # Requires C++20 standard

file(GLOB_RECURSE SRC_C_CORE ${CMAKE_SOURCE_DIR}/core/src/*.c)
file(GLOB_RECURSE SRC_CPP_CORE ${CMAKE_SOURCE_DIR}/core/src/*.cpp)

add_executable(${PROJECT_NAME} ${SRC_C_CORE} ${SRC_CPP_CORE})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/core/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/core/include)

target_link_libraries(${PROJECT_NAME} PUBLIC SDL2)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2main)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2_image)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2_gfx)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

# message("BuildType: ${CMAKE_BUILD_TYPE}")
# if (CMAKE_BUILD_TYPE MATCHES "Debug")
#     message("Building engine with easy_profiler")
#     add_subdirectory(lib/easy_profiler)
#     target_compile_definitions(${LIBRARY_NAME} PUBLIC BUILD_WITH_EASY_PROFILER)
#     target_link_libraries(${LIBRARY_NAME} PUBLIC easy_profiler)
# endif()