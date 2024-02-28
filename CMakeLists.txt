cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "~/pico/pico-sdk")

#set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(rfm69_pico_lib C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()


add_library(rfm69_pico STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/src/rfm69_pico.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/rfm69_pico.h
)

target_include_directories(rfm69_pico INTERFACE
	"${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(rfm69_pico pico_stdlib
	hardware_spi
)
