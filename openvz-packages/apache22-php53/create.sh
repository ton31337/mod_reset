vzctl destroy 53
vzctl create 53 --ostemplate centos-6-x86_64 --ipadd 192.168.0.53
vzctl start 53
vzctl exec 53 'yum clean all ; yum update -y'
vzctl exec 53 yum install git httpd httpd-devel php php-devel gcc make sssd -y
vzctl exec 53 'cd /root ; git clone https://github.com/hostinger/mod_reset.git'
vzctl exec 53 'cd /root/mod_reset ; make'
vzctl exec 53 chkconfig httpd on
vzctl exec 53 chkconfig sssd on
vzctl exec 53 yum remove httpd-devel php-devel -y
vzctl exec 53 wget -O /etc/nsswitch.conf http://repo.main-hosting.com/cloud/sssd/nsswitch.conf
vzctl exec 53 wget -O /etc/sssd/sssd.conf http://repo.main-hosting.com/cloud/sssd/sssd.conf
vzctl exec 53 chmod 600 /etc/sssd/sssd.conf
vzctl stop 53
vzctl mount 53
cd /vz/root/53
tar --numeric-owner -czf /root/apache22-php53-$(date +%s).tar.gz .
cd
vzctl umount 53
