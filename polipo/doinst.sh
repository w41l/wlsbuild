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

config etc/tor/polipo.conf.new

# Install new info files
if [ -x usr/bin/install-info -a -d usr/info ]; then
  usr/bin/install-info usr/info/polipo.info.gz usr/info/dir >/dev/null 2>&1
fi

