cmake_minimum_required(VERSION 3.30)
project(hfx C)

set(CMAKE_C_STANDARD 23)

add_library(hfx STATIC library.c)
