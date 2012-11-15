
if ps acx | grep -q "dbus-daemon"; then
  if [ -x /etc/rc.d/rc.messagebus ]; then
    /etc/rc.d/rc.messagebus reload
  fi
fi
