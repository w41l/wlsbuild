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

config etc/mplayer/mplayer.conf.new
config etc/mplayer/codecs.conf.new
config etc/mplayer/dvb-menu.conf.new
config etc/mplayer/input.conf.new
config etc/mplayer/menu.conf.new
