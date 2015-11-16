if [ -r usr/share/icons/Cheser/icon-theme.cache ]; then
  if [ -x /usr/bin/gtk-update-icon-cache ]; then
    /usr/bin/gtk-update-icon-cache -q /usr/share/icons/Cheser >/dev/null 2>&1
  fi
fi
