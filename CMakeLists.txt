cmake_minimum_required(VERSION 3.14)
project(SimpleArchiver C)

set(CMAKE_C_STANDARD 99)

add_executable(SimpleArchiver main.c source/zziper.c source/zziper.h)