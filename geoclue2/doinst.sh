
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

config etc/geoclue/geoclue.conf.new

if [ -r /var/run/dbus/dbus.pid ]; then
    echo "Reloading D-Bus service"
    pid=$(cat /var/run/dbus/dbus.pid)
    kill -HUP $pid || killall -HUP $pid
fi
