#define MODULE_NAME "reset"
#define CORE_PRIVATE
#include <ap_config.h>
#include <apr_strings.h>
#include <httpd.h>
#include <http_config.h>
#include <http_core.h>
#include <http_log.h>
#ifndef NO_PHP
#include <php_version.h>
#include <zend.h>
#if PHP_MAJOR_VERSION >= 7
#include <zend_sort.h>
#else
#include <zend_qsort.h>
#endif
#include <zend_API.h>
#include <zend_ini.h>
#include <zend_alloc.h>
#include <zend_operators.h>
#endif

#if AP_SERVER_MINORVERSION_NUMBER == 2
#define APACHE_22
#endif

typedef struct {
        unsigned long   enable:1;
        apr_table_t     *php_ini;
        char            *admin;
        char            *docroot;
        char            *tmpdir;
        int             nheaders;
#ifdef MOD_RUID2
        char            *ruid_uid;
#endif
        char            *deny_header;
        char            *hash;
} reset_config;

#ifdef MOD_RUID2
#define RUID_MAXGROUPS        8
typedef struct
{
        int8_t ruid_mode;
        uid_t ruid_uid;
        gid_t ruid_gid;
        gid_t groups[RUID_MAXGROUPS];
        int groupsnr;
} ruid_dir_config_t;
#endif
