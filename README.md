Requirements
=========
This module works properly only with `mod_php` and `prefork` MPM.

Overview
=========
The main idea is simple, just use this module together with reverse proxy in front of Apache. Set custom headers from reverse proxy and manage php.ini values, DocumentRoot, ServerName according to headers.
For instance you set `ResetHeaders              "open_basedir"   "X-Document-Root"` and this will set `open_basedir` on the fly to value from `X-Document-Root` header.

Compile
============
```
make
make ruid (with mod_ruid2 support)
make nophp (without php support)
make clean
```

Install
============
!!! IMPORTANT !!!
This module MUST be loaded after mod_php.
```
LoadModule reset_module /usr/lib64/httpd/modules/mod_reset.so
```

Configuration
==============
```
Reset On
ResetHeaders          "open_basedir"    "X-Document-Root"
ResetHeaders          "upload_tmp_dir"  "X-Tmp-Dir"
ResetHeader           "ServerAdmin"     "X-Server-Admin"
ResetHeader           "DocumentRoot"    "X-Document-Root"
ResetHeader           "RUidGid"         "X-RUID-User"
```
