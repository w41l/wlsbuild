schema_install() {
  SCHEMA="$1"
  MATECONF_CONFIG_SOURCE="xml::etc/mateconf/mateconf.xml.defaults" \
  chroot . mateconftool-2 --makefile-install-rule \
    /etc/mateconf/schemas/$SCHEMA \
    1>/dev/null
}

schema_install system_smb.schemas
schema_install desktop_default_applications.schemas
schema_install system_http_proxy.schemas
schema_install desktop_mate_url_handlers.schemas
schema_install system_dns_sd.schemas

