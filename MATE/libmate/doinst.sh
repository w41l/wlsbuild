schema_install() {
  SCHEMA="$1"
  MATECONF_CONFIG_SOURCE="xml::etc/mateconf/mateconf.xml.defaults" \
  chroot . mateconftool-2 --makefile-install-rule \
    /etc/mateconf/schemas/$SCHEMA \
    1>/dev/null
}

schema_install desktop_mate_interface.schemas
schema_install desktop_mate_applications_at_mobility.schemas
schema_install desktop_mate_background.schemas
schema_install desktop_mate_applications_office.schemas
schema_install desktop_mate_accessibility_startup.schemas
schema_install desktop_mate_applications_browser.schemas
schema_install desktop_mate_accessibility_keyboard.schemas
schema_install desktop_mate_applications_window_manager.schemas
schema_install desktop_mate_file_views.schemas
schema_install desktop_mate_thumbnailers.schemas
schema_install desktop_mate_peripherals_keyboard.schemas
schema_install desktop_mate_lockdown.schemas
schema_install desktop_mate_applications_at_visual.schemas
schema_install desktop_mate_thumbnail_cache.schemas
schema_install desktop_mate_typing_break.schemas
schema_install desktop_mate_sound.schemas
schema_install desktop_mate_peripherals_mouse.schemas
schema_install desktop_mate_applications_terminal.schemas

