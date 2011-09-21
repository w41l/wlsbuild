/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#ifndef _PLUGIN_LIST_H
#define _PLUGIN_LIST_H
#include <glib.h>
#include <glib/gstdio.h>
#include <stdio.h>
#include <string.h>
#include "memmem_compat.h"

typedef struct _ListItem {
    gchar src[4096];
    gchar local[1024];
    gchar path[1024];
    gint id;
    gint hrefid;
    gint controlid;
    gboolean cancelled;
    gboolean playerready;
    gboolean newwindow;
    gboolean streaming;
    gboolean requested;
    gboolean retrieved;
    gboolean play;
    gboolean played;
    gboolean opened;
    guint mediasize;
    gint localsize;
    gint lastsize;
    FILE *localfp;
    gboolean loop;              // TRUE if we should loop
    gint loopcount;             // 0 - loop done, -1 loop forever
    gint bitrate;
    gint bitrate_requests;
    void *plugin;
} ListItem;



void asx_start_element(GMarkupParseContext * context,
                       const gchar * element_name,
                       const gchar ** attribute_names,
                       const gchar ** attribute_values, gpointer user_data, GError ** error);

void asx_end_element(GMarkupParseContext * context,
                     const gchar * element_name, gpointer user_data, GError ** error);

void element_text(GMarkupParseContext * context,
                  const gchar * text, gsize text_len, gpointer user_data, GError ** error);

static const GMarkupParser asx_parser = {
    asx_start_element,
    asx_end_element,
    NULL,                       //element_text,
    NULL,
    NULL,
};

void qml_start_element(GMarkupParseContext * context,
                       const gchar * element_name,
                       const gchar ** attribute_names,
                       const gchar ** attribute_values, gpointer user_data, GError ** error);

static const GMarkupParser qml_parser = {
    qml_start_element,
    NULL,
    NULL,
    NULL,
    NULL,
};



ListItem *list_find(GList * list, gchar * url);
ListItem *list_find_by_id(GList * list, gint id);
ListItem *list_find_by_controlid(GList * list, gint id);
void list_mark_controlid_ready(GList * list, gint id);
void list_mark_controlid_cancelled(GList * list, gint id, gboolean cancelled);
void list_mark_id_played(GList * list, gint id);
ListItem *list_find_next_playable(GList * list);
void list_qualify_url(GList * list, gchar * page_url);
GList *list_clear(GList * list);
void list_dump(GList * list);
GList *list_parse_qt(GList * list, ListItem * item);
GList *list_parse_qt2(GList * list, ListItem * item);
GList *list_parse_asx(GList * list, ListItem * item);
GList *list_parse_qml(GList * list, ListItem * item);   // Quick Time Media Link
GList *list_parse_ram(GList * list, ListItem * item);   // Real Audio Playlist

#endif                          // _PLUGIN_LIST_H
