#!/bin/csh

if ( $?XDG_CONFIG_DIRS ) then
    setenv XDG_CONFIG_DIRS ${XDG_CONFIG_DIRS}:/etc/gnome/xdg
else
    setenv XDG_CONFIG_DIRS /etc/xdg:/etc/gnome/xdg
endif
