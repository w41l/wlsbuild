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

#include "plugin_list.h"
gint asx_loop;
gint entry_id = 0;
GList *parser_list;
ListItem *parser_item;
extern gint streaming(gchar * url);

ListItem *list_find(GList * list, gchar * url)
{
    ListItem *item;
    GList *iter;

    for (iter = list; iter != NULL; iter = g_list_next(iter)) {
        item = (ListItem *) iter->data;
        // printf("list_find item = %s, url = %s\n",item->src,url);
        if (item != NULL) {
            if (g_ascii_strcasecmp(item->src, url) == 0 && item->play == TRUE) {
                // printf("found %s at %p\n",url,item);
                return item;
            }
//            if (g_strrstr(url, item->src) != NULL && item->play == TRUE) {
//                // printf("found %s at %p\n",url,item);
//                return item;
//            }

        }
    }

    return NULL;
}

ListItem *list_find_by_id(GList * list, gint id)
{
    ListItem *item;
    GList *iter;

    for (iter = list; iter != NULL; iter = g_list_next(iter)) {
        item = (ListItem *) iter->data;
        if (item != NULL) {
            if (item->id == id) {
                return item;
            }
        }
    }

    return NULL;
}

ListItem *list_find_by_controlid(GList * list, gint id)
{
    ListItem *item;
    GList *iter;
    ListItem *ret = NULL;

    for (iter = list; iter != NULL; iter = g_list_next(iter)) {
        if (iter != NULL) {
            item = (ListItem *) iter->data;
            if (item != NULL) {
                if (item->controlid == id) {
                    ret = item;
                }
            }
        }
    }

    return ret;
}

void list_mark_controlid_ready(GList * list, gint id)
{
    ListItem *item;
    GList *iter;

    for (iter = list; iter != NULL; iter = g_list_next(iter)) {
        item = (ListItem *) iter->data;
        if (item != NULL) {
            if (item->controlid == id) {
                item->playerready = TRUE;
            }
        }
    }

    return;
}

void list_mark_controlid_cancelled(GList * list, gint id, gboolean cancelled)
{
    ListItem *item;
    GList *iter;

    for (iter = list; iter != NULL; iter = g_list_next(iter)) {
        item = (ListItem *) iter->data;
        if (item != NULL) {
            if (item->controlid == id) {
                item->cancelled = cancelled;
                item->opened = FALSE;
                item->playerready = FALSE;
                item->requested = FALSE;
                item->retrieved = FALSE;
                item->played = FALSE;
            }
        }
    }

    return;
}

void list_mark_id_played(GList * list, gint id)
{
    ListItem *item;
    GList *iter;

    if (id < 0)
        return;
    for (iter = list; iter != NULL; iter = g_list_next(iter)) {
        item = (ListItem *) iter->data;
        if (item != NULL) {
            if (item->id == id) {
                item->played = TRUE;
            }
        }
    }
}

ListItem *list_find_next_playable(GList * list)
{
    ListItem *item;
    GList *iter;

    for (iter = list; iter != NULL; iter = g_list_next(iter)) {
        item = (ListItem *) iter->data;
        if (item != NULL) {
            if (item->played == FALSE && item->play == TRUE) {
                // printf("next playable is %s\n",item->src);
                return item;
            }
        }
    }
    return NULL;
}

void list_qualify_url(GList * list, gchar * page_url)
{
    ListItem *item;
    GList *iter;
    gchar *tmp;
    gchar url[4096];

    for (iter = list; iter != NULL; iter = g_list_next(iter)) {
        item = (ListItem *) iter->data;
        if (item != NULL && item->streaming) {
            tmp = g_strrstr(item->src, "://");
            if (!tmp && !g_file_test(item->src, G_FILE_TEST_EXISTS)) {
                if (page_url != NULL) {
                    //printf("page_url = %s\n", instance->page_url);
                    g_strlcpy(url, item->src, 4096);
                    g_strlcpy(item->src, page_url, 1024);
                    tmp = g_strrstr(item->src, "/");
                    tmp[1] = '\0';
                    //printf("item->src = %s\n", item->src);
                    g_strlcat(item->src, url, 4096);
                }
            }
        }
    }
}

