

set(SRC_CORE_UT_UTILS
    ${CMAKE_CURRENT_LIST_DIR}/unit/utils/utQueue.cpp)

add_executable(core_ut 
    ${SRC_UT_COMMON}
    ${SRC_CORE_UT_UTILS})


include(FetchContent)
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG origin/main 
    UPDATE_DISCONNECTED ON)
FetchContent_MakeAvailable(googletest)

# find_package( GTest CONFIG REQUIRED)
target_link_libraries(core_ut PUBLIC GTest::gtest_main GTest::gmock ${CORE_TARGET})

add_test(unittest core_ut)

include(CTestCoverageCollectGCOV)
