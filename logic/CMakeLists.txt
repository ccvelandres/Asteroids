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

# target_link_options(sample PRIVATE
#     -static-libgcc -static-libstdc++ -lwsock32 -lws2_32
#     -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive)

if(WIN32)
    get_filename_component( _compiler_path ${CMAKE_CXX_COMPILER} PATH )
    set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS 
        ${_compiler_path}/libgcc_s_seh-1.dll
        ${_compiler_path}/libwinpthread-1.dll
        ${_compiler_path}/libstdc++-6.dll)
    add_custom_command(TARGET sample POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            "$<TARGET_RUNTIME_DLLS:sample>"
            "${_compiler_path}/libgcc_s_seh-1.dll"
            "${_compiler_path}/libwinpthread-1.dll"
            "${_compiler_path}/libstdc++-6.dll"
            $<TARGET_FILE_DIR:sample>
            COMMAND_EXPAND_LISTS)
    set_target_properties(sample PROPERTIES
        WIN32_EXECUTABLE TRUE
        LINK_FLAGS -static)
endif()

core_add_asset_directory(${CMAKE_CURRENT_LIST_DIR}/assets)
core_set_asset_directory(${CMAKE_CURRENT_BINARY_DIR}/assets)

include( InstallRequiredSystemLibraries )
install(TARGETS sample DESTINATION bin)
install(FILES $<TARGET_RUNTIME_DLLS:sample> TYPE BIN)
install(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION bin)
install(IMPORTED_RUNTIME_ARTIFACTS 
    sample
    RUNTIME_DEPENDENCY_SET sample_runtime_artifacts)
install(RUNTIME_DEPENDENCY_SET sample_runtime_artifacts DESTINATION bin)
# install(TARGETS sample
#     RUNTIME_DEPENDENCIES
#     PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
#     POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
#     DIRECTORIES $<TARGET_FILE_DIR:sample>)
include(CPack)


message(STATUS "------------------------")