GList *list_clear(GList * list)
{
    ListItem *item;
    GList *iter;

    // printf("clearing list\n");
    if (list != NULL) {
        for (iter = list; iter != NULL; iter = g_list_next(iter)) {
            item = (ListItem *) iter->data;
            if (item != NULL) {
                if (item->localfp) {
                    fclose(item->localfp);
                }
                if (strlen(item->local) > 0) {
                    g_unlink(item->local);
                }
            }
        }
        g_list_free(list);
    }

    return NULL;
}

void list_dump(GList * list)
{
    ListItem *item;
    GList *iter;

    if (list != NULL) {
        for (iter = list; iter != NULL; iter = g_list_next(iter)) {
            item = (ListItem *) iter->data;
            if (item != NULL) {
                printf("Item \n");
                printf("src = %s\n", item->src);
                printf("local = %s\n", item->local);
                printf("id = %i\n", item->id);
                printf("hrefid = %i\n", item->hrefid);
                printf("play = %i\n", item->play);
                printf("played = %i\n", item->played);
                printf("path = %s\n", item->path);
                printf("controlid = %i\n", item->controlid);
                printf("playerready = %i\n", item->playerready);
                printf("newwindow = %i\n", item->newwindow);
                printf("cancelled = %i\n", item->cancelled);
                printf("streaming = %i\n", item->streaming);
                printf("loop = %i\n", item->loop);
                printf("loopcount = %i\n", item->loopcount);
                printf("plugin = %p\n", item->plugin);
            }
        }
    }
}


GList *list_parse_qt(GList * list, ListItem * item)
{
    ListItem *newitem;
    gchar *data;
    gsize datalen;
    gchar *p;
    gchar *nextrmda = NULL;
    gchar *rdrf;                // this is the pointer to a URL that we are adding
    gchar *rmdr;                // this is data about the URL
    gchar url[1024];
    gchar *ptr;
    unsigned int code = 0;      // some value about the URL
    gboolean added = FALSE;

    printf("Entering list_parse_qt localsize = %i\n", item->localsize);

    if (item->localsize < (16 * 1024)) {
        if (g_file_get_contents(item->local, &data, &datalen, NULL)) {
            //printf("read %i bytes from %s\n",datalen, item->local);
            p = (gchar *) memmem_compat(data, datalen, "rmda", 4);
            if (p == NULL) {
                printf("unable to find rmda in %s\n", item->local);
                return list;
            } else {
                if (datalen > 4) {
                    p += 4;     // skip the rmda tag we just found
                    // and find the next one
                    nextrmda = (gchar *) memmem_compat(p, datalen - (p - data), "rmda", 4);
                    if (nextrmda == NULL) {
                        nextrmda = data + datalen;      // point the pointer at the end of the buffer
                    }
                }

                while (p != NULL && !added) {
                    rdrf =
                        (gchar *) memmem_compat(p, datalen - ((long) nextrmda - (long) p), "rdrf",
                                                4);
                    rmdr =
                        (gchar *) memmem_compat(p, datalen - ((long) nextrmda - (long) p), "rmdr",
                                                4);

                    if (rdrf != NULL) {
                        code = (unsigned int) (rdrf[15]);
                        rdrf = rdrf + 16;
                        //printf("URL: %s\n", rdrf);
                        //printf("CODE: %d\n", code);
                    }

                    g_strlcpy(url, item->src, 1024);
                    ptr = g_strrstr(url, "/");
                    if (ptr != NULL && g_strrstr(rdrf, "://") == NULL) {
                        ptr++;
                        ptr[0] = (char) NULL;
                        g_strlcat(url, rdrf, 1024);
                    } else {
                        g_strlcpy(url, rdrf, 1024);
                    }


                    if (rdrf != NULL) {
                        if (code == 163 || code == 165 || code == 167
                            || code == (unsigned int) -93
                            || code == (unsigned int) -91 || code == (unsigned int) -89) {
                            printf("Skipped URL: %s\n", rdrf);
                        } else {
                            if (list_find(list, url) == NULL && strlen(rdrf) > 0) {
                                item->play = FALSE;
                                newitem = g_new0(ListItem, 1);
                                g_strlcpy(newitem->src, url, 1024);
                                // newitem->streaming = streaming(newitem->src);
                                newitem->play = TRUE;
                                newitem->id = item->id;
                                newitem->controlid = item->controlid;
                                g_strlcpy(newitem->path, item->path, 1024);
                                item->id = -1;
                                list = g_list_append(list, newitem);
                                added = TRUE;
                            }
                        }
                    }

                    p = nextrmda + 4;
                    if (p > (data + datalen)) {
                        p = NULL;
                    } else {
                        nextrmda = (gchar *) memmem_compat(p, datalen - (p - data), "rmda", 4);
                        if (nextrmda == NULL) {
                            nextrmda = data + datalen;  // point the pointer at the end of the buffer
                        }
                    }

                }

            }

        } else {
            // printf("Unable to open %s \n",item->local); 
        }
        list_dump(list);

    } else {
        // if file is over 16K it is probably not a playlist
        // so skip parsing it.
        //printf("file not parsed > 16K actual size is %i\n",item->localsize);
    }
    printf("Exiting list_parse_qt\n");
    return list;

}

