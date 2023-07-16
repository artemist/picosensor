cmake_minimum_required(VERSION 3.12)

include(external/pico-sdk/pico_sdk_init.cmake)
include(external/freertos/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(picosensor C CXX ASM)
set(CMAKE_C_STANDARD 17)
set(PICO_BOARD pico_w)
pico_sdk_init()

add_executable(picosensor_sht30 src/main.c)
target_include_directories(picosensor_sht30 PRIVATE src)
target_link_libraries(picosensor_sht30
	pico_cyw43_arch_lwip_sys_freertos
	pico_lwip_mbedtls
	pico_mbedtls
	pico_stdlib
	hardware_i2c
	FreeRTOS-Kernel-Heap4)

pico_enable_stdio_usb(picosensor_sht30 1)
pico_enable_stdio_usb(picosensor_sht30 1)
pico_add_extra_outputs(picosensor_sht30)
