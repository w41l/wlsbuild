schema_install() {
  SCHEMA="$1"
  GCONF_CONFIG_SOURCE="xml::etc/gconf/gconf.xml.defaults" \
  chroot . gconftool-2 --makefile-install-rule \
    /etc/gconf/schemas/$SCHEMA \
    1>/dev/null
}

schema_install compiz-annotate.schemas
schema_install compiz-blur.schemas
schema_install compiz-clone.schemas
schema_install compiz-commands.schemas
schema_install compiz-core.schemas
schema_install compiz-cube.schemas
schema_install compiz-dbus.schemas
schema_install compiz-decoration.schemas
schema_install compiz-fade.schemas
schema_install compiz-fs.schemas
schema_install compiz-gconf.schemas
schema_install compiz-glib.schemas
schema_install compiz-gnomecompat.schemas
schema_install compiz-ini.schemas
schema_install compiz-inotify.schemas
schema_install compiz-kconfig.schemas
schema_install compiz-minimize.schemas
schema_install compiz-move.schemas
schema_install compiz-obs.schemas
schema_install compiz-place.schemas
schema_install compiz-png.schemas
schema_install compiz-regex.schemas
schema_install compiz-resize.schemas
schema_install compiz-rotate.schemas
schema_install compiz-scale.schemas
schema_install compiz-screenshot.schemas
schema_install compiz-svg.schemas
schema_install compiz-switcher.schemas
schema_install compiz-video.schemas
schema_install compiz-water.schemas
schema_install compiz-wobbly.schemas
schema_install compiz-zoom.schemas
schema_install gwd.schemas

# update desktop entries
if [ -x /usr/bin/update-desktop-database ]; then
  /usr/bin/update-desktop-database 1> /dev/null 2> /dev/null
fi

# update hicolor icons
if [ -e /usr/share/icons/hicolor/icon-theme.cache ]; then
  if [ -x /usr/bin/gtk-update-icon-cache ]; then
    /usr/bin/gtk-update-icon-cache -f -q /usr/share/icons/hicolor 1>/dev/null 2>/dev/null
  fi
fi

# run gsettings schemas conversion
if [ -x /usr/bin/glib-compile-schemas ]; then
  /usr/bin/glib-compile-schemas /usr/share/glib-2.0/schemas
fi
