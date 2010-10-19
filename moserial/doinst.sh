if [ -x /usr/bin/rarian-sk-update ]; then
    /usr/bin/rarian-sk-update -o /usr/share/omf &> /dev/null
fi
if [ -x /usr/bin/update-desktop-database ]; then
    /usr/bin/update-desktop-database /usr/share/applications &> /dev/null
fi
if [ -x /usr/bin/gtk-update-icon-cache ]; then
    /usr/bin/gtk-update-icon-cache -f -q /usr/share/icons/hicolor &> /dev/null
fi
if [ -x /usr/bin/rarian-sk-update ]; then
    /usr/bin/rarian-sk-update -o /usr/share/omf &> /dev/null
fi
if [ -x /usr/bin/update-desktop-database ]; then
    /usr/bin/update-desktop-database /usr/share/applications &> /dev/null
fi
if [ -x /usr/bin/gtk-update-icon-cache ]; then
    /usr/bin/gtk-update-icon-cache -f -q /usr/share/icons/hicolor &> /dev/null
fi
