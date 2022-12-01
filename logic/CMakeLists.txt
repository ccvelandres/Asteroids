include(KConfig)

message(STATUS "-------- LOGIC ---------")
kconfig_add_kconfig("${CMAKE_CURRENT_LIST_DIR}/Kconfig")

file(GLOB_RECURSE SRC_C_LOGIC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE SRC_CPP_LOGIC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(sample ${SRC_C_LOGIC} ${SRC_CPP_LOGIC})
kconfig_add_target(sample)
target_include_directories(sample PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(sample PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(sample PUBLIC ${CORE_TARGET})
target_compile_definitions(sample PUBLIC SDL_MAIN_HANDLED)

message(STATUS "------------------------")