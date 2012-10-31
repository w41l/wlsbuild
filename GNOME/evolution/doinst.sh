schema_install() {
  SCHEMA="$1"
  GCONF_CONFIG_SOURCE="xml::etc/gconf/gconf.xml.defaults" \
  chroot . gconftool-2 --makefile-install-rule \
    /etc/gconf/schemas/$SCHEMA \
    1>/dev/null
}

schema_install apps-evolution-attachment-reminder.schemas
schema_install apps-evolution-mail-notification.schemas
schema_install apps-evolution-mail-prompts-checkdefault.schemas
schema_install apps-evolution-template-placeholders.schemas
schema_install apps_evolution_addressbook.schemas
schema_install apps_evolution_calendar.schemas
schema_install apps_evolution_email_custom_header.schemas
schema_install apps_evolution_shell.schemas
schema_install bogo-junk-plugin.schemas
schema_install evolution-mail.schemas

if [ -x /usr/bin/rarian-sk-update ]; then
  /usr/bin/rarian-sk-update 1> /dev/null 2> /dev/null
fi

if [ -x /usr/bin/update-desktop-database ]; then
  /usr/bin/update-desktop-database -q /usr/share/applications >/dev/null 2>&1
fi

if [ -x /usr/bin/update-mime-database ]; then
  /usr/bin/update-mime-database /usr/share/mime >/dev/null 2>&1
fi

if [ -e /usr/share/icons/hicolor/icon-theme.cache ]; then
  if [ -x /usr/bin/gtk-update-icon-cache ]; then
    /usr/bin/gtk-update-icon-cache /usr/share/icons/hicolor >/dev/null 2>&1
  fi
fi

if ps acx | grep -q gconfd-2 ; then
        killall -HUP gconfd-2 ;
fi

