if ps acx | grep -q "udev"; then
  if [ -x /etc/rc.d/rc.udev ]; then
    /etc/rc.d/rc.udev reload
  fi
fi
