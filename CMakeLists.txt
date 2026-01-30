cmake_minimum_required(VERSION 3.13)

# Pull in SDK (must be before project)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(pico_ili9341_all C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

# Add subdirectories for each example
add_subdirectory(01_text_demo)
add_subdirectory(02_graphics_demo)
add_subdirectory(03_image_demo)
add_subdirectory(04_speedometer)

# Print build information
message(STATUS "")
message(STATUS "====================================")
message(STATUS "Pico ILI9341 Display Project")
message(STATUS "====================================")
message(STATUS "Building all examples:")
message(STATUS "  - 01_text_demo")
message(STATUS "  - 02_graphics_demo")
message(STATUS "  - 03_image_demo")
message(STATUS "  - 04_speedometer")
message(STATUS "====================================")
message(STATUS "")