GList *list_parse_qt2(GList * list, ListItem * item)
{
    ListItem *newitem;
    gchar *data;
    gsize datalen;
    gchar *p;
    gchar url[1024];
    gchar *ptr;
    gchar *urlptr;
    gboolean added = FALSE;

    printf("Entering list_parse_qt2 localsize = %i\n", item->localsize);

    if (item->localsize < (256 * 1024)) {
        if (g_file_get_contents(item->local, &data, &datalen, NULL)) {
            //printf("read %i bytes from %s\n",datalen, item->local);
            p = (gchar *) memmem_compat(data, datalen, "mmdr", 4);
            if (p == NULL) {
                printf("unable to find mmdr in %s\n", item->local);
                return list;
            } else {
                while (p != NULL && !added) {
                    urlptr = (gchar *) memmem_compat(p, datalen - (p - data), "url ", 4);


                    if (urlptr == NULL) {
                        p = NULL;
                    } else {
                        urlptr += strlen("url ");
                        g_strlcpy(url, item->src, 1024);
                        ptr = g_strrstr(url, "/");
                        if (ptr != NULL && g_strrstr(urlptr, "://") == NULL) {
                            ptr++;
                            ptr[0] = (char) NULL;
                            g_strlcat(url, urlptr, 1024);
                        } else {
                            g_strlcpy(url, urlptr, 1024);
                        }

                        if (list_find(list, url) == NULL) {
                            item->play = FALSE;
                            newitem = g_new0(ListItem, 1);
                            g_strlcpy(newitem->src, url, 1024);
                            // newitem->streaming = streaming(newitem->src);
                            newitem->play = TRUE;
                            newitem->id = item->id;
                            newitem->controlid = item->controlid;
                            g_strlcpy(newitem->path, item->path, 1024);
                            item->id = -1;
                            list = g_list_append(list, newitem);
                            added = TRUE;
                        }
                        p = (gchar *) memmem_compat(urlptr, datalen - (urlptr - data), "mmdr", 4);
                    }

                }

            }

        } else {
            // printf("Unable to open %s \n",item->local); 
        }
        list_dump(list);

    } else {
        // if file is over 256K it is probably not a playlist
        // so skip parsing it.
        //printf("file not parsed > 256K actual size is %i\n",item->localsize);
    }
    printf("Exiting list_parse_qt2\n");
    return list;

}

void strip_unicode(gchar * data, gsize len)
{
    gsize i = 0;

    for (i = 0; i < len; i++) {
        if (!g_unichar_validate(data[i])) {
            data[i] = ' ';
        }
    }

}

