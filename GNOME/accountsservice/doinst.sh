
if [ -e /var/run/dbus/dbus.pid ]; then
  if [ -x /etc/rc.d/rc.messagebus ]; then
    /etc/rc.d/rc.messagebus reload
  fi
fi
