#!/bin/sh
# AQEMU 0.7.3 Installation Script
# Lisence: GPL v2
# Author: Andrey Rijov (aka RDron)
# Author: Denis Smirnov (aka reMiND)
# Author: Michael Brandstetter

# Print Help
if [ "$1" = "--help" ]; then
	echo "For change install directory run: ./install_script.sh path_to_you_dir"
	exit 0
fi

# User Set Other Installation Dir?
if [ -n "$1" ]
then
	DESTDIR=$1
	echo "Install data path changed to: $1"
fi

# Default Installation Dirs For Linux
PREFIX=/usr

# Set Installation Dirs For FreeBSD
if uname -a | grep "FreeBSD" > /dev/null;
then
	PREFIX=/usr/local
fi

BINDIR=$DESTDIR/$PREFIX/bin
DATADIR=$DESTDIR/$PREFIX/share

# Unistall Previous AQEMU Version
echo "Find and remove a previous AQEMU installation..."

FIND_PREV="no"

if [ -x /usr/share/aqemu ]
then
	FIND_PREV="yes"
	echo "Remove /usr/share/aqemu directory..."
	rm -rf /usr/share/aqemu
fi

if [ -x /usr/share/apps/aqemu ]
then
	FIND_PREV="yes"
	echo "Remove /usr/share/apps/aqemu directory..."
	rm -rf /usr/share/apps/aqemu
fi

if [ -x /usr/local/share/aqemu ]
then
	FIND_PREV="yes"
	echo "Remove /usr/local/share/aqemu directory..."
	rm -rf /usr/local/share/aqemu
fi

if test "$FIND_PREV" = "no"; then
	echo "No previous AQEMU versions found."
fi

# Copy New Data Files
echo "Installing new AQEMU files..."
install -Dm 755 ./AQEMU $BINDIR/aqemu
install -d $DATADIR/aqemu/
install -d $DATADIR/aqemu/os_icons
install -d $DATADIR/aqemu/os_templates
install -Dm 644 ./Russian.qm $DATADIR/aqemu/
install -Dm 644 ./os_icons/* $DATADIR/aqemu/os_icons/
install -Dm 644 ./os_templates/* $DATADIR/aqemu/os_templates/
install -Dm 644 ./aqemu_links.html $DATADIR/aqemu/

echo "Install documentation..."
install -d $DATADIR/doc/aqemu/
for doc_file in AUTHORS CHANGELOG README TODO
do
	bzip2 -9 -c $doc_file > "$DATADIR/doc/aqemu/$doc_file.bz2"
done

echo "Create menu items..."
install -d $DATADIR/pixmaps/
install -Dm 644 ./menu_data/*.png $DATADIR/pixmaps/
install -Dm 644 ./menu_data/aqemu.desktop $DATADIR/applications/aqemu.desktop

install -Dm 644 ./menu_data/aqemu $DATADIR/menu/aqemu
echo "  command=\"$BINDIR/aqemu\" \\" >> $DATADIR/menu/aqemu
echo "  icon=\"$DATADIR/pixmaps/aqemu_48x48.png\" \\" >> $DATADIR/menu/aqemu

echo "Installation complete!"
exit 0
