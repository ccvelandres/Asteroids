
message(STATUS "-------- LOGIC ---------")

file(GLOB_RECURSE SRC_C_LOGIC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE SRC_CPP_LOGIC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SRC_C_LOGIC} ${SRC_CPP_LOGIC})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PUBLIC ${CORE_TARGET})


message(STATUS "------------------------")