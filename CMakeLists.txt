cmake_minimum_required(VERSION 3.13)

project(dump2mem)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O0 -g3 -mcpu=cortex-a9 -mfpu=neon-vfpv3 -mfloat-abi=hard")

add_executable(dump2mem src/dump2mem.c)
install(TARGETS dump2mem RUNTIME DESTINATION bin)