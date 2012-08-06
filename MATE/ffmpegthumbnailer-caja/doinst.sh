schema_install() {
  SCHEMA="$1"
  MATECONF_CONFIG_SOURCE="xml::etc/mateconf/mateconf.xml.defaults" \
  chroot . mateconftool-2 --makefile-install-rule \
    /usr/share/mateconf/schemas/$SCHEMA \
    1>/dev/null
}

schema_install ffmpegthumbnailer-caja.schemas

