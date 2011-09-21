/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * gm_pref_store.c
 * Copyright (C) Kevin DeKorte 2006 <kdekorte@gmail.com>
 * 
 * gm_pref_store.c is free software.
 * 
 * You may redistribute it and/or modify it under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 * 
 * gm_pref_store.c is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with playlist.c.  If not, write to:
 * 	The Free Software Foundation, Inc.,
 * 	51 Franklin Street, Fifth Floor
 * 	Boston, MA  02110-1301, USA.
 */

#include "gm_pref_store.h"

#if GIO_ENABLED && GLIB2_26_ENABLED
#include <gio/gio.h>
#else
#ifdef HAVE_GCONF
#include <gconf/gconf.h>
#include <gconf/gconf-client.h>
#include <gconf/gconf-value.h>
#endif
#endif
#include <stdio.h>

struct _GmPrefStore {
#if GIO_ENABLED && GLIB2_26_ENABLED
    GSettings *settings;
#else
#ifdef HAVE_GCONF
    GConfClient *gconf;
#else
    GKeyFile *keyfile;
#endif
#endif
    gchar *context;
};

GmPrefStore *gm_pref_store_new(const gchar * context)
{

    GmPrefStore *store = (GmPrefStore *) g_new0(GmPrefStore, 1);

#if GIO_ENABLED && GLIB2_26_ENABLED
    store->context = g_strdup_printf("apps.%s.preferences", context);
    store->settings = g_settings_new(store->context);
#else
    store->context = g_strdup(context);
#ifdef HAVE_GCONF
    store->gconf = gconf_client_get_default();
#else
    gchar *filename;

    filename = g_strdup_printf("%s/%s", g_get_user_config_dir(), context);
    if (!g_file_test(filename, G_FILE_TEST_IS_DIR)) {
        g_mkdir_with_parents(filename, 0775);
    }
    g_free(filename);

    store->keyfile = g_key_file_new();
    filename = g_strdup_printf("%s/%s/%s.conf", g_get_user_config_dir(), context, context);
    g_key_file_load_from_file(store->keyfile, filename, G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS, NULL);

#endif
#endif
    return store;
}

void gm_pref_store_free(GmPrefStore * store)
{

#if GIO_ENABLED && GLIB2_26_ENABLED
    g_object_unref(store->settings);
    store->settings = NULL;
#else
#ifdef HAVE_GCONF
    if (G_IS_OBJECT(store->gconf))
        g_object_unref(G_OBJECT(store->gconf));
    store->gconf = NULL;
#else
    gchar *filename;
    gchar *data;

    if (store->keyfile != NULL) {
        filename = g_strdup_printf("%s/%s/%s.conf", g_get_user_config_dir(), store->context, store->context);
        data = g_key_file_to_data(store->keyfile, NULL, NULL);
        if (data != NULL) {
            g_file_set_contents(filename, data, -1, NULL);
            g_free(data);
        }
        g_free(filename);
        g_key_file_free(store->keyfile);
        store->keyfile = NULL;
    }
#endif
#endif
    g_free(store->context);
    store->context = NULL;

    g_free(store);
    store = NULL;

}

gboolean gm_pref_store_get_boolean(GmPrefStore * store, const gchar * key)
{

    gboolean value = FALSE;

    if (store == NULL)
        return FALSE;

#if GIO_ENABLED && GLIB2_26_ENABLED
    value = g_settings_get_boolean(store->settings, key);
#else
#ifdef HAVE_GCONF
    gchar *full_key;

    full_key = g_strdup_printf("/apps/%s/preferences/%s", store->context, key);
    value = gconf_client_get_bool(store->gconf, full_key, NULL);
    g_free(full_key);
#else
    if (g_key_file_has_key(store->keyfile, store->context, key, NULL))
        value = g_key_file_get_boolean(store->keyfile, store->context, key, NULL);
#endif
#endif
    return value;
}

gboolean gm_pref_store_get_boolean_with_default(GmPrefStore * store, const gchar * key, gboolean default_value)
{

    gboolean value = FALSE;

    if (store == NULL)
        return default_value;

#if GIO_ENABLED && GLIB2_26_ENABLED
    value = g_settings_get_boolean(store->settings, key);
#else
#ifdef HAVE_GCONF

    gchar *full_key;
    GConfValue *gcvalue;

    full_key = g_strdup_printf("/apps/%s/preferences/%s", store->context, key);

    gcvalue = gconf_client_get_without_default(store->gconf, full_key, NULL);
    if (gcvalue) {
        value = gconf_client_get_bool(store->gconf, full_key, NULL);
        gconf_value_free(gcvalue);
    } else {
        value = default_value;
    }
    g_free(full_key);

#else

    if (g_key_file_has_key(store->keyfile, store->context, key, NULL)) {
        value = g_key_file_get_boolean(store->keyfile, store->context, key, NULL);
    } else {
        value = default_value;
    }
#endif
#endif
    return value;
}

void gm_pref_store_set_boolean(GmPrefStore * store, const gchar * key, gboolean value)
{

    if (store == NULL)
        return;
#if GIO_ENABLED && GLIB2_26_ENABLED
    g_settings_set_boolean(store->settings, key, value);
#else
#ifdef HAVE_GCONF
    gchar *full_key;

    full_key = g_strdup_printf("/apps/%s/preferences/%s", store->context, key);
    gconf_client_set_bool(store->gconf, full_key, value, NULL);
    g_free(full_key);
#else

    g_key_file_set_boolean(store->keyfile, store->context, key, value);

#endif
#endif
}

