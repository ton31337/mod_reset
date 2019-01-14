#include "mod_reset.h"

module AP_MODULE_DECLARE_DATA reset_module;
#ifdef MOD_RUID2
module AP_MODULE_DECLARE_DATA ruid2_module;
#endif

static int reset_handler(request_rec *r)
{
        reset_config *conf = (reset_config *) ap_get_module_config(r->server->module_config, &reset_module);
#ifdef MOD_RUID2
        ruid_dir_config_t *ruid = ap_get_module_config(r->per_dir_config, &ruid2_module);
#endif
#ifdef APACHE_22
        core_server_config *core = ap_get_module_config(r->server->module_config, &core_module);
#endif
        if (conf->enable) {
                if (conf->deny_header) {
                        char *proxy_ts = (char *) apr_table_get(r->headers_in, conf->deny_header);

                        /* Check if request contains arbitrary header set by proxy */
                        if (!proxy_ts) {
                                ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, "ResetDenyHeader: not found");
                                return HTTP_FORBIDDEN;
                        }

                        apr_time_t current_ts = apr_time_now();

                        /* Check if current TS is valid */
                        if (apr_time_sec(current_ts) < atoi(proxy_ts)) {
                                ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, "ResetDenyHeader: time differs");
                                return HTTP_FORBIDDEN;
                        }
                }
#ifndef NO_PHP
                apr_array_header_t *arr = (apr_array_header_t *) apr_table_elts(conf->php_ini);
                apr_table_entry_t *ini = (apr_table_entry_t *) arr->elts;

                /* Setting php.ini values on the fly */
                int i;
                for (i = 0; i < arr->nelts; i++) {
                        char *value = (char *) apr_table_get(r->headers_in, ini[i].val);
                        if (!value)
                                continue;

#if PHP_MAJOR_VERSION >= 7
                        zend_string *key = zend_string_init(ini[i].key, strlen(ini[i].key) , 0);
                        zend_string *val = zend_string_init(value, strlen(value), 0);
                        zend_alter_ini_entry(key, val, ZEND_INI_SYSTEM, ZEND_INI_STAGE_ACTIVATE);
                        zend_string_release(key);
                        zend_string_release(val);
#else
                        zend_alter_ini_entry(ini[i].key, strlen(ini[i].key) + 1, value, strlen(value), ZEND_INI_SYSTEM, ZEND_INI_STAGE_ACTIVATE);
#endif
                }

#ifdef MOD_RUID2
                char *ruid_uid_header = (char *) apr_table_get(r->headers_in, conf->ruid_uid);
                if (ruid_uid_header) {
                        ruid->ruid_uid = ap_uname2id(ruid_uid_header);
                        ruid->ruid_gid = ap_uname2id(ruid_uid_header);
                }
#endif
#endif

                /* Setting DocumentRoot */
                char *docroot = (char *) apr_table_get(r->headers_in, conf->docroot);
                if (docroot && ap_is_directory(r->pool, docroot)) {
#ifdef APACHE_22
                        core->ap_document_root = docroot;
#else
                        ap_set_document_root(r, docroot);
#endif
                        apr_table_setn(r->subprocess_env, "PHP_DOCUMENT_ROOT", docroot);
                        apr_table_setn(r->subprocess_env, "DOCUMENT_ROOT", docroot);
                } else {
                        ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, "Can't set DocumentRoot as %s!", docroot);
                        return HTTP_FORBIDDEN;
                }

#ifndef NO_PHP
               /* Set TMPDIR environment variable */
               char *tmpdir = (char *) apr_table_get(r->headers_in, conf->tmpdir);
               if (tmpdir && ap_is_directory(r->pool, tmpdir)) {
                   if (apr_env_set("TMPDIR", tmpdir, r->pool) != APR_SUCCESS) {
                       ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, "Can't set TMPDIR environment as %s!", tmpdir);
                   }
               } else {
                   ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, "TMPDIR directory does not exist, check headers!");
               }
#endif

               /* HACK: mod_lsapi uses r->server->server_hostname for socket names:
                * /var/run/mod_lsapi/lsapi_application-x-httpd-lsphp_100000106_$server_hostname.sock=
                * Proper solution would be to patch mod_lsapi to use HOST header instead when dynamic
                * vhosts are in use.
                */
               char *hostname = (char *)apr_table_get(r->headers_in, "HOST");
               if (hostname) {
                       r->server->server_hostname = hostname;
                       r->server->is_virtual = 1;
               }

               /* Setting ServerAdmin */
               char *admin = (char *)apr_table_get(r->headers_in, conf->admin);
               if (admin)
               {
                       apr_table_setn(r->subprocess_env, "SERVER_ADMIN", admin);
                       r->server->server_admin = admin;
                }
        }
        return DECLINED;
}

static void reset_module_register_hooks(apr_pool_t *p)
{
        ap_hook_translate_name(reset_handler, NULL, NULL, APR_HOOK_FIRST);
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

static const char *denyheader_reset(cmd_parms *cmd, void *cfg, const char arg[])
{
        reset_config *conf = (reset_config *) ap_get_module_config(cmd->server->module_config, &reset_module);
        if (arg != NULL && strlen(arg) > 0)
                conf->deny_header = arg;
        return NULL;
}

static const char *headers_reset(cmd_parms *cmd, void *cfg, const char *ini, const char *header)
{
        reset_config *conf = (reset_config *) ap_get_module_config(cmd->server->module_config, &reset_module);
        if (conf->enable) {
                if (ini != NULL && header != NULL) {
                        conf->nheaders++;
                        apr_table_setn(conf->php_ini, ini, header);
                }
        }
        return NULL;
}

static const char *header_reset(cmd_parms *cmd, void *cfg, const char *dir, const char *header)
{
        reset_config *conf = (reset_config *) ap_get_module_config(cmd->server->module_config, &reset_module);
        if (conf->enable) {
                if (header && *header) {
                        if (!strncmp(dir, "TMPDIR", sizeof("TMPDIR"))) {
                                conf->tmpdir = (char *) header;
                        } else if (!strncmp(dir, "DocumentRoot", sizeof("DocumentRoot"))) {
                                conf->docroot = (char *) header;
                        } else if (!strncmp(dir, "ServerAdmin", sizeof("ServerAdmin"))) {
                                conf->admin = (char *) header;
#ifdef MOD_RUID2
                        } else if (!strncmp(dir, "RUidGid", sizeof("RUidGid"))) {
                                conf->ruid_uid = (char *) header;
#endif
                        }
                }
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
        AP_INIT_TAKE1("ResetDenyHeader", denyheader_reset, NULL, RSRC_CONF, "Set header which is used to deny unauthenticated requests."),
        AP_INIT_TAKE2("ResetHeaders", headers_reset, NULL, RSRC_CONF, "Configure headers to be used for checking."),
        AP_INIT_TAKE2("ResetHeader", header_reset, NULL, RSRC_CONF, "Configure header for setting ServerAdmin/DocumentRoot."),
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