gboolean entities_present(gchar * data, gsize len)
{

    if (g_strstr_len(data, len, "&amp;") != NULL)
        return TRUE;
    if (g_strstr_len(data, len, "&lt;") != NULL)
        return TRUE;
    if (g_strstr_len(data, len, "&gt;") != NULL)
        return TRUE;
    if (g_strstr_len(data, len, "&quot;") != NULL)
        return TRUE;
    if (g_strstr_len(data, len, "&apos;") != NULL)
        return TRUE;

    return FALSE;
}

void replace_amp(gchar * data)
{

    gchar *pos;

    pos = g_strrstr(data, "&");
    while (pos) {
        pos[0] = '\x01';
        pos = g_strrstr(data, "&");
    }
}

void unreplace_amp(gchar * data)
{
    gchar *pos;

    pos = g_strrstr(data, "\x01");
    while (pos) {
        pos[0] = '&';
        pos = g_strrstr(data, "\x01");
    }

}

GList *list_parse_asx(GList * list, ListItem * item)
{
    GMarkupParseContext *context;
    gchar *data;
    gsize datalen;

    printf("Entering list_parse_asx localsize = %i\n", item->localsize);

    if (item->localsize < (16 * 1024)) {
        if (g_file_get_contents(item->local, &data, &datalen, NULL)) {
            parser_list = list;
            parser_item = item;
            asx_loop = 0;
            strip_unicode(data, datalen);
            replace_amp(data);

            context = g_markup_parse_context_new(&asx_parser, (GMarkupParseFlags) 0, data, NULL);
            g_markup_parse_context_parse(context, data, datalen, NULL);
            g_markup_parse_context_free(context);
            parser_item = NULL;
            parser_list = NULL;
        }
        list_dump(list);
    }
    printf("Exiting list_parse_asx\n");
    return list;
}

void
asx_start_element(GMarkupParseContext * context,
                  const gchar * element_name,
                  const gchar ** attribute_names,
                  const gchar ** attribute_values, gpointer user_data, GError ** error)
{
    ListItem *newitem;
    gchar *value;
    gint i = 0;
    gchar *ptr;
    gchar url[1024];

    if (g_ascii_strcasecmp(element_name, "REF") == 0) {
        while (attribute_names[i] != NULL) {
            if (g_ascii_strcasecmp(attribute_names[i], "HREF") == 0) {

                if (list_find(parser_list, (gchar *) attribute_values[i])
                    == NULL) {
                    parser_item->play = FALSE;
                    newitem = g_new0(ListItem, 1);
                    value = g_strdup(attribute_values[i]);
                    unreplace_amp(value);
                    ptr = g_strrstr(value, "/");
                    if (ptr == NULL) {
                        g_strlcpy(url, parser_item->src, 1024);
                        ptr = g_strrstr(url, "/");
                        if (ptr != NULL) {
                            ptr[1] = (char) NULL;
                            g_strlcpy(newitem->src, url, 1024);
                            g_strlcat(newitem->src, value, 1024);
                        }
                    } else {
                        g_strlcpy(newitem->src, value, 1024);
                    }

                    g_free(value);
                    newitem->streaming = streaming(newitem->src);
                    // crappy hack, mplayer needs the protocol in lower case, some sites don't
                    if (newitem->streaming) {
                        newitem->src[0] = g_ascii_tolower(newitem->src[0]);
                        newitem->src[1] = g_ascii_tolower(newitem->src[1]);
                        newitem->src[2] = g_ascii_tolower(newitem->src[2]);
                        newitem->src[3] = g_ascii_tolower(newitem->src[3]);
                    }
                    newitem->play = TRUE;
                    if (entry_id != 0) {
                        newitem->id = entry_id;
                    } else {
                        newitem->id = parser_item->id;
                        parser_item->id = -1;
                    }
                    newitem->controlid = parser_item->controlid;
                    if (asx_loop != 0) {
                        newitem->loop = TRUE;
                        newitem->loopcount = asx_loop;
                    }
                    g_strlcpy(newitem->path, parser_item->path, 1024);
                    parser_list = g_list_append(parser_list, newitem);
                }

            }
            i++;
        }
    }
    if (g_ascii_strcasecmp(element_name, "REPEAT") == 0)
        asx_loop--;

    if (g_ascii_strcasecmp(element_name, "ENTRYREF") == 0) {
        entry_id = entry_id + 100;
        while (attribute_names[i] != NULL) {
            if (g_ascii_strcasecmp(attribute_names[i], "HREF") == 0) {
                if (list_find(parser_list, (gchar *) attribute_values[i])
                    == NULL) {
                    parser_item->play = FALSE;
                    newitem = g_new0(ListItem, 1);
                    value = g_strdup(attribute_values[i]);
                    unreplace_amp(value);
                    ptr = g_strrstr(value, "/");
                    if (ptr == NULL) {
                        g_strlcpy(url, parser_item->src, 1024);
                        ptr = g_strrstr(url, "/");
                        if (ptr != NULL) {
                            ptr[1] = (char) NULL;
                            g_strlcpy(newitem->src, url, 1024);
                            g_strlcat(newitem->src, value, 1024);
                        }
                    } else {
                        g_strlcpy(newitem->src, value, 1024);
                    }
                    g_free(value);
                    newitem->streaming = streaming(newitem->src);
                    // crappy hack, mplayer needs the protocol in lower case, some sites don't
                    if (newitem->streaming) {
                        newitem->src[0] = g_ascii_tolower(newitem->src[0]);
                        newitem->src[1] = g_ascii_tolower(newitem->src[1]);
                        newitem->src[2] = g_ascii_tolower(newitem->src[2]);
                        newitem->src[3] = g_ascii_tolower(newitem->src[3]);
                    }
                    newitem->play = TRUE;
                    newitem->id = entry_id;
                    newitem->controlid = parser_item->controlid;
                    if (asx_loop != 0) {
                        newitem->loop = TRUE;
                        newitem->loopcount = asx_loop;
                    }
                    g_strlcpy(newitem->path, parser_item->path, 1024);
                    parser_list = g_list_append(parser_list, newitem);
                }

            }
            i++;
        }
    }
    if (g_ascii_strcasecmp(element_name, "ENTRY") == 0) {
        parser_item->play = 0;
        entry_id = entry_id + 100;
    }
}

