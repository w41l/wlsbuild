if [ -r var/run/dbus/dbus.pid -a -x etc/rc.d/rc.messagebus ]; then
  etc/rc.d/rc.messagebus reload
fi
