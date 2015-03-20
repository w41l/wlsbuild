
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

config etc/conf.d/bootmisc.new
config etc/conf.d/consolefont.new
config etc/conf.d/devfs.new
config etc/conf.d/dmesg.new
config etc/conf.d/fsck.new
config etc/conf.d/hostname.new
config etc/conf.d/hwclock.new
config etc/conf.d/keymaps.new
config etc/conf.d/killprocs.new
config etc/conf.d/localmount.new
config etc/conf.d/modules.new
config etc/conf.d/netmount.new
config etc/conf.d/network.new
config etc/conf.d/staticroute.new
config etc/conf.d/tmpfiles.new
config etc/conf.d/urandom.new
config etc/init.d/sysctl.new
config etc/init.d/udev.new
config etc/init.d/udev-mount.new
config etc/init.d/wpa_supplicant.new
config etc/init.d/swclock.new
config etc/init.d/syslogd.new
config etc/init.d/tmpfiles.dev.new
config etc/init.d/tmpfiles.setup.new
config etc/init.d/swap.new
config etc/init.d/swapfiles.new
config etc/init.d/sysfs.new
config etc/init.d/xdesktop.new
config etc/init.d/ntpd.new
config etc/init.d/openvpn.new
config etc/init.d/procfs.new
config etc/init.d/rpc.idmapd.new
config etc/init.d/savecache.new
config etc/init.d/rpc.statd.new
config etc/init.d/rpcbind.new
config etc/init.d/samba.new
config etc/init.d/serial.new
config etc/init.d/sshd.new
config etc/init.d/termencoding.new
config etc/init.d/udev.slack.new
config etc/init.d/urandom.new
config etc/init.d/NetworkManager.new
config etc/init.d/acct.new
config etc/init.d/acpid.new
config etc/init.d/alsa.new
config etc/init.d/avahi-dnsconfd.new
config etc/init.d/avahid.new
config etc/init.d/bootmisc.new
config etc/init.d/consolefont.new
config etc/init.d/consolekit.new
config etc/init.d/cron.new
config etc/init.d/cupsd.new
config etc/init.d/dbus.new
config etc/init.d/devfs.new
config etc/init.d/device-mapper.new
config etc/init.d/dhcpcd.new
config etc/init.d/dhcpd.new
config etc/init.d/dhcrelay.new
config etc/init.d/dhcrelay6.new
config etc/init.d/dmcrypt.new
config etc/init.d/dmesg.new
config etc/init.d/dmeventd.new
config etc/init.d/dnsmasq.new
config etc/init.d/fsck.new
config etc/init.d/functions.new
config etc/init.d/fuse.new
config etc/init.d/hald.new
config etc/init.d/hostname.new
config etc/init.d/hwclock.new
config etc/init.d/isapnp.new
config etc/init.d/keymaps.new
config etc/init.d/killprocs.new
config etc/init.d/klogd.new
config etc/init.d/ldconfig.new
config etc/init.d/local.new
config etc/init.d/localmount.new
config etc/init.d/loopback.new
config etc/init.d/mcelog.new
config etc/init.d/mdadm.new
config etc/init.d/mdraid.new
config etc/init.d/modules.new
config etc/init.d/mount-ro.new
config etc/init.d/mtab.new
config etc/init.d/named.new
config etc/init.d/netmount.new
config etc/init.d/network.new
config etc/init.d/nfs.new
config etc/init.d/nfsmount.new
config etc/init.d/ntp-client.new
config etc/init.d/numlock.new
config etc/init.d/polkitd.new
config etc/init.d/root.new
config etc/init.d/rpc.pipefs.new
config etc/init.d/staticroute.new
config etc/rc.conf.new
config etc/inittab.new
