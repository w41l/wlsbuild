
if [ -f /usr/share/dict/cracklib-words ]; then
  create-cracklib-dict /usr/share/dict/cracklib-words >/dev/null 2>&1
fi

if [ -f /usr/share/dict/cracklib-extra-words ]; then
  create-cracklib-dict /usr/share/dict/cracklib-extra-words >/dev/null 2>&1
fi
