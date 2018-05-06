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

config etc/default/dnscrypt-proxy.new
config etc/dnscrypt-proxy/dnscrypt-proxy.toml.new
config etc/rc.d/rc.dnscrypt-proxy.new

echo "To start dnscrypt-proxy automatically, add these lines to /etc/rc.d/rc.local"
echo 
echo "  if [ -x /etc/rc.d/rc.dnscrypt-proxy ]; then"
echo "      /etc/rc.d/rc.dnscrypt-proxy start"
echo "  fi"
echo
