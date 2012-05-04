#!/bin/sh
# Copyright Loris Vincenzi (http://www.slacky.eu)
# All rights reserved.
#
# Heavily based on the Slackware 13.37 SlackBuild
# http://grass.itc.it/
#
# Redistribution and use of this script, with or without modification, is
# permitted provided that the following conditions are met:
#
# 1. Redistributions of this script must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
# EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Required: cfitsio, dirac, faac, ffmpeg, gdal, geos, lapack, libgsm, libva
# Required: libvpx, opencore-amr, openjpeg, orc, postgresql, proj, rtmpdump
# Required: schroedinger, speex, wxPython, x264, xvidcore
#
# Exit on most errors
set -e


# Set variables:
CWD=`pwd`
OUTPUT=${OUTPUT:-$CWD}
PKGNAME=grass
TMP=${TMP:-/tmp/txz/$PKGNAME}
PKG=$TMP/package
VERSION=${VERSION:-6.4.1}
ARCH=${ARCH:-i486}
JOBS=${JOBS:--j2}
BUILD=${BUILD:-3}
TAG=${TAG:-sl}


SOURCE="http://grass.osgeo.org/grass64/source/$PKGNAME-$VERSION.tar.gz"


# Download sorce tarball if still not present
if [ ! -e $CWD/$PKGNAME-$VERSION.tar.gz ]; then
  wget -c $SOURCE
fi

# Set compiling FLAGS
if [ "$ARCH" = "i486" ]; then
  SLKCFLAGS="-O2 -march=i486 -mtune=i686"; SLKLDFLAGS=""; LIBDIRSUFFIX=""; CHOST="i486"
elif [ "$ARCH" = "i686" ]; then
  SLKCFLAGS="-O2 -march=i686 -mtune=i686"; SLKLDFLAGS=""; LIBDIRSUFFIX=""; CHOST="i686"
elif [ "$ARCH" = "x86_64" ]; then
  SLKCFLAGS="-O2 -fPIC"; SLKLDFLAGS="-L/usr/lib64"; LIBDIRSUFFIX="64"; CHOST="x86_64"
fi


# Create building directory
rm -rf $PKG
mkdir -p $TMP $PKG $OUTPUT


# Decompress the source tarball
cd $TMP
tar xvzf $CWD/$PKGNAME-$VERSION.tar.gz

echo -e "\E[0;32m+------------------------+\E[0;0m"
echo -e "\E[0;32m| Start SlackBuild GRASS |\E[0;0m"
echo -e "\E[0;32m+------------------------+\E[0;0m"

# Adjust permissions and ownerships
cd $PKGNAME-$VERSION
chown -R root:root .
chmod -R u+w,go+r-w,a-s .


# Building GRASS package
FFINCL="/usr/include/libavcodec /usr/include/libavformat /usr/include/libswscale" \
LDFLAGS="$SLKLDFLAGS -ldl -lncurses" \
CFLAGS="$SLKCFLAGS -ldl -lncurses" \
CXXFLAGS="$SLKCFLAGS" \
./configure --prefix=/opt \
--with-freetype \
--with-cairo \
--with-odbc \
--with-cxx \
--with-geos=/usr/bingeos-config \
--with-ffmpeg \
--with-ffmpeg-includes="/usr/include/libavcodec /usr/include/libavformat /usr/include/libswscale" \
--with-nls \
--with-blas \
--with-lapack \
--with-lapack-libs=/usr/lib \
--with-readline \
--with-python \
--with-postgres \
--with-mysql \
--with-mysql-includes=/usr/include/mysql \
--with-mysql-libs=/usr/lib/mysql \
--with-sqlite \
--with-sqlite-includes=/usr/include \
--with-sqlite-libs=/usr/lib \
--with-motif \
--with-motif-includes=/usr/X11R6/include \
--with-proj-share=/usr/share/proj \
--enable-largefile \
--with-wxwidgets \
--program-prefix= \
--program-suffix= \
--build=$CHOST-slackware-linux
echo -e "\E[0;32mPress any key to build the package!\E[0;0m"
read
make $JOBS
make prefix=$PKG/opt BINDIR=$PKG/usr/bin PREFIX=$PKG/usr install


