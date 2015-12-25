To use openrc as init system (replacing /etc/rc.d/rc.X), run this:
$ cd /etc
$ mv inittab inittab.slackware
$ ln -s openrc/inittab inittab

Please edit/change parameter in these configuration files:

* /etc/openrc/conf.d/hostname: Set your hostname in host.domain
* /etc/openrc/conf.d/network: Set your ip, gateway (like rc.inet1.conf)

If you are using NetworkManager, remove default network init and
replace it with NetworkManager:
$ rc-update del -a network
$ rc-update add NetworkManager default

To show current init order:
$ rc-update show {sysinit,boot,default,desktop}
