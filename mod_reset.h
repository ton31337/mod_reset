#define MODULE_NAME "reset"

#include "httpd.h"
#include "http_log.h"
#include <zend.h>
#include <zend_qsort.h>
#include <zend_API.h>
#include <zend_ini.h>
#include <zend_alloc.h>
#include <zend_operators.h>

typedef struct {
        unsigned long enable:1;
        apr_table_t   *php_ini;
        char          *admin;
        char          *docroot;
        int           nheaders;
} reset_config;
