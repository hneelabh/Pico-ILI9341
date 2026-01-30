cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(dc_pin_test C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(dc_pin_test
    dc_pin_test.c
)

target_link_libraries(dc_pin_test
    pico_stdlib
    hardware_spi
    hardware_gpio
)

pico_add_extra_outputs(dc_pin_test)

pico_enable_stdio_usb(dc_pin_test 1)
pico_enable_stdio_uart(dc_pin_test 0)
