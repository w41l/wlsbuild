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

config etc/openrc/conf.d/NetworkManager.new
config etc/openrc/conf.d/bootmisc.new
config etc/openrc/conf.d/consolefont.new
config etc/openrc/conf.d/devfs.new
config etc/openrc/conf.d/dhcpd.new
config etc/openrc/conf.d/dmesg.new
config etc/openrc/conf.d/fsck.new
config etc/openrc/conf.d/hostname.new
config etc/openrc/conf.d/hwclock.new
config etc/openrc/conf.d/ip6tables.new
config etc/openrc/conf.d/iptables.new
config etc/openrc/conf.d/keymaps.new
config etc/openrc/conf.d/killprocs.new
config etc/openrc/conf.d/localmount.new
config etc/openrc/conf.d/modules.new
config etc/openrc/conf.d/mysqld.new
config etc/openrc/conf.d/netmount.new
config etc/openrc/conf.d/network.new
config etc/openrc/conf.d/ntp-client.new
config etc/openrc/conf.d/ntpd.new
config etc/openrc/conf.d/openvpn.new
config etc/openrc/conf.d/portmap.new
config etc/openrc/conf.d/rsyncd.new
config etc/openrc/conf.d/rsyslog.new
config etc/openrc/conf.d/samba4.new
config etc/openrc/conf.d/sshd.new
config etc/openrc/conf.d/staticroute.new
config etc/openrc/conf.d/tmpfiles.new
config etc/openrc/conf.d/urandom.new

config etc/openrc/init.d/acct.new
config etc/openrc/init.d/acpid.new
config etc/openrc/init.d/alsa.new
config etc/openrc/init.d/atd.new
config etc/openrc/init.d/avahid.new
config etc/openrc/init.d/avahi-dnsconfd.new
config etc/openrc/init.d/bootmisc.new
config etc/openrc/init.d/consolefont.new
config etc/openrc/init.d/consolekit.new
config etc/openrc/init.d/cron.new
config etc/openrc/init.d/cupsd.new
config etc/openrc/init.d/dbus.new
config etc/openrc/init.d/devfs.new
config etc/openrc/init.d/device-mapper.new
config etc/openrc/init.d/dhcpcd.new
config etc/openrc/init.d/dhcpd.new
config etc/openrc/init.d/dhcrelay6.new
config etc/openrc/init.d/dhcrelay.new
config etc/openrc/init.d/dmcrypt.new
config etc/openrc/init.d/dmesg.new
config etc/openrc/init.d/dmeventd.new
config etc/openrc/init.d/dnsmasq.new
config etc/openrc/init.d/fontcache.new
config etc/openrc/init.d/fsck.new
config etc/openrc/init.d/fuse.new
config etc/openrc/init.d/glibschemas.new
config etc/openrc/init.d/gtkmodules.new
config etc/openrc/init.d/hostname.new
config etc/openrc/init.d/httpd.new
config etc/openrc/init.d/hwclock.new
config etc/openrc/init.d/iconcache.new
config etc/openrc/init.d/inetd.new
config etc/openrc/init.d/iptables.new
config etc/openrc/init.d/isapnp.new
config etc/openrc/init.d/keymaps.new
config etc/openrc/init.d/killprocs.new
config etc/openrc/init.d/klogd.new
config etc/openrc/init.d/ldconfig.new
config etc/openrc/init.d/localmount.new
config etc/openrc/init.d/local.new
config etc/openrc/init.d/loopback.new
config etc/openrc/init.d/mcelog.new
config etc/openrc/init.d/mdadm.new
config etc/openrc/init.d/mdraid.new
config etc/openrc/init.d/mimedatabase.new
config etc/openrc/init.d/modules.new
config etc/openrc/init.d/mount-ro.new
config etc/openrc/init.d/mtab.new
config etc/openrc/init.d/mysqld.new
config etc/openrc/init.d/netmount.new
config etc/openrc/init.d/NetworkManager.new
config etc/openrc/init.d/network.new
config etc/openrc/init.d/nfs.new
config etc/openrc/init.d/nfsclient.new
config etc/openrc/init.d/nfsmount.new
config etc/openrc/init.d/nginx.new
config etc/openrc/init.d/ntp-client.new
config etc/openrc/init.d/ntpd.new
config etc/openrc/init.d/numlock.new
config etc/openrc/init.d/openvpn.new
config etc/openrc/init.d/php-fpm.new
config etc/openrc/init.d/polkitd.new
config etc/openrc/init.d/portmap.new
config etc/openrc/init.d/procfs.new
config etc/openrc/init.d/root.new
config etc/openrc/init.d/rpc.pipefs.new
config etc/openrc/init.d/rpc.rquotad.new
config etc/openrc/init.d/rpc.statd.new
config etc/openrc/init.d/rsyncd.new
config etc/openrc/init.d/rsyslog.new
config etc/openrc/init.d/samba4.new
config etc/openrc/init.d/savecache.new
config etc/openrc/init.d/sendmail.new
config etc/openrc/init.d/serial.new
config etc/openrc/init.d/snmpd.new
config etc/openrc/init.d/sshd.new
config etc/openrc/init.d/staticroute.new
config etc/openrc/init.d/swapfiles.new
config etc/openrc/init.d/swap.new
config etc/openrc/init.d/swclock.new
config etc/openrc/init.d/sysctl.new
config etc/openrc/init.d/sysfs.new
config etc/openrc/init.d/syslogd.new
config etc/openrc/init.d/termencoding.new
config etc/openrc/init.d/tmpfiles.dev.new
config etc/openrc/init.d/tmpfiles.setup.new
config etc/openrc/init.d/udev-mount.new
config etc/openrc/init.d/udev.new
config etc/openrc/init.d/urandom.new
config etc/openrc/init.d/vsftpd.new
config etc/openrc/init.d/vsftpd-checkconfig.sh.new
config etc/openrc/init.d/wpa_supplicant.new
config etc/openrc/init.d/ypbind.new

