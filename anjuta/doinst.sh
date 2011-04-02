#!/bin/sh

schema_install() {
  SCHEMA="$1"
  GCONF_CONFIG_SOURCE="xml::etc/gconf/gconf.xml.defaults" \
  chroot . gconftool-2 --makefile-install-rule \
    /etc/gconf/schemas/$SCHEMA \
    1>/dev/null
}

schema_install anjuta-build-basic-autotools-plugin.schemas
schema_install anjuta-cvs-plugin.schemas
schema_install anjuta-debug-manager.schemas
schema_install anjuta-document-manager.schemas
schema_install anjuta-editor-sourceview.schemas
schema_install anjuta-language-cpp-java.schemas
schema_install anjuta-message-manager-plugin.schemas
schema_install anjuta-symbol-db.schemas
schema_install anjuta-terminal-plugin.schemas
schema_install file-manager.schemas
schema_install preferences.schemas
schema_install python-plugin-properties.schemas

if [ -x usr/bin/update-mime-database ]; then
  usr/bin/update-mime-database usr/share/mime >/dev/null 2>&1
fi

if [ -x usr/bin/update-desktop-database ]; then
  usr/bin/update-desktop-database -q usr/share/applications >/dev/null 2>&1
fi

if [ -r usr/share/icons/hicolor/icon-theme.cache ]; then
  if [ -x usr/bin/gtk-update-icon-cache ]; then
    usr/bin/gtk-update-icon-cache -q usr/share/icons/hicolor >/dev/null 2>&1
  fi
fi

# Restart gconfd-2 if running to reload new gconf settings
if ps acx | grep -q gconfd-2 ; then
    killall -HUP gconfd-2 ;
fi

# run gsettings schemas conversion
if [ -x usr/bin/glib-compile-schemas -a -d usr/share/glib-2.0/schemas ]; then
  usr/bin/glib-compile-schemas usr/share/glib-2.0/schemas
fi
