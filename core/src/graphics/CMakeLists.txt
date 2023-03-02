
message(STATUS "Adding Core: Graphics")
file(GLOB SRC_GRAPHICS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
target_sources(${CORE_TARGET} PRIVATE ${SRC_GRAPHICS})

add_subdirectory(renderer)