# Strip binaries and shared objects, if present
( cd $PKG
  find . | xargs file | grep "executable" | grep ELF | cut -f 1 -d : | xargs -r strip --strip-unneeded 2> /dev/null || true
  find . | xargs file | grep "shared object" | grep ELF | cut -f 1 -d : | xargs -r strip --strip-unneeded 2> /dev/null || true
  find . | xargs file | grep "current ar archive" | cut -f 1 -d : | xargs -r strip --strip-unneeded 2> /dev/null || true
)



#Add documentation files: readme, changelog and so on
mkdir -p $PKG/usr/doc/$PKGNAME-$VERSION/SlackBuild
cp -a AUTHORS CHANGES COPYING ChangeLog* GPL.TXT INSTALL README REQUIREMENTS.html SUBMITTING* TODO \
$PKG/usr/doc/$PKGNAME-$VERSION
chmod 644 $PKG/usr/doc/$PKGNAME-$VERSION/*


# Create unversioned symlink
( cd $PKG/opt ; ln -s $PKGNAME-$VERSION $PKGNAME )


# Wrapper script fixups
sed -i -e "s;$PKG;;" -e "s;-$VERSION;;" $PKG/usr/bin/grass64
( cd $PKG/usr/bin ; ln -s grass64 $PKGNAME )

# Fix fonts path for fontcap
sed -i -e "s;$PKG;;" $PKG/opt/$PKGNAME-$VERSION/etc/fontcap


find $PKG/opt/$PKGNAME-$VERSION/man -type f -exec gzip -9 {} \;
for i in $( find $PKG/opt/$PKGNAME-$VERSION/man -type l ) ; do \
  ln -s $( readlink $i ).gz $i.gz ; rm $i ; done

# Install the pkgconfig file.
install -D -m0644 grass.pc $PKG/usr/lib${LIBDIRSUFFIX}/pkgconfig/grass.pc

mkdir -p $PKG/usr/man/man1
cp -a \
  $PKG/opt/$PKGNAME-$VERSION/man/man1/grass6.1.gz \
  $PKG/usr/man/man1/grass.1.gz

# Installing desktop menu file and icons
install -D -m 0644 $CWD/$PKGNAME.desktop \
  $PKG/usr/share/applications/$PKGNAME.desktop
install -D -m 0644 gui/icons/$PKGNAME-48x48.png \
  $PKG/usr/share/pixmaps/$PKGNAME.png



# Also add the slack-desk file and this slackbuild to the package
mkdir -p $PKG/install
cat $CWD/slack-desc > $PKG/install/slack-desc
cat $CWD/slack-desc > $PKG/usr/doc/$PKGNAME-$VERSION/SlackBuild/slack-desc
cat $CWD/$PKGNAME.SlackBuild > $PKG/usr/doc/$PKGNAME-$VERSION/SlackBuild/$PKGNAME.SlackBuild




# Building some post-install execution script
	echo "if [ -x /usr/bin/update-desktop-database ]; then
  /usr/bin/update-desktop-database 1> /dev/null 2> /dev/null
fi" >> $PKG/install/doinst.sh
	echo "" >> $PKG/install/doinst.sh
	echo "if [ -x /usr/bin/gtk-update-icon-cache ]; then
  /usr/bin/gtk-update-icon-cache  -f -q /usr/share/icons/hicolor 1> /dev/null 2> /dev/null
fi" >> $PKG/install/doinst.sh
	echo "" >> $PKG/install/doinst.sh





# Find and print dependencies into slack-required file
cd $PKG
chown -R root:root $PKG
if [ -x /usr/bin/requiredbuilder ];then
ADD=$(ls /var/log/packages/ | sed -ne 's/\(wxPython\)-\([^-]\+-[^-]\+-[^-]\+\)/\1 >= \2/p') \
requiredbuilder -y -v -s  $CWD $PKG
fi
# Create txz package
/sbin/makepkg -l y -c n $OUTPUT/$PKGNAME-$VERSION-$ARCH-$BUILD$TAG.${PKGTYPE:-txz}


# Cleanup if enabled
if [ "$1" = "--cleanup" ]; then
 rm -rf $TMP
fi





