/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * gm_parse.c
 * Copyright (C) Kevin DeKorte 2006 <kdekorte@gmail.com>
 * 
 * gm_parse.c is free software.
 * 
 * You may redistribute it and/or modify it under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 * 
 * gm_parse.c is distributed in the hope that it will be useful,
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

#include "gm_parse.h"

void asx_start_element(GMarkupParseContext * context,
                       const gchar * element_name,
                       const gchar ** attribute_names,
                       const gchar ** attribute_values, gpointer user_data, GError ** error);

void asx_end_element(GMarkupParseContext * context, const gchar * element_name, gpointer user_data, GError ** error);

static const GMarkupParser asx_parser = {
    asx_start_element,
    asx_end_element,
    NULL,                       //element_text,
    NULL,
    NULL,
};

typedef struct _ASXCallback {
    GFunc callback;
    gpointer user_data;
} ASXCallback;

void gm_parse_asx(const gchar * asx_data, GFunc callback, gpointer user_data)
{
    GMarkupParseContext *context;
    ASXCallback *asx_callback = g_new0(ASXCallback, 1);
    gchar *data;
    gsize datalen;


    data = g_strdup(asx_data);
    datalen = g_utf8_strlen(data, -1);
    asx_callback->callback = callback;
    asx_callback->user_data = user_data;

    gm_str_strip_unicode(data, datalen);
    gm_str_replace_amp(data);

    context = g_markup_parse_context_new(&asx_parser, (GMarkupParseFlags) 0, asx_callback, NULL);
    g_markup_parse_context_parse(context, data, datalen, NULL);
    g_markup_parse_context_free(context);
    g_free(data);
    g_free(asx_callback);
}

void
asx_start_element(GMarkupParseContext * context,
                  const gchar * element_name,
                  const gchar ** attribute_names, const gchar ** attribute_values, gpointer user_data, GError ** error)
{
    gchar *value;
    gint i = 0;
    ASXCallback *asx_callback = user_data;      // g_markup_parse_context_get_user_data(context);

    if (g_ascii_strcasecmp(element_name, "REF") == 0 || g_ascii_strcasecmp(element_name, "ENTRYREF") == 0) {
        while (attribute_names[i] != NULL) {
            if (g_ascii_strcasecmp(attribute_names[i], "HREF") == 0) {

                value = g_strdup(attribute_values[i]);
                gm_str_unreplace_amp(value);
                value[0] = g_ascii_tolower(value[0]);
                value[1] = g_ascii_tolower(value[1]);
                value[2] = g_ascii_tolower(value[2]);
                value[3] = g_ascii_tolower(value[3]);

                if (asx_callback->callback != NULL) {
                    asx_callback->callback(value, asx_callback->user_data);
                } else {
                    g_free(value);
                }

            }
            i++;
        }
    }


}

void asx_end_element(GMarkupParseContext * context, const gchar * element_name, gpointer user_data, GError ** error)
{
    // nothing at the moment
}


gboolean gm_parse_asx_is_asx(const gchar * uri)
{

    gchar *line = NULL;
    gchar *newline = NULL;
    gboolean ret = FALSE;

#ifdef GIO_ENABLED
    GFile *file;
    GFileInputStream *input;
    GDataInputStream *data;

    gsize length;
    file = g_file_new_for_uri(uri);
    input = g_file_read(file, NULL, NULL);
    data = g_data_input_stream_new((GInputStream *) input);
    if (data != NULL) {
        line = g_data_input_stream_read_line(data, &length, NULL, NULL);
        while (line != NULL) {
            if (line != NULL)
                g_strstrip(line);
            if (g_utf8_strlen(line, -1) == 0) {
                g_free(line);
                line = g_data_input_stream_read_line(data, &length, NULL, NULL);
                continue;
            }
            newline = g_strdup(line);
            if (g_strncasecmp(newline, "<asx", g_utf8_strlen("<asx", -1)) == 0) {
                ret = TRUE;
                g_free(newline);
                break;
            } else if (g_strrstr(newline, "<asx") != NULL) {
                ret = TRUE;
                g_free(newline);
                break;
            }
            g_free(newline);
            g_free(line);
            line = NULL;
        }
        g_input_stream_close((GInputStream *) data, NULL, NULL);
        g_input_stream_close((GInputStream *) input, NULL, NULL);
    }
    g_object_unref(file);

#else
    FILE *fp;
    gchar **parse;
    gchar *file = NULL;

    file = g_strndup(uri, 4);
    if (strcmp(file, "file") != 0) {
        g_free(file);
        return 0;               // FIXME: remote playlists unsupported
    }
    parse = g_strsplit(uri, "/", 3);
    fp = fopen(parse[2], "r");
    g_strfreev(parse);

    if (fp != NULL) {
        if (line != NULL)
            g_free(line);
        line = g_new0(gchar, 1024);
        if (line != NULL) {
            line = fgets(line, 1024, fp);
            if (line != NULL) {
                if (line != NULL)
                    g_strstrip(line);
                if (g_utf8_strlen(line, -1) == 0) {
                    // do nothing;
                } else {
                    //printf("line = %s\n", line);
                    newline = g_strdup(line);
                    if (g_strncasecmp(newline, "<asx", g_utf8_strlen("<asx", -1)) == 0) {
                        //printf("asx\n");
                        ret = TRUE;
                        g_free(newline);
                    } else if (g_strrstr(newline, "<asx") != NULL) {
                        //printf("asx\n");
                        ret = TRUE;
                        g_free(newline);
                    }
                    g_free(newline);
                }
                g_free(line);
                line = NULL;
            }
            g_free(line);
            line = NULL;
        }
    }
    g_free(file);
    fclose(fp);
#endif
    return ret;
}
