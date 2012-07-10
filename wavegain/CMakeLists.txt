# cmake project file by Prakash Punnoor
# Tue Jul 10 07:38:10 WIT 2012: modified by Widya Walesa <walecha99@gmail.com>

# wavegain info
# SET(WAVEGAIN_VERSION "1.3.1" )
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h )

cmake_minimum_required(VERSION 2.8)
PROJECT(wavegain)

set(CMAKE_VERBOSE_MAKEFILE false)

ADD_EXECUTABLE(wavegain
    dither.c
    gain_analysis.c
    getopt1.c
    getopt.c
    main.c
    misc.c
    recurse.c
    audio.c
    wavegain.c
)

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-lm ")

# INSTALL_TARGETS(/bin wavegain)
