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

config etc/avahi/avahi-daemon.conf.new
config etc/avahi/hosts.new
config etc/avahi/avahi-autoipd.action.new
config etc/avahi/avahi-dnsconfd.action.new
config etc/avahi/services/ssh.service.new
config etc/avahi/services/sftp-ssh.service.new
config etc/rc.d/rc.avahidaemon.new
config etc/rc.d/rc.avahidnsconfd.new

if [ -x /usr/bin/update-desktop-database ]; then
  /usr/bin/update-desktop-database -q /usr/share/applications >/dev/null 2>&1
fi

if [ -x /usr/bin/update-mime-database ]; then
  /usr/bin/update-mime-database /usr/share/mime >/dev/null 2>&1
fi

if [ -e /usr/share/icons/hicolor/icon-theme.cache ]; then
  if [ -x /usr/bin/gtk-update-icon-cache ]; then
    /usr/bin/gtk-update-icon-cache -f -t /usr/share/icons/hicolor >/dev/null 2>&1
  fi
fi

if ps acx | grep -q "dbus-daemon"; then
    if [ -x /etc/rc.d/rc.messagebus ]; then
	/etc/rc.d/rc.messagebus reload
    fi
fi
