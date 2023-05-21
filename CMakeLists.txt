cmake_minimum_required(VERSION 3.23)
project(AudioMagic)

set(CMAKE_CXX_STANDARD 20)

add_executable(AudioMagic main.cpp wave_gen.cpp equalizer.cpp)
