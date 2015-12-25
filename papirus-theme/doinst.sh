
# Create symbolic link for libreoffice icon
echo "Creating symlink for papirus libreoffice icon in:"
if test -n $( uname -m | grep "64" ); then
  echo "/usr/lib64/libreoffice/share/config/images_papirus.zip";
  ( mkdir -p /usr/lib64/libreoffice/share/config;
    cd /usr/lib64/libreoffice/share/config;
    rm -f images_papirus.zip;
    ln -s /usr/share/libreoffice/share/config/images_papirus.zip images_papirus.zip;
  )
else
  echo "/usr/lib/libreoffice/share/config/images_papirus.zip";
  ( mkdir -p /usr/lib/libreoffice/share/config;
    cd /usr/lib/libreoffice/share/config;
    rm -f images_papirus.zip;
    ln -s /usr/share/libreoffice/share/config/images_papirus.zip images_papirus.zip;
  )
fi
