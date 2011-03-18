# Install new info files
if [ -x usr/bin/install-info -a -d usr/info ]; then
  usr/bin/install-info usr/info/vcdxrip.info.gz usr/info/dir >/dev/null 2>&1
  usr/bin/install-info usr/info/vcdimager.info.gz usr/info/dir >/dev/null 2>&1
  usr/bin/install-info usr/info/vcd-info.info.gz usr/info/dir >/dev/null 2>&1
fi
