schema_install() {
  SCHEMA="$1"
  MATECONF_CONFIG_SOURCE="xml::etc/mateconf/mateconf.xml.defaults" \
  chroot . mateconftool-2 --makefile-install-rule \
    /etc/mateconf/schemas/$SCHEMA \
    1>/dev/null
}

schema_install desktop_mate_peripherals_touchpad.schemas
schema_install apps_mate_settings_daemon_xrandr.schemas
schema_install apps_mate_settings_daemon_housekeeping.schemas
schema_install desktop_mate_peripherals_smartcard.schemas
schema_install desktop_mate_keybindings.schemas
schema_install mate-settings-daemon.schemas
schema_install desktop_mate_font_rendering.schemas
schema_install apps_mate_settings_daemon_keybindings.schemas

