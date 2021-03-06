%define build_timestamp %(date +%s)
Summary: mod_reset for Apache 2.4 and PHP 5.5
Name: mod_reset_apache24-php55
Version: 1
Release: %{build_timestamp}
Group: Web
License: GPL+

%description
* GIT_COMMIT: %{getenv:GIT_COMMIT}
* BUILD_URL: %{getenv:BUILD_URL}

%install
mkdir -p %{buildroot}/usr/lib64/httpd/modules/
cp /usr/lib64/httpd/modules/mod_reset.so %{buildroot}/usr/lib64/httpd/modules/

%build

%clean

%files
%defattr(644,root,root,-)

/usr/lib64/httpd/modules/mod_reset.so

%changelog
