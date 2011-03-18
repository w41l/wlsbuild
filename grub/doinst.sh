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

config etc/grub.d/30_os-prober.new
config etc/grub.d/README.new
config etc/grub.d/00_header.new
config etc/grub.d/10_linux.new
config etc/grub.d/40_custom.new

# Install new info files
if [ -x usr/bin/install-info ]; then
  usr/bin/install-info usr/info/grub.info.gz usr/info/dir >/dev/null 2>&1
fi