void
asx_end_element(GMarkupParseContext * context,
                const gchar * element_name, gpointer user_data, GError ** error)
{
    if (g_ascii_strcasecmp(element_name, "REPEAT") == 0)
        asx_loop++;

}

GList *list_parse_qml(GList * list, ListItem * item)
{
    GMarkupParseContext *context;
    gchar *data;
    gsize datalen;

    printf("Entering list_parse_qml localsize = %i\n", item->localsize);

    if (item->localsize < (16 * 1024)) {
        if (g_file_get_contents(item->local, &data, &datalen, NULL)) {
            parser_list = list;
            parser_item = item;
            asx_loop = 0;
            strip_unicode(data, datalen);
            replace_amp(data);

            context = g_markup_parse_context_new(&qml_parser, (GMarkupParseFlags) 0, data, NULL);
            g_markup_parse_context_parse(context, data, datalen, NULL);
            g_markup_parse_context_free(context);
            parser_item = NULL;
            parser_list = NULL;
        }
        list_dump(list);
    }
    printf("Exiting list_parse_qml\n");
    return list;
}

void
qml_start_element(GMarkupParseContext * context,
                  const gchar * element_name,
                  const gchar ** attribute_names,
                  const gchar ** attribute_values, gpointer user_data, GError ** error)
{
    ListItem *newitem;
    gchar *value;
    gint i = 0;

    if (g_ascii_strcasecmp(element_name, "EMBED") == 0) {
        while (attribute_names[i] != NULL) {
            if (g_ascii_strcasecmp(attribute_names[i], "SRC") == 0) {

                if (list_find(parser_list, (gchar *) attribute_values[i])
                    == NULL) {
                    if (parser_item->play) {
                        parser_item->play = FALSE;
                        newitem = g_new0(ListItem, 1);
                        value = g_strdup(attribute_values[i]);
                        unreplace_amp(value);
                        g_strlcpy(newitem->src, value, 1024);
                        g_free(value);
                        newitem->streaming = streaming(newitem->src);
                        // crappy hack, mplayer needs the protocol in lower case, some sites don't
                        if (newitem->streaming) {
                            newitem->src[0] = g_ascii_tolower(newitem->src[0]);
                            newitem->src[1] = g_ascii_tolower(newitem->src[1]);
                            newitem->src[2] = g_ascii_tolower(newitem->src[2]);
                            newitem->src[3] = g_ascii_tolower(newitem->src[3]);
                        }
                        newitem->play = TRUE;
                        if (entry_id != 0) {
                            newitem->id = entry_id;
                        } else {
                            newitem->id = parser_item->id;
                            parser_item->id = -1;
                        }
                        newitem->controlid = parser_item->controlid;
                        if (asx_loop != 0) {
                            newitem->loop = TRUE;
                            newitem->loopcount = asx_loop;
                        }
                        g_strlcpy(newitem->path, parser_item->path, 1024);
                        parser_list = g_list_append(parser_list, newitem);
                    }
                }
            }
            i++;
        }
    }
}

