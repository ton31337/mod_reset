FROM centos:centos7
MAINTAINER donatas.abraitis@gmail.com

RUN yum install http://rpms.famillecollet.com/enterprise/remi-release-7.rpm -y

RUN yum install epel-release -y
RUN yum install --enablerepo=remi git httpd-devel php-devel gcc make rpm-build -y

RUN git clone https://github.com/ton31337/mod_reset.git
RUN cd mod_reset ; make

RUN mkdir -p /root/rpmbuild/{BUILD,RPMS,SOURCES,SPECS,SRPMS,tmp}
COPY apache24-php54.spec /root/rpmbuild/SPECS
RUN rpmbuild -bb /root/rpmbuild/SPECS/apache24-php54.spec
