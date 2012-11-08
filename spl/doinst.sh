#!/bin/sh

chroot . /sbin/depmod -a @KERNVER@