config etc/openrc/inittab.new
config etc/openrc/rc.conf.new

echo "+==============================================================+"
echo "+ Add entries to /etc/openrc/runlevels/* according to my       +"
echo "+ try-n-error. Please review it after install.                 +"
echo "+==============================================================+"
( cd etc/openrc/runlevels/sysinit ; rm -rf devfs )
( cd etc/openrc/runlevels/sysinit ; ln -sf /etc/openrc/init.d/devfs devfs )
( cd etc/openrc/runlevels/sysinit ; rm -rf device-mapper )
( cd etc/openrc/runlevels/sysinit ; ln -sf /etc/openrc/init.d/device-mapper device-mapper )
( cd etc/openrc/runlevels/sysinit ; rm -rf dmesg )
( cd etc/openrc/runlevels/sysinit ; ln -sf /etc/openrc/init.d/dmesg dmesg )
( cd etc/openrc/runlevels/sysinit ; rm -rf sysfs )
( cd etc/openrc/runlevels/sysinit ; ln -sf /etc/openrc/init.d/sysfs sysfs )
( cd etc/openrc/runlevels/sysinit ; rm -rf tmpfiles.dev )
( cd etc/openrc/runlevels/sysinit ; ln -sf /etc/openrc/init.d/tmpfiles.dev tmpfiles.dev )
( cd etc/openrc/runlevels/sysinit ; rm -rf udev )
( cd etc/openrc/runlevels/sysinit ; ln -sf /etc/openrc/init.d/udev udev )
( cd etc/openrc/runlevels/sysinit ; rm -rf udev-mount )
( cd etc/openrc/runlevels/sysinit ; ln -sf /etc/openrc/init.d/udev-mount udev-mount )
( cd etc/openrc/runlevels/sysinit ; rm -rf urandom )
( cd etc/openrc/runlevels/sysinit ; ln -sf /etc/openrc/init.d/urandom urandom )

