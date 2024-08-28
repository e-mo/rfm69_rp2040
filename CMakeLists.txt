cmake_minimum_required(VERSION 3.13)

project(rfm69_rp2040 C CXX ASM)

add_library(rfm69_rp2040 
	src/rfm69_rp2040_interface.c
	src/rfm69_rp2040_rudp.c
)

target_include_directories(rfm69_rp2040 PUBLIC 
	src
)

target_link_libraries(rfm69_rp2040 PUBLIC
	pico_stdlib
	hardware_spi
	pico_rand
)
