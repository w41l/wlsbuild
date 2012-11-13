if ps acx | grep -q dbus ; then
  /etc/rc.d/rc.messagebus reload ;
fi