( cd etc/openrc/runlevels/boot ; rm -rf bootmisc )
( cd etc/openrc/runlevels/boot ; ln -sf /etc/openrc/init.d/bootmisc bootmisc )
( cd etc/openrc/runlevels/boot ; rm -rf consolefont )
( cd etc/openrc/runlevels/boot ; ln -sf /etc/openrc/init.d/consolefont consolefont )
( cd etc/openrc/runlevels/boot ; rm -rf fsck )
( cd etc/openrc/runlevels/boot ; ln -sf /etc/openrc/init.d/fsck fsck )
( cd etc/openrc/runlevels/boot ; rm -rf fuse )
( cd etc/openrc/runlevels/boot ; ln -sf /etc/openrc/init.d/fuse fuse )
( cd etc/openrc/runlevels/boot ; rm -rf hostname )
( cd etc/openrc/runlevels/boot ; ln -sf /etc/openrc/init.d/hostname hostname )
( cd etc/openrc/runlevels/boot ; rm -rf hwclock )
( cd etc/openrc/runlevels/boot ; ln -sf /etc/openrc/init.d/hwclock hwclock )
( cd etc/openrc/runlevels/boot ; rm -rf keymaps )
( cd etc/openrc/runlevels/boot ; ln -sf /etc/openrc/init.d/keymaps keymaps )
( cd etc/openrc/runlevels/boot ; rm -rf localmount )
( cd etc/openrc/runlevels/boot ; ln -sf /etc/openrc/init.d/localmount localmount )
( cd etc/openrc/runlevels/boot ; rm -rf loopback )
( cd etc/openrc/runlevels/boot ; ln -sf /etc/openrc/init.d/loopback loopback )
( cd etc/openrc/runlevels/boot ; rm -rf modules )
( cd etc/openrc/runlevels/boot ; ln -sf /etc/openrc/init.d/modules modules )
( cd etc/openrc/runlevels/boot ; rm -rf mtab )
( cd etc/openrc/runlevels/boot ; ln -sf /etc/openrc/init.d/mtab mtab )
( cd etc/openrc/runlevels/boot ; rm -rf network )
( cd etc/openrc/runlevels/boot ; ln -sf /etc/openrc/init.d/network network )
( cd etc/openrc/runlevels/boot ; rm -rf procfs )
( cd etc/openrc/runlevels/boot ; ln -sf /etc/openrc/init.d/procfs procfs )
( cd etc/openrc/runlevels/boot ; rm -rf root )
( cd etc/openrc/runlevels/boot ; ln -sf /etc/openrc/init.d/root root )
( cd etc/openrc/runlevels/boot ; rm -rf staticroute )
( cd etc/openrc/runlevels/boot ; ln -sf /etc/openrc/init.d/staticroute staticroute )
( cd etc/openrc/runlevels/boot ; rm -rf swap )
( cd etc/openrc/runlevels/boot ; ln -sf /etc/openrc/init.d/swap swap )
( cd etc/openrc/runlevels/boot ; rm -rf swapfiles )
( cd etc/openrc/runlevels/boot ; ln -sf /etc/openrc/init.d/swapfiles swapfiles )
( cd etc/openrc/runlevels/boot ; rm -rf termencoding )
( cd etc/openrc/runlevels/boot ; ln -sf /etc/openrc/init.d/termencoding termencoding )
( cd etc/openrc/runlevels/boot ; rm -rf tmpfiles.setup )
( cd etc/openrc/runlevels/boot ; ln -sf /etc/openrc/init.d/tmpfiles.setup tmpfiles.setup )

