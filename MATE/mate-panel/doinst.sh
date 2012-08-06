schema_install() {
  SCHEMA="$1"
  MATECONF_CONFIG_SOURCE="xml::etc/mateconf/mateconf.xml.defaults" \
  chroot . mateconftool-2 --makefile-install-rule \
    /etc/mateconf/schemas/$SCHEMA \
    1>/dev/null
}

schema_install panel-general.schemas
schema_install workspace-switcher.schemas
schema_install panel-global.schemas
schema_install fish.schemas
schema_install panel-object.schemas
schema_install window-list.schemas
schema_install clock.schemas
schema_install panel-compatibility.schemas
schema_install panel-toplevel.schemas

entry_install() {
  ENTRY="$1"
  MATECONF_CONFIG_SOURCE="xml::etc/mateconf/mateconf.xml.defaults" \
  chroot . mateconftool-2 --direct \
    --config-source="xml::etc/mateconf/mateconf.xml.defaults" \
    --load /etc/mateconf/schemas/${ENTRY} 1>/dev/null
}

entry_install panel-default-setup.entries

if [ -x /usr/bin/update-desktop-database ]; then
  /usr/bin/update-desktop-database -q /usr/share/applications >/dev/null 2>&1
fi

if [ -e usr/share/icons/hicolor/icon-theme.cache ]; then
  if [ -x /usr/bin/gtk-update-icon-cache ]; then
    /usr/bin/gtk-update-icon-cache /usr/share/icons/hicolor >/dev/null 2>&1
  fi
fi

