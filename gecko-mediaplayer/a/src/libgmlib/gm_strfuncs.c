/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * gm_strfuncs.c
 * Copyright (C) Kevin DeKorte 2006 <kdekorte@gmail.com>
 * 
 * gm_strfuncs.c is free software.
 * 
 * You may redistribute it and/or modify it under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 * 
 * gm_strfuncs.c is distributed in the hope that it will be useful,
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

#include "gm_strfuncs.h"

void gm_str_replace_amp(gchar * data)
{

    gchar *pos;

    pos = g_strrstr(data, "&");
    while (pos) {
        pos[0] = '\x01';
        pos = g_strrstr(data, "&");
    }
}

void gm_str_unreplace_amp(gchar * data)
{
    gchar *pos;

    pos = g_strrstr(data, "\x01");
    while (pos) {
        pos[0] = '&';
        pos = g_strrstr(data, "\x01");
    }

}

void gm_str_strip_unicode(gchar * data, gsize len)
{
    gsize i = 0;

    if (data != NULL) {
        for (i = 0; i < len; i++) {
            if (!g_unichar_validate(data[i])) {
                data[i] = ' ';
            }
        }
    }
}

gint gm_str_hms_in_seconds(gchar * timestr)
{
    gchar **split;
    gint ret = 0;

    split = g_strsplit(timestr, ":", 0);

    switch (g_strv_length(split)) {
    case 1:
        ret = g_ascii_strtod(split[0], NULL);
        break;
    case 2:
        ret = g_ascii_strtod(split[0], NULL) * 60 + g_ascii_strtod(split[1], NULL);
        break;
    case 3:
        ret =
            g_ascii_strtod(split[0], NULL) * 3600 + g_ascii_strtod(split[1], NULL) * 60 + g_ascii_strtod(split[2],
                                                                                                         NULL);
        break;
    }

    g_strfreev(split);

    return ret;
}
