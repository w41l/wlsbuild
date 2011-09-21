if [ -x /usr/bin/update-desktop-database ]; then
  /usr/bin/update-desktop-database -q usr/share/applications >/dev/null 2>&1
fi

if [ -r usr/share/icons/hicolor/icon-theme.cache ]; then
  if [ -x /usr/bin/gtk-update-icon-cache ]; then
    /usr/bin/gtk-update-icon-cache -q usr/share/icons/hicolor >/dev/null 2>&1
  fi
fi

if [ -f /usr/share/autostart/wicd-tray.desktop ]; then
  mv /usr/share/autostart/wicd-tray.desktop /usr/share/autostart/wicd-tray.desktop.disabled
fi

if [ -f /etc/xdg/autostart/wicd-tray.desktop ]; then
  mv /etc/xdg/autostart/wicd-tray.desktop /etc/xdg/autostart/wicd-tray.desktop.disabled
fi

