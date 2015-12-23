FROM centos:centos7
MAINTAINER donatas.abraitis@gmail.com

RUN yum install epel-release -y

RUN yum install git httpd-devel php-devel gcc make -y

RUN git clone https://github.com/ton31337/mod_reset.git
RUN cd mod_reset && make clean ; make