gint gm_pref_store_get_int(GmPrefStore * store, const gchar * key)
{

    gint value = 0;

    if (store == NULL)
        return value;
#if GIO_ENABLED && GLIB2_26_ENABLED
    value = g_settings_get_int(store->settings, key);
#else
#ifdef HAVE_GCONF
    gchar *full_key;

    full_key = g_strdup_printf("/apps/%s/preferences/%s", store->context, key);
    value = gconf_client_get_int(store->gconf, full_key, NULL);
    g_free(full_key);
#else

    if (g_key_file_has_key(store->keyfile, store->context, key, NULL))
        value = g_key_file_get_integer(store->keyfile, store->context, key, NULL);

#endif
#endif
    return value;
}

gint gm_pref_store_get_int_with_default(GmPrefStore * store, const gchar * key, gint default_value)
{

    gint value = 0;

    if (store == NULL)
        return default_value;

#if GIO_ENABLED && GLIB2_26_ENABLED
    value = g_settings_get_int(store->settings, key);
#else
#ifdef HAVE_GCONF

    gchar *full_key;
    GConfValue *gcvalue;

    full_key = g_strdup_printf("/apps/%s/preferences/%s", store->context, key);

    gcvalue = gconf_client_get_without_default(store->gconf, full_key, NULL);
    if (gcvalue) {
        value = gconf_client_get_int(store->gconf, full_key, NULL);
        gconf_value_free(gcvalue);
    } else {
        value = default_value;
    }
    g_free(full_key);

#else

    if (g_key_file_has_key(store->keyfile, store->context, key, NULL)) {
        value = g_key_file_get_integer(store->keyfile, store->context, key, NULL);
    } else {
        value = default_value;
    }

#endif
#endif
    return value;
}


void gm_pref_store_set_int(GmPrefStore * store, const gchar * key, gint value)
{

    if (store == NULL)
        return;

#if GIO_ENABLED && GLIB2_26_ENABLED
    g_settings_set_int(store->settings, key, value);
#else
#ifdef HAVE_GCONF
    gchar *full_key;

    full_key = g_strdup_printf("/apps/%s/preferences/%s", store->context, key);
    gconf_client_set_int(store->gconf, full_key, value, NULL);
    g_free(full_key);
#else

    g_key_file_set_integer(store->keyfile, store->context, key, value);

#endif
#endif
}

gfloat gm_pref_store_get_float(GmPrefStore * store, const gchar * key)
{

    gfloat value = 0.0;

    if (store == NULL)
        return value;

#if GIO_ENABLED && GLIB2_26_ENABLED
    value = g_settings_get_double(store->settings, key);
#else
#ifdef HAVE_GCONF
    gchar *full_key;

    full_key = g_strdup_printf("/apps/%s/preferences/%s", store->context, key);
    value = gconf_client_get_float(store->gconf, full_key, NULL);
    g_free(full_key);
#else

    if (g_key_file_has_key(store->keyfile, store->context, key, NULL))
        value = g_key_file_get_double(store->keyfile, store->context, key, NULL);

#endif
#endif
    return value;
}

void gm_pref_store_set_float(GmPrefStore * store, const gchar * key, gfloat value)
{

    if (store == NULL)
        return;
#if GIO_ENABLED && GLIB2_26_ENABLED
    g_settings_set_double(store->settings, key, value);
#else
#ifdef HAVE_GCONF
    gchar *full_key;

    full_key = g_strdup_printf("/apps/%s/preferences/%s", store->context, key);
    gconf_client_set_float(store->gconf, full_key, value, NULL);
    g_free(full_key);
#else

    g_key_file_set_double(store->keyfile, store->context, key, value);

#endif
#endif
}

gchar *gm_pref_store_get_string(GmPrefStore * store, const gchar * key)
{

    gchar *value = NULL;

    if (store == NULL)
        return value;

#if GIO_ENABLED && GLIB2_26_ENABLED
    value = g_settings_get_string(store->settings, key);
#else
#ifdef HAVE_GCONF
    gchar *full_key;

    full_key = g_strdup_printf("/apps/%s/preferences/%s", store->context, key);
    value = gconf_client_get_string(store->gconf, full_key, NULL);
    g_free(full_key);
#else

    if (g_key_file_has_key(store->keyfile, store->context, key, NULL))
        value = g_key_file_get_string(store->keyfile, store->context, key, NULL);

#endif
#endif
    return value;
}

void gm_pref_store_set_string(GmPrefStore * store, const gchar * key, gchar * value)
{

    if (store == NULL)
        return;
#if GIO_ENABLED && GLIB2_26_ENABLED
    if (value == NULL) {
        g_settings_reset(store->settings, key);
    } else {
        g_settings_set_string(store->settings, key, value);
    }
#else
#ifdef HAVE_GCONF
    gchar *full_key;

    full_key = g_strdup_printf("/apps/%s/preferences/%s", store->context, key);
    gconf_client_unset(store->gconf, full_key, NULL);
    if (value != NULL && strlen(g_strstrip(value)) > 0)
        gconf_client_set_string(store->gconf, full_key, value, NULL);
    g_free(full_key);
#else
    if (value != NULL && strlen(g_strstrip(value)) > 0) {
        g_key_file_set_string(store->keyfile, store->context, key, value);
    } else {
        g_key_file_remove_key(store->keyfile, store->context, key, NULL);
    }
#endif
#endif
}

void gm_pref_store_unset(GmPrefStore * store, const gchar * key)
{

    if (store == NULL)
        return;
#if GIO_ENABLED && GLIB2_26_ENABLED
    g_settings_reset(store->settings, key);
#else
#ifdef HAVE_GCONF
    gchar *full_key;

    full_key = g_strdup_printf("/apps/%s/preferences/%s", store->context, key);
    gconf_client_unset(store->gconf, full_key, NULL);
    g_free(full_key);
#else
    g_key_file_remove_key(store->keyfile, store->context, key, NULL);
#endif
#endif
}
