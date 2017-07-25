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
config etc/openrc/conf.d/acpid.new
config etc/openrc/conf.d/agetty.new
config etc/openrc/conf.d/atd.new
config etc/openrc/conf.d/bootmisc.new
config etc/openrc/conf.d/consolefont.new
config etc/openrc/conf.d/cpufreq.new
config etc/openrc/conf.d/dcron.new
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
config etc/openrc/conf.d/motd.new
config etc/openrc/conf.d/mtab.new
config etc/openrc/conf.d/mysqld.new
config etc/openrc/conf.d/net-online.new
config etc/openrc/conf.d/netmount.new
config etc/openrc/conf.d/network.new
config etc/openrc/conf.d/ntp-client.new
config etc/openrc/conf.d/ntpd.new
config etc/openrc/conf.d/openvpn.new
config etc/openrc/conf.d/rpcbind.new
config etc/openrc/conf.d/rsyncd.new
config etc/openrc/conf.d/samba4.new
config etc/openrc/conf.d/sendmail.new
config etc/openrc/conf.d/sshd.new
config etc/openrc/conf.d/staticroute.new
config etc/openrc/conf.d/swap.new
config etc/openrc/conf.d/udev-settle.new
config etc/openrc/conf.d/udev-trigger.new
config etc/openrc/conf.d/udev.new
config etc/openrc/conf.d/urandom.new
config etc/openrc/conf.d/wpa_supplicant.new
config etc/openrc/conf.d/xdm.new

config etc/openrc/init.d/NetworkManager.new
config etc/openrc/init.d/acct.new
config etc/openrc/init.d/acpid.new
config etc/openrc/init.d/agetty.new
config etc/openrc/init.d/atd.new
config etc/openrc/init.d/avahi-dnsconfd.new
config etc/openrc/init.d/avahid.new
config etc/openrc/init.d/binfmt.new
config etc/openrc/init.d/bootmisc.new
config etc/openrc/init.d/consolefont.new
config etc/openrc/init.d/consolekit.new
config etc/openrc/init.d/cpufreq.new
config etc/openrc/init.d/cupsd.new
config etc/openrc/init.d/dbus.new
config etc/openrc/init.d/dcron.new
config etc/openrc/init.d/devfs.new
config etc/openrc/init.d/device-mapper.new
config etc/openrc/init.d/dhcpcd.new
config etc/openrc/init.d/dhcpd.new
config etc/openrc/init.d/dhcrelay.new
config etc/openrc/init.d/dhcrelay6.new
config etc/openrc/init.d/dmcrypt.new
config etc/openrc/init.d/dmesg.new
config etc/openrc/init.d/dmeventd.new
config etc/openrc/init.d/dnsmasq.new
config etc/openrc/init.d/elogind.new
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
config etc/openrc/init.d/libvirtd.new
config etc/openrc/init.d/local.new
config etc/openrc/init.d/localmount.new
config etc/openrc/init.d/loop.new
config etc/openrc/init.d/loopback.new
config etc/openrc/init.d/mcelog.new
config etc/openrc/init.d/mdadm.new
config etc/openrc/init.d/mdraid.new
config etc/openrc/init.d/mimedatabase.new
config etc/openrc/init.d/modules-load.new
config etc/openrc/init.d/modules.new
config etc/openrc/init.d/motd.new
config etc/openrc/init.d/mount-ro.new
config etc/openrc/init.d/mtab.new
config etc/openrc/init.d/mysqld.new
config etc/openrc/init.d/net-online.new
config etc/openrc/init.d/netmount.new
config etc/openrc/init.d/network.new
config etc/openrc/init.d/nfs.new
config etc/openrc/init.d/nfsclient.new
config etc/openrc/init.d/nfsmount.new
config etc/openrc/init.d/ntp-client.new
config etc/openrc/init.d/ntpd.new
config etc/openrc/init.d/numlock.new
config etc/openrc/init.d/openvpn.new
config etc/openrc/init.d/osclock.new
config etc/openrc/init.d/php-fpm.new
config etc/openrc/init.d/procfs.new
config etc/openrc/init.d/root.new
config etc/openrc/init.d/rpc.pipefs.new
config etc/openrc/init.d/rpc.rquotad.new
config etc/openrc/init.d/rpc.statd.new
config etc/openrc/init.d/rpcbind.new
config etc/openrc/init.d/rsyncd.new
config etc/openrc/init.d/runsvdir.new
config etc/openrc/init.d/s6-svscan.new
config etc/openrc/init.d/samba.new
config etc/openrc/init.d/samba4.new
config etc/openrc/init.d/savecache.new
config etc/openrc/init.d/sendmail.new
config etc/openrc/init.d/serial.new
config etc/openrc/init.d/snmpd.new
config etc/openrc/init.d/sshd.new
config etc/openrc/init.d/staticroute.new
config etc/openrc/init.d/swap.new
config etc/openrc/init.d/swapfiles.new
config etc/openrc/init.d/swclock.new
config etc/openrc/init.d/sysctl.new
config etc/openrc/init.d/sysfs.new
config etc/openrc/init.d/syslogd.new
config etc/openrc/init.d/termencoding.new
config etc/openrc/init.d/udev-settle.new
config etc/openrc/init.d/udev-trigger.new
config etc/openrc/init.d/udev.new
config etc/openrc/init.d/urandom.new
config etc/openrc/init.d/vsftpd-checkconfig.sh.new
config etc/openrc/init.d/vsftpd.new
config etc/openrc/init.d/wpa_supplicant.new
config etc/openrc/init.d/xdm.new
config etc/openrc/init.d/ypbind.new

config etc/openrc/inittab.new
config etc/openrc/rc.conf.new

sleep 2
echo " !! WARNING !! WARNING !! WARNING !!"
echo
echo " Please read /etc/openrc/README.TXT for details."
echo " Run slackpkg new-config to merge all .new init scrips."
echo " Then remove or moved out .orig backup from /etc/openrc/init.d after merging. Then reboot."
echo
sleep 2
