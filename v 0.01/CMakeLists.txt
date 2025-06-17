cmake_minimum_required(VERSION 4.0.2)
project(pacman)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE OFF) # Enable debug output for build process

# Set the executable name
set(EXECUTABLE_NAME ${PROJECT_NAME})

# Define source and include directories explicitly
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(INC_DIR "${CMAKE_SOURCE_DIR}/inc")
#set(RES_DIR "${CMAKE_SOURCE_DIR}/res")

# Include directories for header files and for easier access - this will include all subdirectories automatically 
include_directories(${INC_DIR})

# Collect all source and header files, including those in subdirectories
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp" "${SRC_DIR}/*/**.cpp")
file(GLOB_RECURSE HEADERS "${INC_DIR}/*.hpp" "${INC_DIR}/*/**.hpp")

# Add main.cpp to the sources and settings.hpp to the headers
set(ORIGIN main.cpp) # manually added main.cpp as origin
list(APPEND SOURCES "${CMAKE_SOURCE_DIR}/${ORIGIN}")
list(REMOVE_DUPLICATES SOURCES) # Remove duplicates

# Create executable with all source files
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# SFML find packages
find_package(SFML 2.6 COMPONENTS system graphics window audio REQUIRED)
if(NOT SFML_FOUND)
    message(FATAL_ERROR "SFML not found!")
endif()

# nlohmann-json find package
find_package(nlohmann_json REQUIRED)
if(NOT nlohmann_json_FOUND)
    message(FATAL_ERROR "nlohmann_json not found!")
endif()

# Link SFML and nlohmann-json libraries to the executable
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-system sfml-graphics sfml-window sfml-audio)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} PRIVATE Shcore)

# Set the output properties
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${EXECUTABLE_NAME}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Compiler warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE 
    -Wall -Wextra -Wpedantic
    -Wno-unused-parameter) # Suppress unused parameter warnings
endif()

# Debug output
message(STATUS "Configuration Summary:")
message(STATUS "  Project name: ${PROJECT_NAME}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Source directory: ${SRC_DIR}")
message(STATUS "  Include directory: ${INC_DIR}")
message(STATUS "  SFML version: ${SFML_VERSION}")
message(STATUS "  Main file: ${ORIGIN}")
set(ONLY_SOURCE_NAME) # Create a list to hold sources without paths
foreach(SOURCE ${SOURCES})
    # Get the sources without the path
    get_filename_component(FILE_NAME_S ${SOURCE} NAME)
    # Append to the list of sources
    list(APPEND ONLY_SOURCE_NAME ${FILE_NAME_S})
endforeach()
message(STATUS "  Source files included: ${ONLY_SOURCE_NAME}")
set(ONLY_HEADER_NAME) # Create a list to hold headers without paths
foreach(HEADER ${HEADERS})
    # Get the headers without the path
    get_filename_component(FILE_NAME_H ${HEADER} NAME)
    # Append to the list of headers
    list(APPEND ONLY_HEADER_NAME ${FILE_NAME_H})
endforeach()
message(STATUS "  Header files included: ${ONLY_HEADER_NAME}")



### TERMINAL COMMANDS ###
# clear
# if (Test-Path "build") { Remove-Item "build" -Recurse -Force }    # Remove/Delete build directory
# rm -r build
# cmake -B build -S . -G "MinGW Makefiles" && cmake --build build
# cmake -B build -S . -G "MinGW Makefiles"
# cmake --build build
# ./pacman     # Change if project name does not correspond to this
# ./pacman > output.log 2>&1     # Run with logging
