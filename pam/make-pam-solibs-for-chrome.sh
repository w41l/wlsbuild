#!/bin/sh
 
# Copyright 2011  Patrick J. Volkerding, Sebeka, Minnesota, USA
# All rights reserved.
#
# Redistribution and use of this script, with or without modification, is
# permitted provided that the following conditions are met:
#
# 1. Redistributions of this script must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
#  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
#  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
#  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO
#  EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
#  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
#  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
#  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
#  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# This expects to find a file pam-*.txz in the local directory that
# will contain a usable PAM shared library to satify the requirement
# for that library.  To get whatever is actually using PAM working is
# going to need more PAM structure installed, but luckily I've yet to
# encounter what needs it and everything works fine with only the
# libpam.so.0 installed.

if ! ls pam-*-*-*.txz 1> /dev/null 2> /dev/null ; then
  echo "FAIL:  no Slackware pam txz package found."
  exit 1
fi

PKGNAM=google-chrome-pam-solibs
VERSION=${VERSION:-$(echo pam-*-*-*.txz | cut -f 2 -d -)}
ARCH=${ARCH:-$(echo pam-*-*-*.txz | cut -f 3 -d -)}
BUILD=${BUILD:-$(echo pam-*-*-*.txz | cut -f 4 -d - | cut -f 1 -d .)}

CWD=$(pwd)
TMP=${TMP:-/tmp}
PKG=$TMP/package-$PKGNAM
rm -rf $PKG
mkdir -p $TMP $PKG

cd $PKG
mkdir tmp
( cd tmp
  explodepkg $CWD/pam-$VERSION-$ARCH-$BUILD.txz
  sh install/doinst.sh
)
mkdir -p $PKG/opt/google/chrome
if [ -d tmp/lib64 ]; then
  cp -a tmp/lib64/libpam.so.0* $PKG/opt/google/chrome
else
  cp -a tmp/lib/libpam.so.0* $PKG/opt/google/chrome
fi
rm -rf $PKG/tmp

mkdir -p $PKG/install
cat << EOF > $PKG/install/slack-desc
# HOW TO EDIT THIS FILE:
# The "handy ruler" below makes it easier to edit a package description.  Line
# up the first '|' above the ':' following the base package name, and the '|'
# on the right side marks the last column you can put a character in.  You must
# make exactly 11 lines for the formatting to be correct.  It's also
# customary to leave one space after the ':'.
                        |-----handy-ruler------------------------------------------------------|
google-chrome-pam-solibs: google-chrome-pam-solibs (libpam.so.0)
google-chrome-pam-solibs:
google-chrome-pam-solibs: This is a package that provides libpam.so.0 to satisfy the library
google-chrome-pam-solibs: requirement for Google Chrome when that is installed in the
google-chrome-pam-solibs: usual /opt/google/chrome directory.  It does not provide any other
google-chrome-pam-solibs: PAM features, and cannot be used to compile against or by other
google-chrome-pam-solibs: programs.  If you need real PAM for some reason (like to compile
google-chrome-pam-solibs: Chromium), please see the pam.SlackBuild in the source directory.
google-chrome-pam-solibs:
google-chrome-pam-solibs:
google-chrome-pam-solibs:
EOF

cd $PKG
/sbin/makepkg -l y -c n $TMP/$PKGNAM-$VERSION-$ARCH-$BUILD$TAG.txz

