# Install new info files
if [ -x usr/bin/install-info ]; then
  usr/bin/install-info usr/info/gsl-ref.info-6.gz usr/info/dir >/dev/null 2>&1
  usr/bin/install-info usr/info/gsl-ref.info-2.gz usr/info/dir >/dev/null 2>&1
  usr/bin/install-info usr/info/gsl-ref.info-5.gz usr/info/dir >/dev/null 2>&1
  usr/bin/install-info usr/info/gsl-ref.info-4.gz usr/info/dir >/dev/null 2>&1
  usr/bin/install-info usr/info/gsl-ref.info.gz usr/info/dir >/dev/null 2>&1
  usr/bin/install-info usr/info/gsl-ref.info-3.gz usr/info/dir >/dev/null 2>&1
  usr/bin/install-info usr/info/gsl-ref.info-1.gz usr/info/dir >/dev/null 2>&1
fi

