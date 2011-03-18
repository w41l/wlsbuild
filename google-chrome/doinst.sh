if [ -x usr/bin/update-desktop-database -a -d usr/share/applications ]; then
  usr/bin/update-desktop-database -q usr/share/applications >/dev/null 2>&1
fi

if [ -x usr/bin/update-mime-database -a -d usr/share/mime ]; then
  usr/bin/update-mime-database usr/share/mime >/dev/null 2>&1
fi

if [ -e usr/share/icons/hicolor/icon-theme.cache ]; then
  if [ -x usr/bin/gtk-update-icon-cache ]; then
    usr/bin/gtk-update-icon-cache usr/share/icons/hicolor >/dev/null 2>&1
  fi
fi

if [ -e usr/share/icons/gnome/icon-theme.cache ]; then
  if [ -x usr/bin/gtk-update-icon-cache ]; then
    usr/bin/gtk-update-icon-cache usr/share/icons/gnome >/dev/null 2>&1
  fi
fi
