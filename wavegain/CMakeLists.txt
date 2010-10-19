# cmake project file by Prakash Punnoor
cmake_minimum_required(VERSION 2.8)
PROJECT(Wavegain)

ADD_EXECUTABLE(wavegain
              audio.c
              dither.c
              gain_analysis.c
              getopt1.c
              getopt.c
              main.c
              misc.c
              recurse.c
              wavegain.c
)
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-lm ")

INSTALL_TARGETS(/bin wavegain)
