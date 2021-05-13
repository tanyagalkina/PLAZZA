cmake_minimum_required(VERSION 3.19)
project(plazzajetzt)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS -pthread)
link_libraries(rt)

add_executable(plazza1 src/Error.hpp
        src/Kitchen.cpp
        src/Messenger.cpp
        src/Parser.cpp
        src/Reception.cpp
        src/ThreadPool.cpp
        src/main.cpp
        )
