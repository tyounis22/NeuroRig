cmake_minimum_required(VERSION 3.31)
project(NeuroRig)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG        master
)

FetchContent_MakeAvailable(raylib)

add_executable(NeuroRig
        main.cpp
        Entity.cpp
        RigidBody.cpp
        RigidBody.h
        Vehicle.cpp
        Vehicle.h
)

# Tell CMake to link Raylib to your executable
target_link_libraries(NeuroRig raylib)