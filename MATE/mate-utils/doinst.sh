schema_install() {
  SCHEMA="$1"
  MATECONF_CONFIG_SOURCE="xml::etc/mateconf/mateconf.xml.defaults" \
  chroot . mateconftool-2 --makefile-install-rule \
    /etc/mateconf/schemas/$SCHEMA \
    1>/dev/null
}

schema_install mate-system-log.schemas
schema_install baobab.schemas
schema_install mate-screenshot.schemas
schema_install mate-search-tool.schemas
schema_install mate-dictionary.schemas

if [ -x /usr/bin/update-desktop-database ]; then
  /usr/bin/update-desktop-database -q usr/share/applications >/dev/null 2>&1
fi

if [ -x /usr/bin/rarian-sk-update ]; then
  /usr/bin/rarian-sk-update 1> /dev/null 2> /dev/null
fi

