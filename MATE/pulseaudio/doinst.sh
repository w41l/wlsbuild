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

config etc/asound.conf.new
config etc/pulse/client.conf.new
config etc/pulse/daemon.conf.new
config etc/pulse/default.pa.new
config etc/pulse/system.pa.new

if [ -h /usr/bin/esd ]; then
  echo "Skipping creating esdcompat symlink"
else
  echo "Creating esdcompat symlink"
  mv /usr/bin/esd /usr/bin/esd.pulsified
  ln -sf /usr/bin/esdcompat /usr/bin/esd
fi

if [ -x /usr/bin/glib-compile-schemas ]; then
  /usr/bin/glib-compile-schemas /usr/share/glib-2.0/schemas >/dev/null 2>&1
fi

