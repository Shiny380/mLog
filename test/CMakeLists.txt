include_directories (${TEST_SOURCE_DIR}/src)

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.1
)
FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

# The below is needed to allow mbed integration
add_definitions(-D BUILD_TESTING)

add_definitions(-D MLOG_LEVEL=0)
add_executable(test_mLog mLog_test.cpp ../src/mLog.cpp)
target_link_libraries(test_mLog PRIVATE Catch2::Catch2WithMain)

include(Catch)
# handles add_test()'s automatically
catch_discover_tests(test_mLog)
