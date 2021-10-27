cmake_minimum_required(VERSION 3.13)

project(dump2mem)

add_executable(dump2mem src/dump2mem.c)
install(TARGETS dump2mem RUNTIME DESTINATION bin)
