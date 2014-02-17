echo "backup old modules"
mv /lib/modules/@KERNEL_VERSION@/kernel/drivers/input/mouse/psmouse.ko \
   /lib/modules/@KERNEL_VERSION@/kernel/drivers/input/mouse/psmouse.ko.old

/sbin/depmod -a @KERNEL_VERSION@
