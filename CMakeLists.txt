cmake_minimum_required(VERSION 3.10)
project(Flappy)

# Use C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directory
include_directories(include)

# Gather all .cpp files
file(GLOB SOURCES "src/*.cpp")

# Create the executable
add_executable(flappy ${SOURCES})

# Link SFML
if (WIN32)
    set(SFML_STATIC_LIBRARIES TRUE)  # Optional: if you're linking statically
    find_package(SFML 3.0 COMPONENTS system window graphics audio REQUIRED)
else()
    find_package(SFML 3.0 COMPONENTS system window graphics audio REQUIRED)
endif()

target_link_libraries(flappy sfml-system sfml-window sfml-graphics sfml-audio)

