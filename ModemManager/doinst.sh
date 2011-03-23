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

config etc/dbus-1/system.d/org.freedesktop.ModemManager.conf.new
config etc/udev/rules.d/77-mm-longcheer-port-types.rules.new
config etc/udev/rules.d/77-mm-platform-serial-whitelist.rules.new
config etc/udev/rules.d/77-mm-zte-port-types.rules.new
config etc/udev/rules.d/77-mm-simtech-port-types.rules.new
config etc/udev/rules.d/77-mm-pcmcia-device-blacklist.rules.new
config etc/udev/rules.d/77-mm-ericsson-mbm.rules.new
config etc/udev/rules.d/77-mm-usb-device-blacklist.rules.new

if ps acx | grep -q "dbus-daemon" ; then
    etc/rc.d/rc.messagebus reload
fi
