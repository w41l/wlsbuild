# Preserve new configuration files
install_file() {
  NEW="$1"
  OLD="`dirname $NEW`/`basename $NEW .new`"
  # If there's no config file by that name, mv it over:
  if [ ! -r $OLD ]; then
    mv $NEW $OLD
  elif [ "`cat $OLD | md5sum`" = "`cat $NEW | md5sum`" ]; then # toss the redundant copy
    rm $NEW
  fi
  # Otherwise, we leave the .new copy for the admin to consider...
}

install_file etc/mono/2.0/settings.map.new
install_file etc/mono/2.0/Browsers/Compat.browser.new
install_file etc/mono/2.0/DefaultWsdlHelpGenerator.aspx.new
install_file etc/mono/2.0/machine.config.new
install_file etc/mono/2.0/web.config.new
install_file etc/mono/browscap.ini.new
install_file etc/mono/4.0/settings.map.new
install_file etc/mono/4.0/DefaultWsdlHelpGenerator.aspx.new
install_file etc/mono/4.0/machine.config.new
install_file etc/mono/4.0/web.config.new
install_file etc/mono/mconfig/config.xml.new
install_file etc/mono/config.new
