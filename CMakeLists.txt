cmake_minimum_required(VERSION 2.8.4)
project(NetworkSystem)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    include/event_scheduler.h
    include/main.h
    include/message_manager.h
    include/thread_pool.h
    src/event_scheduler.cpp
    src/main.cpp
    src/message_manager.cpp
    src/thread_pool.cpp
    test/threadTest.h)

add_executable(NetworkSystem ${SOURCE_FILES})