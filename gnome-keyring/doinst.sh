#schema_install() {
#  SCHEMA="$1"
#  GCONF_CONFIG_SOURCE="xml::etc/gconf/gconf.xml.defaults" \
#  chroot . gconftool-2 --makefile-install-rule \
#    /etc/gconf/schemas/$SCHEMA \
#    1>/dev/null
#}

#schema_install gnome-keyring.schemas

if [ -x usr/bin/glib-compile-schemas -a -d usr/share/glib-2.0/schemas ]; then
    usr/bin/glib-compile-schemas usr/share/glib-2.0/schemas
fi
