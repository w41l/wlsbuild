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

config etc/dbus-1/system.d/NetworkManager.conf.new
config etc/rc.d/rc.networkmanager.new
config etc/dbus-1/system.d/nm-dhcp-client.conf.new
config etc/dbus-1/system.d/nm-dispatcher.conf.new
config etc/dbus-1/system.d/nm-avahi-autoipd.conf.new
config etc/NetworkManager/nm-system-settings.conf.new
