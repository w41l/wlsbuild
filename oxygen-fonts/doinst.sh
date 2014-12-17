if [ -x /usr/bin/fc-cache ]; then
  echo "Updating font-cache: /usr/bin/fc-cache -f -s";
  /usr/bin/fc-cache -f -s >/dev/null 2>&1
fi

