#!/bin/sh

# Install new info files
if [ -x usr/bin/install-info ]; then
  usr/bin/install-info usr/info/autogen.info-1.gz usr/info/dir >/dev/null 2>&1
  usr/bin/install-info usr/info/autogen.info-2.gz usr/info/dir >/dev/null 2>&1
  usr/bin/install-info usr/info/autogen.info.gz usr/info/dir >/dev/null 2>&1
fi

