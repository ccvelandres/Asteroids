include(Kconfig)
include(${CORE_CMAKE_MODULE_PATH}/assets.cmake)

message(STATUS "-------- LOGIC ---------")
kconfig_add_kconfig("${CMAKE_CURRENT_LIST_DIR}/Kconfig")

find_package(SDL2 REQUIRED)

file(GLOB_RECURSE SRC_C_LOGIC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE SRC_CPP_LOGIC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(sample ${SRC_C_LOGIC} ${SRC_CPP_LOGIC})
kconfig_add_target(sample)
target_include_directories(sample PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(sample PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(sample PUBLIC ${CORE_TARGET})
target_compile_definitions(sample PUBLIC SDL_MAIN_HANDLED)

if(WIN32)
add_custom_command(TARGET sample POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:sample> $<TARGET_FILE_DIR:sample>
        COMMAND_EXPAND_LISTS)
endif()

core_add_asset_directory(${CMAKE_CURRENT_LIST_DIR}/assets)
core_set_asset_directory(${CMAKE_CURRENT_BINARY_DIR}/assets)

message(STATUS "------------------------")