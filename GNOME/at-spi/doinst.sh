schema_install() {
  SCHEMA="$1"
  GCONF_CONFIG_SOURCE="xml::etc/gconf/gconf.xml.defaults" \
  chroot . gconftool-2 --makefile-install-rule \
    /etc/gconf/schemas/$SCHEMA \
    1>/dev/null
}

schema_install at-spi.schemas

if [ -x /usr/bin/update-desktop-database ]; then
  /usr/bin/update-desktop-database 1> /dev/null 2> /dev/null
fi

if [ -x /usr/bin/update-mime-database ]; then
  /usr/bin/update-mime-database /usr/share/mime 1> /dev/null 2> /dev/null
fi

if [ -e /usr/share/icons/hicolor/icon-theme.cache ]; then
  if [ -x /usr/bin/gtk-update-icon-cache ]; then
    /usr/bin/gtk-update-icon-cache -f -q /usr/share/icons/hicolor 1>/dev/null 2>/dev/null
  fi
fi

if ps acx | grep -q gconfd-2 ; then
    killall -HUP gconfd-2 ;
fi

if [ -x /usr/bin/glib-compile-schemas -a -d /usr/share/glib-2.0/schemas ]; then
  /usr/bin/glib-compile-schemas /usr/share/glib-2.0/schemas
fi
