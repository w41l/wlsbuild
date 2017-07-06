
if [ -d usr/share/applications ]; then
  /usr/bin/update-desktop-database -q /usr/share/applications >/dev/null 2>&1
fi

if [ -r usr/share/icons/hicolor/icon-theme.cache ]; then
  /usr/bin/gtk-update-icon-cache -q /usr/share/icons/hicolor >/dev/null 2>&1
fi
