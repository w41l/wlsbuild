
if [ -x /usr/bin/update-desktop-database ]; then
  /usr/bin/update-desktop-database -q /usr/share/applications >/dev/null 2>&1
fi

if [ -e /var/run/dbus/dbus.pid ]; then
  /etc/rc.d/rc.messagebus reload
fi
