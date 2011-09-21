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

#include "plugin.h"
#include "plugin_list.h"
#include "plugin_setup.h"

// void new_instance(CPlugin * instance, nsPluginCreateData * parameters)
void new_instance(CPlugin * instance, int16_t argc, char *argn[], char *argv[])
{

    int16_t i;
    guint j;
    gint newwindow = 0;
    gint loop = 0;
    gboolean autohref = FALSE;
    gboolean force_streaming = FALSE;
    ListItem *item = NULL;
    ListItem *src = NULL;
    ListItem *href = NULL;
    ListItem *qtsrc = NULL;
    gchar *arg[10];
    GRand *rand;
    gchar *tmp;
    //gchar *url;
    gchar **parse;
    gint width = 0, height = 0;
    GError *error;
    NPError nperror;
    guint32 supportsWindowless = FALSE; // NPBool + padding
    gchar *app_name;

    if (instance->mode == NP_EMBED) {
        for (i = 0; i < argc; i++) {

            if (argn[i] == NULL)
                continue;

            printf("ARG: %s = %s\n", argn[i], argv[i]);

            if (g_ascii_strcasecmp(argn[i], "name") == 0) {
                instance->name = g_strdup(argv[i]);
            }

            if (g_ascii_strcasecmp(argn[i], "id") == 0) {
                instance->id = g_strdup(argv[i]);
            }

            if (g_ascii_strcasecmp(argn[i], "console") == 0) {
                instance->console = g_strdup(argv[i]);
                if (instance->console != NULL) {
                    for (j = 0; j < strlen(instance->console); j++) {
                        if (!g_ascii_isalnum(instance->console[j])) {
                            instance->console[j] = 'a';
                        }
                    }
                }
            }

            if (g_ascii_strcasecmp(argn[i], "controls") == 0) {
                instance->controls = g_strdup(argv[i]);
            }

            if (g_ascii_strcasecmp(argn[i], "showcontrols") == 0) {
                if (g_ascii_strcasecmp(argv[i], "true") == 0
                    || g_ascii_strcasecmp(argv[i], "yes") == 0
                    || g_ascii_strcasecmp(argv[i], "1") == 0) {
                    instance->show_controls = 1;
                } else {
                    instance->show_controls = 0;
                }
            }

            if (g_ascii_strcasecmp(argn[i], "width") == 0) {
                sscanf(argv[i], "%i", &width);
            }

            if (g_ascii_strcasecmp(argn[i], "height") == 0) {
                sscanf(argv[i], "%i", &height);
            }

            if (g_ascii_strcasecmp(argn[i], "style") == 0) {
                tmp = g_strrstr(argv[i], "width:");
                sscanf(tmp + strlen("width:"), "%i", &width);
                tmp = g_strrstr(argv[i], "height:");
                sscanf(tmp + strlen("height:"), "%i", &height);
            }


            if (g_ascii_strcasecmp(argn[i], "src") == 0 || g_ascii_strcasecmp(argn[i], "url") == 0) {
                item = g_new0(ListItem, 1);
                if (g_strrstr(argv[i], "XXmovies.apple.com")) { // tmp disabled, to reenable remove XX
                    tmp = g_strrstr(argv[i], "movies.");
                    if (tmp != NULL && strlen(tmp) > strlen("movies.")) {
                        tmp = tmp + strlen("movies.");
                        g_snprintf(item->src, 4096, "http://www.%s", tmp);
                    } else {
                        g_strlcpy(item->src, argv[i], 4096);
                    }
                } else {
                    g_strlcpy(item->src, argv[i], 4096);
                }
                // printf("Item src = %s\n",item->src);
                item->streaming = streaming(item->src);
                item->play = TRUE;
                item->id = instance->nextid++;
                instance->playlist = g_list_append(instance->playlist, item);
                src = item;
            }

            if (g_ascii_strcasecmp(argn[i], "filename") == 0) {
                item = g_new0(ListItem, 1);
                g_strlcpy(item->src, argv[i], 4096);
                // printf("Item src = %s\n",item->src);
                item->streaming = streaming(item->src);
                item->play = TRUE;
                item->id = instance->nextid++;
                instance->playlist = g_list_append(instance->playlist, item);
                src = item;
            }

            if (g_ascii_strcasecmp(argn[i], "href") == 0) {
                item = g_new0(ListItem, 1);
                if (g_strrstr(argv[i], "XXmovies.apple.com")) {
                    tmp = g_strrstr(argv[i], "movies.");
                    if (tmp != NULL && strlen(tmp) > strlen("movies.")) {
                        tmp = tmp + strlen("movies.");
                        g_snprintf(item->src, 4096, "http://www.%s", tmp);
                    } else {
                        g_strlcpy(item->src, argv[i], 4096);
                    }
                } else {
                    g_strlcpy(item->src, argv[i], 4096);
                }
                // printf("Item href = %s\n",item->src);
                item->streaming = streaming(item->src);
                item->play = FALSE;
                item->id = instance->nextid++;
                instance->playlist = g_list_append(instance->playlist, item);
                href = item;
            }

            if (g_ascii_strcasecmp(argn[i], "qtsrc") == 0) {
                item = g_new0(ListItem, 1);
                g_strlcpy(item->src, argv[i], 4096);
                item->streaming = streaming(item->src);
                item->play = TRUE;
                item->id = instance->nextid++;
                instance->playlist = g_list_append(instance->playlist, item);
                qtsrc = item;
            }

            if (g_ascii_strcasecmp(argn[i], "file") == 0) {
                item = g_new0(ListItem, 1);
                g_strlcpy(item->local, argv[i], 1024);
                // printf("Item href = %s\n",item->src);
                item->streaming = streaming(item->src);
                item->play = TRUE;
                item->id = instance->nextid++;
                instance->playlist = g_list_append(instance->playlist, item);
                src = item;
            }

            if (g_ascii_strcasecmp(argn[i], "target") == 0) {
                if (g_ascii_strcasecmp(argv[i], "quicktimeplayer") == 0) {
                    newwindow = TRUE;
                } else {
                    item = g_new0(ListItem, 1);
                    g_strlcpy(item->src, argv[i], 4096);
                    // printf("Item href = %s\n",item->src);
                    item->streaming = streaming(item->src);
                    item->play = FALSE;
                    item->id = instance->nextid++;
                    instance->playlist = g_list_append(instance->playlist, item);
                    src = item;
                    instance->show_controls = 0;
                }

            }

            if (g_ascii_strcasecmp(argn[i], "hidden") == 0) {
                if (g_ascii_strcasecmp(argv[i], "true") == 0
                    || g_ascii_strcasecmp(argv[i], "yes") == 0
                    || g_ascii_strcasecmp(argv[i], "1") == 0) {
                    instance->hidden = TRUE;
                } else {
                    instance->hidden = FALSE;
                }
            }

            if (g_ascii_strcasecmp(argn[i], "tv_device") == 0) {
                instance->tv_device = g_strdup_printf("%s", argv[i]);
            }

            if (g_ascii_strcasecmp(argn[i], "tv_driver") == 0) {
                instance->tv_driver = g_strdup_printf("%s", argv[i]);
            }

            if (g_ascii_strcasecmp(argn[i], "tv_input") == 0) {
                instance->tv_input = g_strdup_printf("%s", argv[i]);
            }

            if (g_ascii_strcasecmp(argn[i], "tv_width") == 0) {
                sscanf(argv[i], "%i", &(instance->tv_width));
            }

            if (g_ascii_strcasecmp(argn[i], "tv_height") == 0) {
                sscanf(argv[i], "%i", &(instance->tv_height));
            }

            if (g_ascii_strcasecmp(argn[i], "autohref") == 0) {
                if (g_ascii_strcasecmp(argv[i], "true") == 0
                    || g_ascii_strcasecmp(argv[i], "yes") == 0
                    || g_ascii_strcasecmp(argv[i], "autohref") == 0
                    || g_ascii_strcasecmp(argv[i], "1") == 0) {
                    autohref = TRUE;
                } else {
                    autohref = FALSE;
                }
            }

            if (g_ascii_strcasecmp(argn[i], "data") == 0) {
                // autohref = TRUE;
            }

            if ((g_ascii_strcasecmp(argn[i], "autoplay") == 0)
                || (g_ascii_strcasecmp(argn[i], "autostart") == 0)) {
                if (g_ascii_strcasecmp(argv[i], "false") == 0
                    || g_ascii_strcasecmp(argv[i], "no") == 0
                    || g_ascii_strcasecmp(argv[i], "0") == 0) {
                    instance->autostart = 0;
                } else {
                    instance->autostart = 1;
                }
            }

            if (g_ascii_strcasecmp(argn[i], "enablecontextmenu") == 0) {
                if (g_ascii_strcasecmp(argv[i], "false") == 0
                    || g_ascii_strcasecmp(argv[i], "no") == 0
                    || g_ascii_strcasecmp(argv[i], "0") == 0) {
                    instance->disable_context_menu = TRUE;
                } else {
                    instance->disable_context_menu = FALSE;
                }
            }

            if (g_ascii_strcasecmp(argn[i], "enablefullscreen") == 0) {
                if (g_ascii_strcasecmp(argv[i], "false") == 0
                    || g_ascii_strcasecmp(argv[i], "no") == 0
                    || g_ascii_strcasecmp(argv[i], "0") == 0) {
                    instance->disable_fullscreen = TRUE;
                } else {
                    instance->disable_fullscreen = FALSE;
                }
            }

            if ((g_ascii_strcasecmp(argn[i], "loop") == 0)
                || (g_ascii_strcasecmp(argn[i], "autorewind")
                    == 0)
                || (g_ascii_strcasecmp(argn[i], "repeat") == 0)) {

                if (g_ascii_strcasecmp(argv[i], "true") == 0
                    || g_ascii_strcasecmp(argv[i], "yes") == 0
                    || g_ascii_strcasecmp(argv[i], "infinite") == 0) {
                    loop = -1;
                } else if (g_ascii_isdigit((int) *(argv[i]))) {
                    sscanf(argv[i], "%i", &loop);
                } else {
                    loop = 0;   // loop disabled
                }
            }

            if (g_ascii_strcasecmp(argn[i], "nocache") == 0
                || g_ascii_strcasecmp(argn[i], "qtsrcdontusebrowser") == 0) {
                if (g_ascii_strcasecmp(argv[i], "true") == 0
                    || g_ascii_strcasecmp(argv[i], "yes") == 0
                    || g_ascii_strcasecmp(argv[i], "1") == 0
                    || g_ascii_strcasecmp(argv[i], "") == 0 || argv[i] == NULL) {
                    force_streaming = TRUE;
                } else {
                    force_streaming = FALSE;
                }
            }

            if (g_ascii_strcasecmp(argn[i], "postdomevents") == 0) {
                if (g_ascii_strcasecmp(argv[i], "true") == 0
                    || g_ascii_strcasecmp(argv[i], "yes") == 0
                    || g_ascii_strcasecmp(argv[i], "1") == 0) {
                    instance->post_dom_events = TRUE;
                } else {
                    instance->post_dom_events = FALSE;
                }
            }
            if (g_ascii_strcasecmp(argn[i], "onmediacomplete") == 0
                || g_ascii_strcasecmp(argn[i], "onendofstream") == 0) {
                if (g_ascii_strncasecmp(argv[i], "javascript:", 11) == 0) {
                    instance->event_mediacomplete = g_strdup_printf("%s", argv[i]);
                } else {
                    instance->event_mediacomplete = g_strdup_printf("javascript:%s", argv[i]);
                }
            }

            if (g_ascii_strcasecmp(argn[i], "ondestroy") == 0) {
                if (g_ascii_strncasecmp(argv[i], "javascript:", 11) == 0) {
                    instance->event_destroy = g_strdup_printf("%s", argv[i]);
                } else {
                    instance->event_destroy = g_strdup_printf("javascript:%s", argv[i]);
                }
            }

            if (g_ascii_strcasecmp(argn[i], "onmouseover") == 0) {
                if (g_ascii_strncasecmp(argv[i], "javascript:", 11) == 0) {
                    instance->event_enterwindow = g_strdup_printf("%s", argv[i]);
                } else {
                    instance->event_enterwindow = g_strdup_printf("javascript:%s", argv[i]);
                }
            }

            if (g_ascii_strcasecmp(argn[i], "onmouseout") == 0) {
                if (g_ascii_strncasecmp(argv[i], "javascript:", 11) == 0) {
                    instance->event_leavewindow = g_strdup_printf("%s", argv[i]);
                } else {
                    instance->event_leavewindow = g_strdup_printf("javascript:%s", argv[i]);
                }
            }

            if (g_ascii_strcasecmp(argn[i], "onclick") == 0) {
                if (g_ascii_strncasecmp(argv[i], "javascript:", 11) == 0) {
                    instance->event_mouseclicked = g_strdup_printf("%s", argv[i]);
                } else {
                    instance->event_mouseclicked = g_strdup_printf("javascript:%s", argv[i]);
                }
            }

            if (g_ascii_strcasecmp(argn[i], "onmousedown") == 0) {
                parse = g_strsplit(argv[i], "(", 0);
                if (g_ascii_strncasecmp(parse[0], "javascript:", 11) == 0) {
                    instance->event_mousedown = g_strdup_printf("%s", parse[0]);
                } else {
                    instance->event_mousedown = g_strdup_printf("javascript:%s", parse[0]);
                }
                g_strfreev(parse);
            }

            if (g_ascii_strcasecmp(argn[i], "onmouseup") == 0) {
                parse = g_strsplit(argv[i], "(", 0);
                if (g_ascii_strncasecmp(parse[0], "javascript:", 11) == 0) {
                    instance->event_mouseup = g_strdup_printf("%s", parse[0]);
                } else {
                    instance->event_mouseup = g_strdup_printf("javascript:%s", parse[0]);
                }
                g_strfreev(parse);
            }

            if (g_ascii_strcasecmp(argn[i], "debug") == 0) {
                if (g_ascii_strcasecmp(argv[i], "true") == 0
                    || g_ascii_strcasecmp(argv[i], "yes") == 0
                    || g_ascii_strcasecmp(argv[i], "1") == 0) {
                    instance->debug = TRUE;

                } else {
                    instance->debug = FALSE;
                }
            }

            if (g_ascii_strcasecmp(argn[i], "windowless") == 0) {
                if (g_ascii_strcasecmp(argv[i], "true") == 0
                    || g_ascii_strcasecmp(argv[i], "yes") == 0
                    || g_ascii_strcasecmp(argv[i], "1") == 0) {
                    instance->windowless = TRUE;
                } else {
                    instance->windowless = FALSE;
                }
            }

            if (g_ascii_strncasecmp(argn[i], "qtnext", 6) == 0) {
                parse = g_strsplit_set(argv[i], "<>", 0);
                if (parse[1] != NULL) {
                    item = g_new0(ListItem, 1);
                    tmp = g_strrstr(src->src, "/");
                    if (tmp) {
                        g_strlcpy(item->src, src->src, 1024);
                        tmp = g_strrstr(item->src, "/");
                        tmp[1] = '\0';
                        g_strlcat(item->src, parse[1], 4096);
                    } else {
                        g_strlcpy(item->src, parse[1], 4096);
                    }
                    item->streaming = streaming(item->src);
                    item->play = TRUE;
                    item->id = instance->nextid++;
                    instance->playlist = g_list_append(instance->playlist, item);
                }
                g_strfreev(parse);
            }

        };
    } else {
        // printf("Non-Embed Mode\n");
        for (i = 0; i < argc; i++) {

            if (argn[i] == NULL)
                continue;

            printf("ARG: %s = %s\n", argn[i], argv[i]);

            if (g_ascii_strcasecmp(argn[i], "src") == 0) {
                item = g_new0(ListItem, 1);
                if (g_strrstr(argv[i], "XXmovies.apple.com")) { // tmp disabled, to reenable remove XX
                    tmp = g_strrstr(argv[i], "movies.");
                    if (tmp != NULL && strlen(tmp) > strlen("movies.")) {
                        tmp = tmp + strlen("movies.");
                        g_snprintf(item->src, 4096, "http://www.%s", tmp);
                    } else {
                        g_strlcpy(item->src, argv[i], 4096);
                    }
                } else {
                    g_strlcpy(item->src, argv[i], 4096);
                }
                item->streaming = streaming(item->src);
                printf("this should match %s\n", item->src);
                item->play = TRUE;
                item->id = instance->nextid++;
                instance->playlist = g_list_append(instance->playlist, item);
                src = item;
            }

        }
    }

    nperror = NPN_GetValue(instance->mInstance, (NPNVariable) 17 /* NPNVSupportsWindowless */ ,
                           &supportsWindowless);
    supportsWindowless = (nperror == NPERR_NO_ERROR) && supportsWindowless;
    if (instance->windowless) {
        if (supportsWindowless) {
            // NPN_SetValue (instance->mInstance, NPPVpluginWindowBool, (void *) FALSE);
            // NPN_SetValue (instance->mInstance, NPPVpluginTransparentBool, (void *) TRUE);
        } else {
            instance->windowless = FALSE;
        }
    }

    if (qtsrc != NULL) {
        src->play = FALSE;
    }

    if (src != NULL) {
        if (loop != 0) {
            src->loop = TRUE;
            src->loopcount = loop;
        } else {
            loop = FALSE;
        }
    } else {
        // Having this event fire creates several instances of gmp at some websites
        // postPlayStateChange(instance->mInstance, STATE_TRANSITIONING);
    }

    // link up src to href objects by id
    if (href != NULL && src != NULL) {
        src->hrefid = href->id;
    }
    // if target is set, set it on the href
    if (href != NULL) {
        href->newwindow = newwindow;
    }

    if (item == NULL) {
        item = g_new0(ListItem, 1);
        item->play = FALSE;
        item->id = instance->nextid++;
        item->streaming = FALSE;
    }
#ifdef DISABLE_CACHE
    item->streaming = TRUE;
#else
    if (force_streaming) {
        item->streaming = TRUE;
        src->streaming = TRUE;
        if (qtsrc)
            qtsrc->streaming = TRUE;
    }

    list_qualify_url(instance->playlist, instance->page_url);


//    if (g_ascii_strcasecmp(instance->mimetype,"video/x-flv") == 0) {
//      item->streaming = TRUE;
//    }
#endif
    // list_dump(instance->playlist);

    if (instance->hidden == TRUE || ((width == 0 || height == 0) && instance->mode != NP_FULL)) {
        if (item->streaming) {
            open_location(instance, item, FALSE);
            item->requested = 1;
        } else {
            item->requested = 1;
            instance->GetURLNotify(instance->mInstance, item->src, NULL, item);
        }
    }

    if (autohref == TRUE) {
        src->play = FALSE;
        href->play = TRUE;
        i = 0;
        // generate a random controlid
        rand = g_rand_new();
        href->controlid = g_rand_int_range(rand, 0, 65535);
        // printf("autohref controlid = %i\n", href->controlid);
        g_rand_free(rand);
        tmp = g_strdup_printf("/control/%i", href->controlid);
        g_strlcpy(href->path, tmp, 1024);
        g_free(tmp);

        //list_dump(instance->playlist);
        app_name = NULL;
        if (instance->player_backend != NULL) {
            app_name = g_find_program_in_path(instance->player_backend);
        }
        if (app_name == NULL) {
            app_name = g_find_program_in_path("gnome-mplayer");
            if (app_name == NULL)
                app_name = g_find_program_in_path("gnome-mplayer-minimal");
        }

        arg[i++] = g_strdup(app_name);
        arg[i++] = g_strdup_printf("--controlid=%i", href->controlid);
        arg[i] = NULL;
        error = NULL;
        if (g_spawn_async(NULL, arg, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, NULL, &error) == FALSE) {
            printf("Unable to launch: %s\n", error->message);
            g_error_free(error);
            error = NULL;
        }
        instance->GetURLNotify(instance->mInstance, href->src, NULL, href);
        g_free(app_name);
    }


}

