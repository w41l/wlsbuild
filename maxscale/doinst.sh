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

config etc/rc.d/rc.maxscale.new
config etc/logrotate.d/maxscale.new
config etc/maxscale.cnf.new

if ! cat /etc/ld.so.conf | grep -q "maxscale" >/dev/null; then
    echo "/usr/@LIB@/maxscale" >> /etc/ld.so.conf
fi
