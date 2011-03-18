schema_install() {
  SCHEMA="$1"
  GCONF_CONFIG_SOURCE="xml::etc/gconf/gconf.xml.defaults" \
  chroot . gconftool-2 --makefile-install-rule \
    /etc/gconf/schemas/$SCHEMA \
    1>/dev/null
}

schema_install evince-thumbnailer-comics.schemas
schema_install evince-thumbnailer-ps.schemas
schema_install evince-thumbnailer.schemas
schema_install evince-thumbnailer-djvu.schemas

# update rarian database
if [ -x usr/bin/rarian-sk-update ]; then
  usr/bin/rarian-sk-update 1> /dev/null 2> /dev/null
fi

# update desktop entries
if [ -x usr/bin/update-desktop-database ]; then
  usr/bin/update-desktop-database 1> /dev/null 2> /dev/null
fi

# update hicolor icons
if [ -e usr/share/icons/hicolor/icon-theme.cache ]; then
    rm -f usr/share/icons/hicolor/icon-theme.cache
    usr/bin/gtk-update-icon-cache -f -q usr/share/icons/hicolor 1>/dev/null 2>/dev/null
fi

# Restart gconfd-2 if running to reload new gconf settings
if ps acx | grep -q gconfd-2 ; then
    killall -HUP gconfd-2 ;
fi

# run gsettings schemas conversion
if [ -x usr/bin/glib-compile-schemas -a -d usr/share/glib-2.0/schemas ]; then
  usr/bin/glib-compile-schemas usr/share/glib-2.0/schemas
fi
