cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()

set(PICO_BOARD pico CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(freertosproject C CXX ASM)

pico_sdk_init()


# add_subdirectory(external/lwip)
# add_subdirectory(external/tinyusb)
# add_subdirectory(external/webserver  )

add_subdirectory(lib/webserver)
add_subdirectory(lib/ssd1306)
add_subdirectory(lib/lcd1602)
#add_subdirectory(lib/led)
# === FreeRTOS library ===

add_library(freertos_kernel
    external/FreeRTOS-Kernel/tasks.c
    external/FreeRTOS-Kernel/list.c
    external/FreeRTOS-Kernel/queue.c
    external/FreeRTOS-Kernel/timers.c
    external/FreeRTOS-Kernel/event_groups.c

    external/FreeRTOS-Kernel/portable/GCC/ARM_CM0/port.c
    external/FreeRTOS-Kernel/portable/GCC/ARM_CM0/portasm.c
    external/FreeRTOS-Kernel/portable/GCC/ARM_CM0/mpu_wrappers_v2_asm.c
    external/FreeRTOS-Kernel/portable/MemMang/heap_4.c
)


target_include_directories(freertos_kernel PUBLIC
    external/FreeRTOS-Kernel/include
    external/FreeRTOS-Kernel/portable/GCC/ARM_CM0
    ${CMAKE_CURRENT_LIST_DIR} # für FreeRTOSConfig.h im Hauptverzeichnis
)

add_executable(freertosproject
    freertosproject.c
)

pico_set_program_name(freertosproject "freertosproject")
pico_set_program_version(freertosproject "0.1")

pico_enable_stdio_uart(freertosproject 0)
pico_enable_stdio_usb(freertosproject 1)

target_link_libraries(freertosproject
    pico_stdio
    pico_stdlib
    hardware_spi
    hardware_i2c
    hardware_timer
    hardware_uart

    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
    pico_lwip
    pico_lwip_http
    pico_lwip_udp
    pico_lwip_dhcpd
    pico_lwip_netif
    pico_lwip_rndis

    webserver
    webserver_lib
    ssd1306_i2c_lib
    lcd_1602_i2c_lib
    
    

    

    lwipcore
    lwipallapps
    tinyusb_device

    freertos_kernel
    
)

target_include_directories(freertosproject PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(freertosproject)
