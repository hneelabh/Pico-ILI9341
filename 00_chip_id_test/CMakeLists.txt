cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(chip_id_test C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(chip_id_test
    chip_id_test.c
)

target_link_libraries(chip_id_test
    pico_stdlib
    hardware_spi
    hardware_gpio
)

pico_add_extra_outputs(chip_id_test)

pico_enable_stdio_usb(chip_id_test 1)
pico_enable_stdio_uart(chip_id_test 0)
