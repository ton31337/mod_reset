%define build_timestamp %(date +%s)
Summary: mod_reset for Apache 2.2 and PHP 5.3
Name: mod_reset_apache22-php53
Version: 1
Release: %{build_timestamp}
Group: Web
License: GPL+

%description

%install
mkdir -p %{buildroot}/usr/lib64/httpd/modules/
cp /usr/lib64/httpd/modules/mod_reset.so %{buildroot}/usr/lib64/httpd/modules/

%build

%clean

%files
%defattr(644,root,root,-)

/usr/lib64/httpd/modules/mod_reset.so

%changelog
