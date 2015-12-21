#include "mod_reset.h"

module AP_MODULE_DECLARE_DATA reset_module;

static int reset_handler(request_rec *r)
{
        reset_config *conf = (reset_config *) ap_get_module_config(r->server->module_config, &reset_module);
        if (conf->enable) {
                apr_array_header_t *arr = (apr_array_header_t *) apr_table_elts(conf->php_ini);
                apr_table_entry_t *ini = (apr_table_entry_t *) arr->elts;

                int i;
                for (i = 0; i < arr->nelts; i++) {
                        char *value = (char *) apr_table_get(r->headers_in, ini[i].val);
                        zend_alter_ini_entry(ini[i].key, strlen(ini[i].key) + 1, value, strlen(value), ZEND_INI_SYSTEM, ZEND_INI_STAGE_ACTIVATE);
                }
        }
        return DECLINED;
}

static void reset_module_register_hooks(apr_pool_t *p)
{
        ap_hook_handler(reset_handler, NULL, NULL, APR_HOOK_FIRST);
}

static const char *enable_reset(cmd_parms *cmd, void *cfg, const char arg[])
{
        reset_config *conf = (reset_config *) ap_get_module_config(cmd->server->module_config, &reset_module);
        if (arg != NULL && !strncmp(arg, "On", 2)) {
                conf->enable = 1;
                conf->php_ini = apr_table_make(cmd->pool, conf->nheaders);
        }
        return NULL;
}

static const char *headers_reset(cmd_parms *cmd, void *cfg, const char *ini, const char *header)
{
        reset_config *conf = (reset_config *) ap_get_module_config(cmd->server->module_config, &reset_module);
        if (ini != NULL && header != NULL) {
                conf->nheaders++;
                apr_table_set(conf->php_ini, ini, header);
        }
        return NULL;
}

static void * reset_create_server_config(apr_pool_t * p, server_rec * s) {
        reset_config *conf = (reset_config *) apr_pcalloc(p, sizeof(reset_config));
        conf->enable = 0;
        conf->nheaders = 0;
        return (void *)conf;
}

static const command_rec reset_module_directives[] =
{
        AP_INIT_TAKE1("Reset", enable_reset, NULL, RSRC_CONF, "Enable/Disable reset module."),
        AP_INIT_TAKE2("ResetHeaders", headers_reset, NULL, RSRC_CONF, "Configure headers to be used for checking."),
        {NULL}
};

module AP_MODULE_DECLARE_DATA reset_module = {
        STANDARD20_MODULE_STUFF,
        NULL,
        NULL,
        reset_create_server_config,
        NULL,
        reset_module_directives,
        reset_module_register_hooks
};
