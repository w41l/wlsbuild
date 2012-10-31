
if [ -d /usr/share/fonts/cantarell -a -x /usr/bin/fc-cache ]; then
    /usr/bin/fc-cache -f -s >/dev/null 2>&1
fi