GList *list_parse_ram(GList * list, ListItem * item)
{
    gchar *data;
    gsize datalen;
    gchar **output;
    gint i;
    gboolean addline;
    ListItem *newitem;
    gchar *value;
    gchar *ptr;
    gchar url[1024];

    printf("Entering list_parse_ram localsize = %i\n", item->localsize);

    if (item->localsize < (16 * 1024)) {
        if (g_file_get_contents(item->local, &data, &datalen, NULL)) {
            if (data != NULL) {
                output = g_strsplit_set(data, "\n\r", 0);
                parser_list = list;
                parser_item = item;
                i = 0;
                while (output != NULL && output[i] != NULL) {
                    // printf("%s\n", output[i]);
                    addline = FALSE;
                    if (g_ascii_strncasecmp(output[i], "rtsp://", strlen("rtsp://")) == 0) {
                        addline = TRUE;
                    }
                    if (g_ascii_strncasecmp(output[i], "http://", strlen("http://")) == 0) {
                        addline = TRUE;
                    }
                    if (addline) {
                        if (list_find(parser_list, (gchar *) output[i])
                            == NULL) {
                            parser_item->play = FALSE;
                            newitem = g_new0(ListItem, 1);
                            value = g_strdup(output[i]);
                            unreplace_amp(value);
                            ptr = g_strrstr(value, "/");
                            if (ptr == NULL) {
                                g_strlcpy(url, parser_item->src, 1024);
                                ptr = g_strrstr(url, "/");
                                if (ptr != NULL) {
                                    ptr[1] = (char) NULL;
                                    g_strlcpy(newitem->src, url, 1024);
                                    g_strlcat(newitem->src, value, 1024);
                                }
                            } else {
                                g_strlcpy(newitem->src, value, 1024);
                            }
                            g_free(value);
                            newitem->streaming = streaming(newitem->src);
                            // crappy hack, mplayer needs the protocol in lower case, some sites don't
                            if (newitem->streaming) {
                                newitem->src[0] = g_ascii_tolower(newitem->src[0]);
                                newitem->src[1] = g_ascii_tolower(newitem->src[1]);
                                newitem->src[2] = g_ascii_tolower(newitem->src[2]);
                                newitem->src[3] = g_ascii_tolower(newitem->src[3]);
                            }
                            newitem->play = TRUE;
                            newitem->id = ++entry_id;
                            newitem->controlid = parser_item->controlid;
                            g_strlcpy(newitem->path, parser_item->path, 1024);
                            parser_list = g_list_append(parser_list, newitem);
                        }
                    }
                    i++;
                }
                g_strfreev(output);
                parser_list = NULL;
                parser_item = NULL;
            }
        }
        list_dump(list);
    }
    printf("Exiting list_parse_ram\n");
    return list;
}