( cd etc/openrc/runlevels/default ; rm -rf acpid )
( cd etc/openrc/runlevels/default ; ln -sf /etc/openrc/init.d/acpid acpid )
( cd etc/openrc/runlevels/default ; rm -rf alsa )
( cd etc/openrc/runlevels/default ; ln -sf /etc/openrc/init.d/alsa alsa )
( cd etc/openrc/runlevels/default ; rm -rf atd )
( cd etc/openrc/runlevels/default ; ln -sf /etc/openrc/init.d/atd atd )
( cd etc/openrc/runlevels/default ; rm -rf consolekit )
( cd etc/openrc/runlevels/default ; ln -sf /etc/openrc/init.d/consolekit consolekit )
( cd etc/openrc/runlevels/default ; rm -rf cron )
( cd etc/openrc/runlevels/default ; ln -sf /etc/openrc/init.d/cron cron )
( cd etc/openrc/runlevels/default ; rm -rf cupsd )
( cd etc/openrc/runlevels/default ; ln -sf /etc/openrc/init.d/cupsd cupsd )
( cd etc/openrc/runlevels/default ; rm -rf dbus )
( cd etc/openrc/runlevels/default ; ln -sf /etc/openrc/init.d/dbus dbus )
( cd etc/openrc/runlevels/default ; rm -rf fontcache )
( cd etc/openrc/runlevels/default ; ln -sf /etc/openrc/init.d/fontcache fontcache )
( cd etc/openrc/runlevels/default ; rm -rf glibschemas )
( cd etc/openrc/runlevels/default ; ln -sf /etc/openrc/init.d/glibschemas glibschemas )
( cd etc/openrc/runlevels/default ; rm -rf gtkmodules )
( cd etc/openrc/runlevels/default ; ln -sf /etc/openrc/init.d/gtkmodules gtkmodules )
( cd etc/openrc/runlevels/default ; rm -rf iconcache )
( cd etc/openrc/runlevels/default ; ln -sf /etc/openrc/init.d/iconcache iconcache )
( cd etc/openrc/runlevels/default ; rm -rf klogd )
( cd etc/openrc/runlevels/default ; ln -sf /etc/openrc/init.d/klogd klogd )
( cd etc/openrc/runlevels/default ; rm -rf ldconfig )
( cd etc/openrc/runlevels/default ; ln -sf /etc/openrc/init.d/ldconfig ldconfig )
( cd etc/openrc/runlevels/default ; rm -rf local )
( cd etc/openrc/runlevels/default ; ln -sf /etc/openrc/init.d/local local )
( cd etc/openrc/runlevels/default ; rm -rf mimedatabase )
( cd etc/openrc/runlevels/default ; ln -sf /etc/openrc/init.d/mimedatabase mimedatabase )
( cd etc/openrc/runlevels/default ; rm -rf numlock )
( cd etc/openrc/runlevels/default ; ln -sf /etc/openrc/init.d/numlock numlock )
( cd etc/openrc/runlevels/default ; rm -rf sshd )
( cd etc/openrc/runlevels/default ; ln -sf /etc/openrc/init.d/sshd sshd )
( cd etc/openrc/runlevels/default ; rm -rf syslogd )
( cd etc/openrc/runlevels/default ; ln -sf /etc/openrc/init.d/syslogd syslogd )

( cd etc/openrc/runlevels/shutdown ; rm -rf killprocs )
( cd etc/openrc/runlevels/shutdown ; ln -sf /etc/openrc/init.d/killprocs killprocs )
( cd etc/openrc/runlevels/shutdown ; rm -rf mount-ro )
( cd etc/openrc/runlevels/shutdown ; ln -sf /etc/openrc/init.d/mount-ro mount-ro )
( cd etc/openrc/runlevels/shutdown ; rm -rf savecache )
( cd etc/openrc/runlevels/shutdown ; ln -sf /etc/openrc/init.d/savecache savecache )
( cd etc/openrc/runlevels/shutdown ; rm -rf swclock )
( cd etc/openrc/runlevels/shutdown ; ln -sf /etc/openrc/init.d/swclock swclock )
