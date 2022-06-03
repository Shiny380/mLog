cmake_minimum_required(VERSION 3.16)
project(mLog)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

# if block used to prevent other projects from running 
#  these tests as this will be a library 
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
    enable_testing()
    if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
        add_subdirectory(test)
    elseif(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
        # TODO:
        # add_definitions(-D BUILD_MLOG_EXAMPLE)
        # add_executable(mLogExample example.cpp)
    endif()
endif()

