# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "~/pico/pico-sdk")

#set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "2.0.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 2.0.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# step back one level on includes and sources because we are hiding CMakeLists file

set(target "rudp_tx_example")
project(${target} C CXX ASM)

set(RFM69_LIB_PATH "../../..")
add_subdirectory(${RFM69_LIB_PATH} libs/rfm69_rp2040)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(${target} src/rudp_tx_example.c)

pico_enable_stdio_uart(${target} 0)
pico_enable_stdio_usb(${target} 1)

# Add the standard library to the build
target_link_libraries(${target} pico_stdlib)
target_link_libraries(${target} rfm69_rp2040)


# Add the standard include files to the build
target_include_directories(${target} PRIVATE src)

target_compile_definitions(${target} PRIVATE RFM69_HIGH_POWER)

pico_add_extra_outputs(${target})
