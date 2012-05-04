
if ps acx | grep -q cupsd >/dev/null ; then
  if [ -x /etc/rc.d/rc.cups ]; then
    /etc/rc.d/rc.cups restart
  fi
fi

