
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

echo "+==============================================================+"
echo "+ Add entries to /etc/runlevels/* according to my try-n-error. +"
echo "+                                                              +"
echo "+ Please review it after install.                              +"
echo "+==============================================================+"

( cd etc/runlevels/sysinit ; rm -rf devfs )
( cd etc/runlevels/sysinit ; ln -sf /etc/init.d/devfs devfs )
( cd etc/runlevels/sysinit ; rm -rf dmesg )
( cd etc/runlevels/sysinit ; ln -sf /etc/init.d/dmesg dmesg )
( cd etc/runlevels/sysinit ; rm -rf sysfs )
( cd etc/runlevels/sysinit ; ln -sf /etc/init.d/sysfs sysfs )
( cd etc/runlevels/sysinit ; rm -rf tmpfiles.dev )
( cd etc/runlevels/sysinit ; ln -sf /etc/init.d/tmpfiles.dev tmpfiles.dev )
( cd etc/runlevels/boot ; rm -rf hostname )
( cd etc/runlevels/boot ; ln -sf /etc/init.d/hostname hostname )
( cd etc/runlevels/boot ; rm -rf loopback )
( cd etc/runlevels/boot ; ln -sf /etc/init.d/loopback loopback )
( cd etc/runlevels/boot ; rm -rf root )
( cd etc/runlevels/boot ; ln -sf /etc/init.d/root root )
( cd etc/runlevels/boot ; rm -rf sysctl )
( cd etc/runlevels/boot ; ln -sf /etc/init.d/sysctl sysctl )
( cd etc/runlevels/boot ; rm -rf keymaps )
( cd etc/runlevels/boot ; ln -sf /etc/init.d/keymaps keymaps )
( cd etc/runlevels/boot ; rm -rf modules )
( cd etc/runlevels/boot ; ln -sf /etc/init.d/modules modules )
( cd etc/runlevels/boot ; rm -rf termencoding )
( cd etc/runlevels/boot ; ln -sf /etc/init.d/termencoding termencoding )
( cd etc/runlevels/boot ; rm -rf network )
( cd etc/runlevels/boot ; ln -sf /etc/init.d/network network )
( cd etc/runlevels/boot ; rm -rf staticroute )
( cd etc/runlevels/boot ; ln -sf /etc/init.d/staticroute staticroute )
( cd etc/runlevels/boot ; rm -rf fuse )
( cd etc/runlevels/boot ; ln -sf /etc/init.d/fuse fuse )
( cd etc/runlevels/boot ; rm -rf consolefont )
( cd etc/runlevels/boot ; ln -sf /etc/init.d/consolefont consolefont )
( cd etc/runlevels/boot ; rm -rf dmesg )
( cd etc/runlevels/boot ; ln -sf /etc/init.d/dmesg dmesg )
( cd etc/runlevels/boot ; rm -rf mtab )
( cd etc/runlevels/boot ; ln -sf /etc/init.d/mtab mtab )
( cd etc/runlevels/boot ; rm -rf fsck )
( cd etc/runlevels/boot ; ln -sf /etc/init.d/fsck fsck )
( cd etc/runlevels/boot ; rm -rf devfs )
( cd etc/runlevels/boot ; ln -sf /etc/init.d/devfs devfs )
( cd etc/runlevels/boot ; rm -rf procfs )
( cd etc/runlevels/boot ; ln -sf /etc/init.d/procfs procfs )
( cd etc/runlevels/boot ; rm -rf sysfs )
( cd etc/runlevels/boot ; ln -sf /etc/init.d/sysfs sysfs )
( cd etc/runlevels/boot ; rm -rf hwclock )
( cd etc/runlevels/boot ; ln -sf /etc/init.d/hwclock hwclock )
( cd etc/runlevels/boot ; rm -rf tmpfiles.dev )
( cd etc/runlevels/boot ; ln -sf /etc/init.d/tmpfiles.dev tmpfiles.dev )
( cd etc/runlevels/boot ; rm -rf localmount )
( cd etc/runlevels/boot ; ln -sf /etc/init.d/localmount localmount )
( cd etc/runlevels/boot ; rm -rf udev )
( cd etc/runlevels/boot ; ln -sf /etc/init.d/udev udev )
( cd etc/runlevels/boot ; rm -rf urandom )
( cd etc/runlevels/boot ; ln -sf /etc/init.d/urandom urandom )
( cd etc/runlevels/boot ; rm -rf bootmisc )
( cd etc/runlevels/boot ; ln -sf /etc/init.d/bootmisc bootmisc )
( cd etc/runlevels/boot ; rm -rf tmpfiles.setup )
( cd etc/runlevels/boot ; ln -sf /etc/init.d/tmpfiles.setup tmpfiles.setup )
( cd etc/runlevels/boot ; rm -rf swap )
( cd etc/runlevels/boot ; ln -sf /etc/init.d/swap swap )
( cd etc/runlevels/boot ; rm -rf swapfiles )
( cd etc/runlevels/boot ; ln -sf /etc/init.d/swapfiles swapfiles )
( cd etc/runlevels/boot ; rm -rf device-mapper )
( cd etc/runlevels/boot ; ln -sf /etc/init.d/device-mapper device-mapper )
( cd etc/runlevels/default ; rm -rf syslogd )
( cd etc/runlevels/default ; ln -sf /etc/init.d/syslogd syslogd )
( cd etc/runlevels/default ; rm -rf local )
( cd etc/runlevels/default ; ln -sf /etc/init.d/local local )
( cd etc/runlevels/default ; rm -rf samba )
( cd etc/runlevels/default ; ln -sf /etc/init.d/samba samba )
( cd etc/runlevels/default ; rm -rf alsa )
( cd etc/runlevels/default ; ln -sf /etc/init.d/alsa alsa )
( cd etc/runlevels/default ; rm -rf klogd )
( cd etc/runlevels/default ; ln -sf /etc/init.d/klogd klogd )
( cd etc/runlevels/default ; rm -rf netmount )
( cd etc/runlevels/default ; ln -sf /etc/init.d/netmount netmount )
( cd etc/runlevels/default ; rm -rf ntpd )
( cd etc/runlevels/default ; ln -sf /etc/init.d/ntpd ntpd )
( cd etc/runlevels/default ; rm -rf xdesktop )
( cd etc/runlevels/default ; ln -sf /etc/init.d/xdesktop xdesktop )
( cd etc/runlevels/default ; rm -rf dbus )
( cd etc/runlevels/default ; ln -sf /etc/init.d/dbus dbus )
( cd etc/runlevels/default ; rm -rf NetworkManager )
( cd etc/runlevels/default ; ln -sf /etc/init.d/NetworkManager NetworkManager )
( cd etc/runlevels/default ; rm -rf ldconfig )
( cd etc/runlevels/default ; ln -sf /etc/init.d/ldconfig ldconfig )
( cd etc/runlevels/default ; rm -rf cron )
( cd etc/runlevels/default ; ln -sf /etc/init.d/cron cron )
( cd etc/runlevels/default ; rm -rf consolekit )
( cd etc/runlevels/default ; ln -sf /etc/init.d/consolekit consolekit )
( cd etc/runlevels/default ; rm -rf numlock )
( cd etc/runlevels/default ; ln -sf /etc/init.d/numlock numlock )
( cd etc/runlevels/default ; rm -rf acpid )
( cd etc/runlevels/default ; ln -sf /etc/init.d/acpid acpid )
( cd etc/runlevels/default ; rm -rf hostname )
( cd etc/runlevels/default ; ln -sf /etc/init.d/hostname hostname )
( cd etc/runlevels/default ; rm -rf cupsd )
( cd etc/runlevels/default ; ln -sf /etc/init.d/cupsd cupsd )
( cd etc/runlevels/shutdown ; rm -rf killprocs )
( cd etc/runlevels/shutdown ; ln -sf /etc/init.d/killprocs killprocs )
( cd etc/runlevels/shutdown ; rm -rf swclock )
( cd etc/runlevels/shutdown ; ln -sf /etc/init.d/swclock swclock )
( cd etc/runlevels/shutdown ; rm -rf mount-ro )
( cd etc/runlevels/shutdown ; ln -sf /etc/init.d/mount-ro mount-ro )
( cd etc/runlevels/shutdown ; rm -rf savecache )
( cd etc/runlevels/shutdown ; ln -sf /etc/init.d/savecache savecache )

sleep 5

