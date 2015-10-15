if [ -x usr/bin/glib-compile-schemas -a -d usr/share/glib-2.0/schemas ]; then
    usr/bin/glib-compile-schemas usr/share/glib-2.0/schemas 1>/dev/null 2>/dev/null
fi

if [ -x /usr/bin/gtk-update-icon-cache -a -r /usr/share/icons/hicolor/icon-theme.cache ]; then
    usr/bin/gtk-update-icon-cache -t -f usr/share/icons/hicolor 1>/dev/null 2>/dev/null
fi
