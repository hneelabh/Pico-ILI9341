cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(text_demo C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(text_demo
    main.c
    ../lib/ili9341.c
)

target_include_directories(text_demo PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../lib
)

target_link_libraries(text_demo
    pico_stdlib
    hardware_spi
)

pico_add_extra_outputs(text_demo)

# Enable USB output, disable UART output
pico_enable_stdio_usb(text_demo 1)
pico_enable_stdio_uart(text_demo 0)
