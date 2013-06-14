#!/bin/bash

NAME="chromium"
CHROMEDIR="/usr/@LIBDIR@/chromium"
DEFAULT="/etc/$NAME/default"

if [ -r $DEFAULT ]; then
    source $DEFAULT
    $CHROMEDIR/$NAME $CHROMIUM_FLAGS $1
else
    $CHROMEDIR/$NAME $1
fi
