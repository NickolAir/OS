cmake_minimum_required(VERSION 3.24)
project(project3 C)

set(CMAKE_C_STANDARD 11)

add_executable(project3 FileSystems/proc_pagemap.c)