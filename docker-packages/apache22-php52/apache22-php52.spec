%define build_timestamp %(date +%s)
Summary: mod_reset for Apache 2.2
Name: mod_reset_apache22
Version: 1
Release: %{build_timestamp}
Group: Web
License: GPL+

%description

%prep

%build

%install

%clean

%files
%defattr(644,root,root,-)

/usr/lib64/httpd/modules/mod_reset.so

%changelog
