sed -e '/\/opt\/grass-*/d' -e '/\/opt\/grass/d' -i /etc/ld.so.conf
echo '/opt/grass/lib' >> etc/ld.so.conf
sbin/ldconfig -r .

