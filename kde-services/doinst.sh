
if [ -x /usr/bin/update-desktop-database ]; then
  /usr/bin/update-desktop-database -q /usr/share/applications >/dev/null 2>&1
fi

if [ -r /usr/share/icons/hicolor/icon-theme.cache ]; then
  if [ -x /usr/bin/gtk-update-icon-cache ]; then
    /usr/bin/gtk-update-icon-cache -q /usr/share/icons/hicolor >/dev/null 2>&1
  fi
fi

xdg-mime install --mode system --novendor /usr/share/mime/text/kde-services.xml
update-mime-database /usr/share/mime > /dev/null
xdg-icon-resource forceupdate --theme hicolor
xdg-desktop-menu forceupdate