gboolean streaming(gchar * url)
{
    gboolean ret = FALSE;
    char *p;

    if (g_ascii_strncasecmp(url, "mms://", 6) == 0)
        ret = TRUE;

    if (g_ascii_strncasecmp(url, "mmst://", 7) == 0)
        ret = TRUE;

    if (g_ascii_strncasecmp(url, "mmsu://", 7) == 0)
        ret = TRUE;

    if (g_ascii_strncasecmp(url, "rtsp://", 7) == 0)
        ret = TRUE;

    if (g_ascii_strncasecmp(url, "tv://", 5) == 0)
        ret = TRUE;

    if (g_ascii_strncasecmp(url, "dvd://", 6) == 0)
        ret = TRUE;

    if (g_ascii_strncasecmp(url, "udp://", 6) == 0)
        ret = TRUE;

    if (g_strrstr(url, ".m3u") != NULL)
        ret = TRUE;

    if (g_strrstr(url, "stream") != NULL) {
        if (g_strrstr(url, "http://") == NULL)
            ret = TRUE;
    }

    if (g_strrstr(url, "MSWMExt=.asf") != NULL)
        ret = TRUE;

    if (ret == FALSE && g_ascii_strncasecmp(url, "file://", 7) == 0) {
        p = g_filename_from_uri(url, NULL, NULL);
        if (p != NULL) {
            if (g_file_test(p, G_FILE_TEST_EXISTS)) {
                g_strlcpy(url, p, 1024);
                ret = TRUE;
            }
            g_free(p);
        }
    }
    return ret;
}
