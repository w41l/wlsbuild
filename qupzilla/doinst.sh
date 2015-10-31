if [ -d usr/share/applications ]; then
  /usr/bin/update-desktop-database -q usr/share/applications >/dev/null 2>&1
fi

if [ -d usr/share/mime ]; then
  /usr/bin/update-mime-database usr/share/mime >/dev/null 2>&1
fi

if [ -f usr/share/icons/hicolor/icon-theme.cache ]; then
  /usr/bin/gtk-update-icon-cache usr/share/icons/hicolor >/dev/null 2>&1
fi

