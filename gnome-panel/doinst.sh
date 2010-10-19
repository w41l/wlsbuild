schema_install() {
  SCHEMA="$1"
  GCONF_CONFIG_SOURCE="xml::etc/gconf/gconf.xml.defaults" \
  chroot . gconftool-2 --makefile-install-rule \
    /etc/gconf/schemas/$SCHEMA \
    1>/dev/null
}

schema_install gnome-session.schemas
schema_install clock.schemas
schema_install workspace-switcher.schemas
schema_install fish.schemas
schema_install panel-global.schemas
schema_install panel-compatibility.schemas
schema_install panel-general.schemas
schema_install panel-toplevel.schemas
schema_install window-list.schemas
schema_install panel-object.schemas

entry_install() {
    ENTRY="$1"
    GCONF_CONFIG_SOURCE="xml::etc/gconf/gconf.xml.defaults" \
    chroot . gconftool-2 --direct \
        --config-source="xml::etc/gconf/gconf.xml.defaults" \
        --load /etc/gconf/schemas/${ENTRY} 1>/dev/null
}

entry_install panel-default-setup.entries

config() {
  NEW="$1"
  OLD="$(dirname $NEW)/$(basename $NEW .new)"
  # If there's no config file by that name, mv it over:
  if [ ! -r $OLD ]; then
    mv $NEW $OLD
  elif [ "$(cat $OLD | md5sum)" = "$(cat $NEW | md5sum)" ]; then
    # toss the redundant copy
    rm $NEW
  fi
  # Otherwise, we leave the .new copy for the admin to consider...
}

config etc/dbus-1/system.d/org.gnome.ClockApplet.Mechanism.conf.new

if [ -x /usr/bin/update-desktop-database ]; then
  /usr/bin/update-desktop-database -q usr/share/applications >/dev/null 2>&1
fi

if [ -x /usr/bin/update-mime-database ]; then
  /usr/bin/update-mime-database usr/share/mime >/dev/null 2>&1
fi

if [ -e usr/share/icons/hicolor/icon-theme.cache ]; then
  if [ -x /usr/bin/gtk-update-icon-cache ]; then
    /usr/bin/gtk-update-icon-cache usr/share/icons/hicolor >/dev/null 2>&1
  fi
fi

