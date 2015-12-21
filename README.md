Compile
============
```
apxs -c -I/usr/include/php/main -I/usr/include/php/Zend -I/usr/include/php/TSRM -i mod_reset.c
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
ResetHeaders "open_basedir"   "X-Document-Root"
ResetHeaders "upload_tmp_dir" "X-Tmp-Dir"
```
