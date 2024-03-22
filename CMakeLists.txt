cmake_minimum_required(VERSION 3.27)
project(cache_simulator C)

set(CMAKE_C_STANDARD 99)

# Add your executable
add_executable(cache_simulator main.c
        MRU/simulator.c
        MRU/simulator.h
)

# Add compile options for your executable target
target_compile_options(cache_simulator PRIVATE -pedantic -Wall -Werror)
