#!/bin/bash

NAME="chromium"
CHROMEDIR="/usr/@LIBDIR@/chromium"
DEFAULT="/etc/$NAME/default"

if [ -r $DEFAULT ]; then
    source $DEFAULT
    $CHROMEDIR/$NAME --enable-plugins $CHROMIUM_FLAGS $1
else
    $CHROMEDIR/$NAME --enable-plugins $1
fi
