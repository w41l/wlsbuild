if ps acx | grep -q "dbus-daemon" ; then
    etc/rc.d/rc.messagebus reload
fi
