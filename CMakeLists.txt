cmake_minimum_required(VERSION 3.24)
include(${CMAKE_SOURCE_DIR}/cmake/toolchain-arm.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/target-mcu.cmake)

# project settings
project(examples C ASM)
set(CMAKE_C_STANDARD 11)

add_library(libstefi
        src/gpio.c
        src/peripheral.c
        src/util.c
        src/uart.c
        src/startup.c
        src/syscalls.c
        src/system.c
        src/systick.c
        src/interrupts.c
        src/timer.c
        src/i2c.c
        src/rng.c
        src/ringbuffer.c
        src/spi.c
        src/adc.c
)

target_include_directories(libstefi PUBLIC include)

# Private implementation headers
target_include_directories(libstefi
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/internal
)

# Function to automatically add all subdirectories under a given directory
function(add_subdirectories base_dir)
    file(GLOB subdirectories LIST_DIRECTORIES true ${base_dir}/*)
    foreach(subdir ${subdirectories})
        if(IS_DIRECTORY ${subdir})
            add_subdirectory(${subdir})
        endif()
    endforeach()
endfunction()

# Automatically add all example and test projects
add_subdirectories(${CMAKE_SOURCE_DIR}/examples)
add_subdirectories(${CMAKE_SOURCE_DIR}/tests)

# Add subdirectory for the lib
add_subdirectory(components/simple_io)
add_subdirectory(components/display_oled)
add_subdirectory(boards)

#target_link_libraries(libstefi PUBLIC DisplayOLED)
target_link_libraries(libstefi PUBLIC SimpleIO)