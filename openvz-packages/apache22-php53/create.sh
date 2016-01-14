vzctl umount 53
vzctl stop 53
vzctl destroy 53 || exit 1
vzctl create 53 --ostemplate centos-6-x86_64 --ipadd 192.168.0.53 || exit 1
vzctl start 53 || exit 1
vzctl exec 53 'yum clean all ; yum update -y ; yum install epel-release -y' || exit 1
vzctl exec 53 yum install git httpd httpd-devel php php-devel gcc make sssd mod_ruid2 -y || exit 1
vzctl exec 53 'cd /root ; git clone https://github.com/hostinger/mod_reset.git' || exit 1
vzctl exec 53 'cd /root/mod_reset ; make' || exit 1
vzctl exec 53 'cd /root ; git clone https://github.com/gnif/mod_rpaf.git' || exit 1
vzctl exec 53 'cd /root/mod_rpaf ; make && make install' || exit 1
vzctl exec 53 chkconfig httpd on || exit 1
vzctl exec 53 chkconfig sssd on || exit 1
vzctl exec 53 wget -O /etc/nsswitch.conf http://repo.main-hosting.com/cloud/sssd/nsswitch.conf || exit 1
vzctl exec 53 wget -O /etc/sssd/sssd.conf http://repo.main-hosting.com/cloud/sssd/sssd.conf || exit 1
vzctl exec 53 chmod 600 /etc/sssd/sssd.conf || exit 1
vzctl exec 53 yum remove httpd-devel php-devel -y || exit 1
vzctl exec 53 rm -rf /root/mod_reset || exit 1
vzctl exec 53 rm -rf /root/mod_rpaf || exit 1
vzctl stop 53 || exit 1
vzctl mount 53 || exit 1
cd /vz/root/53 && tar --numeric-owner -czf /root/apache22-php53-$(date +%s).tar.gz . || exit 1
