FROM centos:centos6
MAINTAINER donatas.abraitis@gmail.com

ARG GIT_COMMIT
ARG BUILD_URL

RUN yum install git httpd-devel php-devel gcc make rpm-build -y

RUN git clone https://github.com/ton31337/mod_reset.git
RUN cd mod_reset ; make

RUN mkdir -p /root/rpmbuild/{BUILD,RPMS,SOURCES,SPECS,SRPMS,tmp}
COPY apache22-php53.spec /root/rpmbuild/SPECS
RUN rpmbuild -bb /root/rpmbuild/SPECS/apache22-php53.spec
