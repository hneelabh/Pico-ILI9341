cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(bare_minimum_test C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(bare_minimum_test
    bare_minimum_test.c
)

target_link_libraries(bare_minimum_test
    pico_stdlib
    hardware_spi
    hardware_gpio
)

pico_add_extra_outputs(bare_minimum_test)

pico_enable_stdio_usb(bare_minimum_test 1)
pico_enable_stdio_uart(bare_minimum_test 0)
