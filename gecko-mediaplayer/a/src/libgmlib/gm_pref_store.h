/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * gm_pref_store.h
 * Copyright (C) Kevin DeKorte 2006 <kdekorte@gmail.com>
 * 
 * gm_pref_store.h is free software.
 * 
 * You may redistribute it and/or modify it under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 * 
 * gm_pref_store.h is distributed in the hope that it will be useful,
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
#include <glib.h>
#include <string.h>
#ifndef __GM_PREF_STORE_H__
#define __GM_PREF_STORE_H__

typedef struct _GmPrefStore GmPrefStore;

#ifdef __cplusplus
extern "C" {
#endif

    GmPrefStore *gm_pref_store_new(const gchar * context);
    void gm_pref_store_free(GmPrefStore * store);

    gboolean gm_pref_store_get_boolean(GmPrefStore * store, const gchar * key);
    gboolean gm_pref_store_get_boolean_with_default(GmPrefStore * store, const gchar * key, gboolean default_value);
    void gm_pref_store_set_boolean(GmPrefStore * store, const gchar * key, gboolean value);

    gint gm_pref_store_get_int(GmPrefStore * store, const gchar * key);
    gint gm_pref_store_get_int_with_default(GmPrefStore * store, const gchar * key, gint default_value);
    void gm_pref_store_set_int(GmPrefStore * store, const gchar * key, gint value);

    gfloat gm_pref_store_get_float(GmPrefStore * store, const gchar * key);
    void gm_pref_store_set_float(GmPrefStore * store, const gchar * key, gfloat value);

    gchar *gm_pref_store_get_string(GmPrefStore * store, const gchar * key);
    void gm_pref_store_set_string(GmPrefStore * store, const gchar * key, gchar * value);

    void gm_pref_store_unset(GmPrefStore * store, const gchar * key);
#ifdef __cplusplus
}
#endif
#endif
