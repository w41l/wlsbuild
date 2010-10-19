if ! grep "^kvm:" /etc/group >/dev/null 2>&1; then
  /usr/sbin/groupadd -g 186 kvm || exit 1
fi;
