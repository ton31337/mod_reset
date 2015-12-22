#define MODULE_NAME "reset"
#define CORE_PRIVATE
#include <ap_config.h>
#include <httpd.h>
#include <http_config.h>
#include <http_core.h>
#include <http_log.h>
#include <zend.h>
#include <zend_qsort.h>
#include <zend_API.h>
#include <zend_ini.h>
#include <zend_alloc.h>
#include <zend_operators.h>

#if AP_SERVER_MINORVERSION_NUMBER == 2
#define APACHE_22
#endif

typedef struct {
        unsigned long enable:1;
        apr_table_t   *php_ini;
        char          *admin;
        char          *docroot;
        int           nheaders;
} reset_config;
