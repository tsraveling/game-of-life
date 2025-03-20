cmake_minimum_required(VERSION 3.24)
project(life)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Debug configuration
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0")

# Add vendor subdirectory, which includes the SDL3 library
add_subdirectory(vendor)

# Automatically include all source files in the src directory
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link vendor libraries to target 
target_link_libraries(${PROJECT_NAME} PRIVATE vendor)

# Copy SDL3 libraries to the build directory (if needed)
if(APPLE)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        $<TARGET_FILE_DIR:SDL3::SDL3>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()
