#!/bin/sh

if [ ! "$XDG_CONFIG_DIRS" = "" ]; then
  XDG_CONFIG_DIRS=$XDG_CONFIG_DIRS:/etc/gnome/xdg
else
  XDG_CONFIG_DIRS=/etc/xdg:/etc/gnome/xdg
fi
export XDG_CONFIG_DIRS

