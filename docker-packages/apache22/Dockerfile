FROM akrasic/centos5
MAINTAINER donatas.abraitis@gmail.com

COPY utter.repo /etc/yum.repos.d
RUN rpm --import http://yum.jasonlitka.com/RPM-GPG-KEY-jlitka

RUN yum install epel-release -y
RUN yum install git httpd-devel php-devel gcc make -y ; true

RUN git clone https://github.com/ton31337/mod_reset.git
RUN cd mod_reset && make clean ; make
