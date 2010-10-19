# update mono search index
if [ -x usr/bin/monodoc ]; then
  chroot . usr/bin/monodoc --make-index 1> /dev/null 2> /dev/null
fi
