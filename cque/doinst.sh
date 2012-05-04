
if [ -d /opt/cel/bin ]; then
  echo "Running CQue setup from /opt/cel"
  ( cd /opt/cel ; /opt/cel/bin/setup -f0 )
else
  echo "Not running CQue setup because no /opt/cel found."
fi

if ps acx | grep -q cupsd >/dev/null; then
  if [ -x /etc/rc.d/rc.cups ]; then
    /etc/rc.d/rc.cups restart
  fi
fi

