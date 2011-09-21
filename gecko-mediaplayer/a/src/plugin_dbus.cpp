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

#include "plugin_dbus.h"

static DBusHandlerResult filter_func(DBusConnection * connection,
                                     DBusMessage * message, void *user_data)
{

    const gchar *sender;
    const gchar *destination;
    int message_type;
    gchar *s = NULL;
    DBusError error;
    // DBusMessage *reply_message;
    gchar *path;
    CPlugin *instance;
    ListItem *item = NULL;
    gchar *arg[10];
    gint i;
    GRand *rand;
    gchar *tmp;
    GError *gerror;
    gchar *app_name;

    message_type = dbus_message_get_type(message);
    sender = dbus_message_get_sender(message);
    destination = dbus_message_get_destination(message);

    /*
       printf("path=%s; interface=%s; member=%s; data=%s\n",
       dbus_message_get_path(message),
       dbus_message_get_interface(message), dbus_message_get_member(message), s);
     */

    instance = (CPlugin *) user_data;
    path = instance->path;

    if (dbus_message_get_path(message) != NULL
        && is_valid_path(instance, dbus_message_get_path(message))) {

        //printf("Path matched %s\n", dbus_message_get_path(message));
        if (message_type == DBUS_MESSAGE_TYPE_SIGNAL) {
            if (g_ascii_strcasecmp(dbus_message_get_member(message), "Ready") == 0) {
                dbus_error_init(&error);
                if (dbus_message_get_args(message, &error, DBUS_TYPE_INT32, &i, DBUS_TYPE_INVALID)) {
                    item = list_find_by_controlid(instance->playlist, i);
                    if (item != NULL) {
                        list_mark_controlid_ready(instance->playlist, i);
                    } else {
                        // printf("Control id not found\n");
                    }
                } else {
                    dbus_error_free(&error);
                }

                instance->playerready = TRUE;
                // disable this as WMP doesn't do this
                // postPlayStateChange(instance->mInstance, STATE_READY);

                if (g_strrstr(instance->mimetype, "audio") != NULL) {
                    instance->cache_size =
                        request_int_value(instance, item, "GetPluginAudioCacheSize");
                } else {
                    instance->cache_size =
                        request_int_value(instance, item, "GetPluginVideoCacheSize");
                }
                if (instance->cache_size == 0) {
                    item->streaming = 1;
                }
                return DBUS_HANDLER_RESULT_HANDLED;
            }
            if (g_ascii_strcasecmp(dbus_message_get_member(message), "Cancel") == 0) {
                dbus_error_init(&error);
                if (dbus_message_get_args(message, &error, DBUS_TYPE_INT32, &i, DBUS_TYPE_INVALID)) {
                    list_mark_controlid_cancelled(instance->playlist, i, TRUE);
                } else {
                    dbus_error_free(&error);
                }
                return DBUS_HANDLER_RESULT_HANDLED;
            }
            if (g_ascii_strcasecmp(dbus_message_get_member(message), "ReloadPlugins") == 0) {
                NPN_ReloadPlugins(FALSE);
                return DBUS_HANDLER_RESULT_HANDLED;
            }

            if (g_ascii_strcasecmp(dbus_message_get_member(message), "ListDump") == 0) {

                printf("playlist:\n");
                list_dump(instance->playlist);

                return DBUS_HANDLER_RESULT_HANDLED;
            }

            if (g_ascii_strcasecmp(dbus_message_get_member(message), "RequestById") == 0) {
                dbus_error_init(&error);
                if (dbus_message_get_args(message, &error, DBUS_TYPE_STRING, &s, DBUS_TYPE_INVALID)) {
                    printf("Got id %s\n", s);
                    item = list_find_by_id(instance->playlist, (gint) g_strtod(s, NULL));
                    if (item != NULL) {
                        item->play = TRUE;
                        item->cancelled = FALSE;
                        item->retrieved = FALSE;
                        printf("id %s has url of %s\n", s, item->src);
                        printf("id %s has newwindow = %i\n", s, item->newwindow);
                        if (item->newwindow == 0) {
                            send_signal_with_boolean(instance, item, "SetShowControls", TRUE);
                            if (item->streaming) {
                                send_signal_with_string(instance, item, "Open", item->src);
                            } else {
                                instance->GetURLNotify(instance->mInstance, item->src, NULL, item);
                            }
                        } else {
                            i = 0;
                            // generate a random controlid
                            rand = g_rand_new();
                            item->controlid = g_rand_int_range(rand, 0, 65535);
                            g_rand_free(rand);
                            tmp = g_strdup_printf("/control/%i", item->controlid);
                            g_strlcpy(item->path, tmp, 1024);
                            g_free(tmp);

                            // list_dump(instance->playlist);
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
                            arg[i++] = g_strdup_printf("--controlid=%i", item->controlid);
                            arg[i] = NULL;
                            gerror = NULL;
                            if (g_spawn_async(NULL, arg, NULL,
                                              G_SPAWN_SEARCH_PATH, NULL, NULL, NULL,
                                              &gerror) == FALSE) {
                                printf("Unable to launch %s: %s\n", app_name, gerror->message);
                                g_error_free(gerror);
                                gerror = NULL;
                            }
                            g_free(app_name);
                            printf("requesting %s \n", item->src);
                            instance->GetURLNotify(instance->mInstance, item->src, NULL, item);
                        }
                        instance->lastopened->played = TRUE;
                        item->requested = TRUE;
                        instance->lastopened = item;
                    }
                } else {
                    dbus_error_free(&error);
                }
                //list_dump(instance->playlist);
                return DBUS_HANDLER_RESULT_HANDLED;
            }

            if (g_ascii_strcasecmp(dbus_message_get_member(message), "Next") == 0) {

                postPlayStateChange(instance->mInstance, STATE_TRANSITIONING);

                if (instance->lastopened != NULL && instance->lastopened->loop == FALSE) {
                    list_mark_id_played(instance->playlist, instance->lastopened->id);
                    instance->lastopened->played = TRUE;
                    item = list_find_next_playable(instance->playlist);
                } else {
                    if (instance->lastopened != NULL && instance->lastopened->loop == TRUE) {
                        if (instance->lastopened->loopcount < 0) {
                            item = instance->lastopened;
                            item->played = FALSE;
                            item->opened = FALSE;
                        } else if (instance->lastopened->loopcount > 0) {
                            instance->lastopened->loopcount--;
                            item = instance->lastopened;
                            item->played = FALSE;
                            item->opened = FALSE;
                        } else {
                            // listcount = 0
                            instance->lastopened->loop = FALSE;
                            item = list_find_next_playable(instance->playlist);
                        }
                    }
                }

                //printf("next item src = %s\n", item->src);
                //printf("next item local = %s\n", item->local);
                //printf("next item streaming = %i\n", item->streaming);

                if (item != NULL) {
                    /*
                       if (item->newwindow == 0) {
                       open_location(instance, item, TRUE);
                       } else {
                       i = 0;
                       arg[i++] = g_strdup("gnome-mplayer");
                       arg[i++] =
                       g_strdup_printf("--controlid=%i",
                       instance->controlid);
                       arg[i++] = g_strdup(item->src);
                       arg[i] = NULL;
                       g_spawn_async(NULL, arg, NULL,
                       G_SPAWN_SEARCH_PATH,
                       NULL, NULL, NULL, NULL);
                       item->opened = TRUE;
                       instance->lastopened = item;
                       }
                     */
                    if (!item->streaming) {
                        item->controlid = instance->lastopened->controlid;
                        g_strlcpy(item->path, instance->lastopened->path, 1024);
                        item->playerready = instance->lastopened->playerready;
                        item->newwindow = instance->lastopened->newwindow;
                        item->cancelled = FALSE;
                        if (item->retrieved) {
                            open_location(instance, item, TRUE);
                        } else {
                            instance->GetURLNotify(instance->mInstance, item->src, NULL, item);
                        }
                    } else {
                        open_location(instance, item, FALSE);
                    }
                }
                return DBUS_HANDLER_RESULT_HANDLED;
            }
            if (g_ascii_strcasecmp(dbus_message_get_member(message), "Event") == 0) {
                dbus_error_init(&error);
                if (dbus_message_get_args
                    (message, &error, DBUS_TYPE_STRING, &s, DBUS_TYPE_INT32, &i,
                     DBUS_TYPE_INVALID)) {
                    // uncomment for event debugging
                    // printf("Event: %s button %i\n",s,i);

                    if (g_ascii_strcasecmp(s, "MediaComplete") == 0) {
                        if (instance->event_mediacomplete != NULL) {
                            NPN_GetURL(instance->mInstance, instance->event_mediacomplete, NULL);
                        }
                        postPlayStateChange(instance->mInstance, STATE_MEDIAENDED);
                    }
                    if (g_ascii_strcasecmp(s, "MouseClicked") == 0) {
                        if (instance->event_mouseclicked != NULL) {
                            NPN_GetURL(instance->mInstance, instance->event_mouseclicked, NULL);
                        }
                    }
                    if (g_ascii_strcasecmp(s, "EnterWindow") == 0) {
                        if (instance->event_enterwindow != NULL) {
                            NPN_GetURL(instance->mInstance, instance->event_enterwindow, NULL);
                        }
                    }
                    if (g_ascii_strcasecmp(s, "LeaveWindow") == 0) {
                        if (instance->event_leavewindow != NULL) {
                            NPN_GetURL(instance->mInstance, instance->event_leavewindow, NULL);
                        }
                    }
                    if (g_ascii_strcasecmp(s, "MouseDown") == 0) {
                        if (instance->event_mousedown != NULL) {
                            tmp = g_strdup_printf("%s(%i);", instance->event_mousedown, i);
                            NPN_GetURL(instance->mInstance, tmp, NULL);
                            g_free(tmp);
                        }
                    }
                    if (g_ascii_strcasecmp(s, "MouseUp") == 0) {
                        if (instance->event_mouseup != NULL) {
                            tmp = g_strdup_printf("%s(%i);", instance->event_mouseup, i);
                            NPN_GetURL(instance->mInstance, tmp, NULL);
                            g_free(tmp);
                        }
                    }
                    if (g_ascii_strcasecmp(s, "TimeChanged") == 0) {
                        if (instance->post_dom_events && instance->id != NULL) {
                            postDOMEvent(instance->mInstance, instance->id, "qt_timechanged");
                        }
                    }
                    if (g_ascii_strcasecmp(s, "Ended") == 0) {
                        if (instance->post_dom_events && instance->id != NULL) {
                            postDOMEvent(instance->mInstance, instance->id, "qt_ended");
                        }
                    }
                    if (g_ascii_strcasecmp(s, "MediaStopped") == 0) {
                        if (instance->post_dom_events && instance->id != NULL) {
                            postDOMEvent(instance->mInstance, instance->id, "qt_ended");
                        }
                        postPlayStateChange(instance->mInstance, STATE_STOPPED);
                    }
                    if (g_ascii_strcasecmp(s, "MediaPlaying") == 0) {
                        if (instance->post_dom_events && instance->id != NULL) {
                            postDOMEvent(instance->mInstance, instance->id, "qt_play");
                        }
                        postPlayStateChange(instance->mInstance, STATE_PLAYING);
                    }
                    if (g_ascii_strcasecmp(s, "MediaPaused") == 0) {
                        if (instance->post_dom_events && instance->id != NULL) {
                            postDOMEvent(instance->mInstance, instance->id, "qt_pause");
                        }
                        postPlayStateChange(instance->mInstance, STATE_PAUSED);
                    }
                }
            }
        }
    } else {
        // printf("path didn't match path = %s\n", path);
    }
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

DBusConnection *dbus_hookup(CPlugin * instance)
{
    DBusConnection *connection;
    DBusError dberror;
    DBusBusType type = DBUS_BUS_SESSION;

    dbus_error_init(&dberror);
    connection = dbus_bus_get_private(type, &dberror);

    dbus_connection_setup_with_g_main(connection, NULL);

    dbus_bus_add_match(connection, "type='signal',interface='com.gecko.mediaplayer'", NULL);
    dbus_connection_add_filter(connection, filter_func, instance, NULL);

    printf("DBUS connection created\nListening to path %s\n", instance->path);

    return connection;
}

DBusConnection *dbus_unhook(DBusConnection * connection, CPlugin * instance)
{

    dbus_connection_flush(connection);
    dbus_connection_remove_filter(connection, filter_func, instance);
    dbus_connection_close(connection);
    dbus_connection_unref(connection);

    return NULL;
}

void open_location(CPlugin * instance, ListItem * item, gboolean uselocal)
{
    DBusMessage *message;
    const char *file;
    const char *id;
    char *path;
    GError *error = NULL;
    gchar *argvn[255];
    gint arg = 0;
    gint ok;
    gchar *app_name;
    gchar *nullstr;

    //list_dump(instance->playlist);
    //printf("Opening %s to connection %p\n",file, instance->connection);
    if (item == NULL || instance == NULL)
        return;

    postPlayStateChange(instance->mInstance, STATE_PLAYING);

    if (!(instance->player_launched)) {
        if (!item->opened) {
            if (uselocal && strlen(item->local) > 0) {
                file = g_strdup(item->local);
            } else {
                file = g_strdup(item->src);
            }

            //printf("launching gnome-mplayer from Open with id = %i\n",instance->controlid);
            app_name = NULL;
            if (instance->player_backend != NULL) {
                app_name = g_find_program_in_path(instance->player_backend);
            }
            if (app_name == NULL) {
                app_name = g_find_program_in_path("gnome-mplayer");
                if (app_name == NULL)
                    app_name = g_find_program_in_path("gnome-mplayer-minimal");
            }
            argvn[arg++] = g_strdup(app_name);
            argvn[arg++] = g_strdup_printf("--window=-1");
            argvn[arg++] = g_strdup_printf("--controlid=%i", instance->controlid);
            argvn[arg++] = g_strdup_printf("--autostart=%i", instance->autostart);
            if (instance->disable_context_menu == TRUE)
                argvn[arg++] = g_strdup_printf("--disablecontextmenu");
            if (instance->debug == TRUE)
                argvn[arg++] = g_strdup_printf("--verbose");

            argvn[arg++] = g_strdup_printf("%s", file);
            argvn[arg] = NULL;
            instance->playerready = FALSE;
            ok = g_spawn_async(NULL, argvn, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, NULL, &error);

            if (ok)
                instance->player_launched = TRUE;
            item->opened = TRUE;
            instance->lastopened = item;
            g_free(app_name);
        }

        return;

    } else {
        //printf("waiting for ready\n");
        while (!(instance->playerready)) {
            g_main_context_iteration(NULL, FALSE);
        }
        //printf("got player, waiting for controlid %i\n",item->controlid);
        if (item->controlid != 0) {
            while (!(item->playerready) && !item->cancelled) {
                g_main_context_iteration(NULL, FALSE);
            }
        }
        //printf("ready!\n");
    }

    // printf("item %s is opened == %i\n",item->src, item->opened);

    if (!item->opened) {
        if (uselocal && strlen(item->local) > 0) {
            file = g_strdup(item->local);
        } else {
            file = g_strdup(item->src);
        }

        if (strlen(item->path) > 0) {
            path = item->path;
        } else {
            path = instance->path;
        }

        printf("Sending Open %s to connection %p\nitem->hrefid = %i item->src = %s\n", file,
               instance->connection, item->hrefid, item->src);
        if (item->hrefid == 0) {
            if (item->streaming) {
                send_signal_with_double(instance, item, "SetCachePercent", 0);
                nullstr = g_strdup("");
                send_signal_with_string(instance, item, "SetProgressText", nullstr);
                g_free(nullstr);
            }
            message = dbus_message_new_signal(path, "com.gnome.mplayer", "Open");
            dbus_message_append_args(message, DBUS_TYPE_STRING, &file, DBUS_TYPE_INVALID);
            dbus_connection_send(instance->connection, message, NULL);
            dbus_message_unref(message);
            if (item->retrieved == TRUE)
                send_signal_with_double(instance, item, "SetCachePercent", 1.0);

        } else {
            // ok, not done here yet, may need a new window for Apple HD video
            id = g_strdup_printf("%i", item->hrefid);
            message = dbus_message_new_signal(path, "com.gnome.mplayer", "OpenButton");
            dbus_message_append_args(message, DBUS_TYPE_STRING, &file,
                                     DBUS_TYPE_STRING, &id, DBUS_TYPE_INVALID);
            dbus_connection_send(instance->connection, message, NULL);
            dbus_message_unref(message);
        }
        send_signal_with_string(instance, item, "SetURL", item->src);
        item->opened = TRUE;
        instance->lastopened = item;
    } else {
        printf("Item already opened before\n");
    }
}

void resize_window(CPlugin * instance, ListItem * item, gint x, gint y)
{
    DBusMessage *message;
    gchar *path;

    if (instance == NULL)
        return;
    if (x < 0 || y < 0)
        return;

    if (item != NULL && strlen(item->path) > 0) {
        path = item->path;
    } else {
        path = instance->path;
    }

    if (instance->playerready) {
        if (instance->connection != NULL && path != NULL) {
            message = dbus_message_new_signal(path, "com.gnome.mplayer", "ResizeWindow");
            dbus_message_append_args(message, DBUS_TYPE_INT32, &x,
                                     DBUS_TYPE_INT32, &y, DBUS_TYPE_INVALID);
            dbus_connection_send(instance->connection, message, NULL);
            dbus_message_unref(message);
        }
    }
}


void send_signal(CPlugin * instance, ListItem * item, const gchar * signal)
{
    DBusMessage *message;
    const char *localsignal;
    gchar *path;

    // printf("Sending %s to connection %p\n", signal, instance->connection);
    if (instance == NULL)
        return;

    if (instance->console != NULL) {
        path = g_strdup_printf("/console/%s", instance->console);
    } else {
        if (item != NULL && strlen(item->path) > 0) {
            path = g_strdup(item->path);
        } else {
            path = g_strdup(instance->path);
        }
    }

    if (instance->playerready && instance->connection != NULL) {
        localsignal = g_strdup(signal);
        message = dbus_message_new_signal(path, "com.gnome.mplayer", localsignal);
        dbus_connection_send(instance->connection, message, NULL);
        dbus_message_unref(message);
    }

    g_free(path);

}

void send_signal_when_ready(CPlugin * instance, ListItem * item, const gchar * signal)
{
    DBusMessage *message;
    const char *localsignal;
    gchar *path;

    if (instance == NULL)
        return;

    if (item != NULL && strlen(item->path) > 0) {
        path = item->path;
    } else {
        path = instance->path;
    }

    if (instance->player_launched) {
        while (!(instance->playerready)) {
            g_main_context_iteration(NULL, FALSE);
            g_usleep(1000);
        }
        if (instance->playerready && (instance->connection != NULL)) {
            //printf("Sending %s to connection %p\n", signal, instance->connection);
            localsignal = g_strdup(signal);
            message = dbus_message_new_signal(path, "com.gnome.mplayer", localsignal);
            dbus_connection_send(instance->connection, message, NULL);
            dbus_message_unref(message);
            //printf("Sent %s to connection %p\n", signal, instance->connection);
        }
    }
}

void send_signal_with_string(CPlugin * instance, ListItem * item, const gchar * signal, gchar * str)
{
    DBusMessage *message;
    const char *localsignal;
    const char *localstr;
    gchar *path;

    printf("Sending %s to connection %p\n", signal, instance->connection);
    if (instance == NULL)
        return;

    if (instance->console != NULL) {
        path = g_strdup_printf("/console/%s", instance->console);
    } else {
        if (item != NULL && strlen(item->path) > 0) {
            path = g_strdup(item->path);
        } else {
            path = g_strdup(instance->path);
        }
    }

    if (instance->playerready && instance->connection != NULL) {
        localsignal = g_strdup(signal);
        localstr = g_strdup(str);
        message = dbus_message_new_signal(path, "com.gnome.mplayer", localsignal);
        dbus_message_append_args(message, DBUS_TYPE_STRING, &localstr, DBUS_TYPE_INVALID);
        dbus_connection_send(instance->connection, message, NULL);
        dbus_message_unref(message);
    }

    g_free(path);
}

void send_signal_with_double(CPlugin * instance, ListItem * item, const gchar * signal, gdouble dbl)
{
    DBusMessage *message;
    const char *localsignal;
    gchar *path;

    //printf("Sending %s to connection %p\n", signal, instance->connection);
    if (instance == NULL)
        return;

    if (instance->console != NULL) {
        path = g_strdup_printf("/console/%s", instance->console);
    } else {
        if (item != NULL && strlen(item->path) > 0) {
            path = g_strdup(item->path);
        } else {
            path = g_strdup(instance->path);
        }
    }

    if (instance->playerready && instance->connection != NULL) {
        localsignal = g_strdup(signal);
        message = dbus_message_new_signal(path, "com.gnome.mplayer", localsignal);
        dbus_message_append_args(message, DBUS_TYPE_DOUBLE, &dbl, DBUS_TYPE_INVALID);
        dbus_connection_send(instance->connection, message, NULL);
        dbus_message_unref(message);
    }

    g_free(path);
}

void send_signal_with_boolean(CPlugin * instance, ListItem * item,
                              const gchar * signal, gboolean boolean)
{
    DBusMessage *message;
    const char *localsignal;
    gchar *path;

    //printf("Sending %s to connection %p\n", signal, instance->connection);
    if (instance == NULL)
        return;

    if (instance->console != NULL) {
        path = g_strdup_printf("/console/%s", instance->console);
    } else {
        if (item != NULL && strlen(item->path) > 0) {
            path = g_strdup(item->path);
        } else {
            path = g_strdup(instance->path);
        }
    }

    if (instance->playerready && instance->connection != NULL) {
        localsignal = g_strdup(signal);
        message = dbus_message_new_signal(path, "com.gnome.mplayer", localsignal);
        dbus_message_append_args(message, DBUS_TYPE_BOOLEAN, &boolean, DBUS_TYPE_INVALID);
        dbus_connection_send(instance->connection, message, NULL);
        dbus_message_unref(message);
    }

    g_free(path);
}

void send_signal_with_integer(CPlugin * instance, ListItem * item, const gchar * signal, gint val)
{
    DBusMessage *message;
    const char *localsignal;
    gchar *path;

    //printf("Sending %s to connection %p\n", signal, instance->connection);
    if (instance == NULL)
        return;

    if (instance->console != NULL) {
        path = g_strdup_printf("/console/%s", instance->console);
    } else {
        if (item != NULL && strlen(item->path) > 0) {
            path = g_strdup(item->path);
        } else {
            path = g_strdup(instance->path);
        }
    }

    if (instance->playerready && instance->connection != NULL) {
        localsignal = g_strdup(signal);
        message = dbus_message_new_signal(path, "com.gnome.mplayer", localsignal);
        dbus_message_append_args(message, DBUS_TYPE_INT32, &val, DBUS_TYPE_INVALID);
        dbus_connection_send(instance->connection, message, NULL);
        dbus_message_unref(message);
    }

    g_free(path);
}


gboolean request_boolean_value(CPlugin * instance, ListItem * item, const gchar * member)
{
    DBusMessage *message;
    DBusMessage *replymessage;
    const gchar *localmember;
    DBusError error;
    gboolean result = FALSE;
    gchar *path;
    gchar *dest;
    gint controlid;

    //printf("Requesting %s to connection %p\n", member, instance->connection);
    if (instance == NULL)
        return result;

    if (item != NULL && strlen(item->path) > 0) {
        path = item->path;
        controlid = item->controlid;
    } else {
        path = instance->path;
        controlid = instance->controlid;
    }

    dest = g_strdup_printf("com.gnome.mplayer.cid%i", controlid);

    if (instance->playerready && instance->connection != NULL) {
        localmember = g_strdup(member);
        message = dbus_message_new_method_call(dest, path, "com.gnome.mplayer", localmember);
        dbus_error_init(&error);
        replymessage =
            dbus_connection_send_with_reply_and_block(instance->connection, message, -1, &error);
        if (dbus_error_is_set(&error)) {
            printf("Error message = %s\n", error.message);
        }
        dbus_message_get_args(replymessage, &error, DBUS_TYPE_BOOLEAN, &result, DBUS_TYPE_INVALID);
        dbus_message_unref(message);
        dbus_message_unref(replymessage);
    }
    g_free(dest);

    return result;
}

gdouble request_double_value(CPlugin * instance, ListItem * item, const gchar * member)
{
    DBusMessage *message;
    DBusMessage *replymessage;
    const gchar *localmember;
    DBusError error;
    gdouble result = 0.0;
    gchar *path;
    gchar *dest;
    gint controlid;

    //printf("Requesting %s to connection %p\n", member, instance->connection);
    if (instance == NULL)
        return result;

    if (item != NULL && strlen(item->path) > 0) {
        path = item->path;
        controlid = item->controlid;
    } else {
        path = instance->path;
        controlid = instance->controlid;
    }

    dest = g_strdup_printf("com.gnome.mplayer.cid%i", controlid);

    if (instance->playerready && instance->connection != NULL) {
        localmember = g_strdup(member);
        message = dbus_message_new_method_call(dest, path, "com.gnome.mplayer", localmember);
        dbus_error_init(&error);
        replymessage =
            dbus_connection_send_with_reply_and_block(instance->connection, message, -1, &error);
        if (dbus_error_is_set(&error)) {
            printf("Error message = %s\n", error.message);
        }
        dbus_message_get_args(replymessage, &error, DBUS_TYPE_DOUBLE, &result, DBUS_TYPE_INVALID);
        dbus_message_unref(message);
        dbus_message_unref(replymessage);
    }

    g_free(dest);

    return result;
}

gint request_int_value(CPlugin * instance, ListItem * item, const gchar * member)
{
    DBusMessage *message;
    DBusMessage *replymessage;
    const gchar *localmember;
    DBusError error;
    gint result = 0;
    gchar *path;
    gchar *dest;
    gint controlid;

    //printf("Requesting %s to connection %p\n", member, instance->connection);
    if (instance == NULL)
        return result;

    if (item != NULL && strlen(item->path) > 0) {
        path = item->path;
        controlid = item->controlid;
    } else {
        path = instance->path;
        controlid = instance->controlid;
    }

    dest = g_strdup_printf("com.gnome.mplayer.cid%i", controlid);

    if (instance->playerready && instance->connection != NULL) {
        localmember = g_strdup(member);
        message = dbus_message_new_method_call(dest, path, "com.gnome.mplayer", localmember);
        dbus_error_init(&error);
        replymessage =
            dbus_connection_send_with_reply_and_block(instance->connection, message, -1, &error);
        if (dbus_error_is_set(&error)) {
            printf("Error message = %s\n", error.message);
        }
        dbus_message_get_args(replymessage, &error, DBUS_TYPE_INT32, &result, DBUS_TYPE_INVALID);
        dbus_message_unref(message);
        dbus_message_unref(replymessage);
    }
    g_free(dest);

    return result;
}

gchar *request_string_value(CPlugin * instance, ListItem * item, const gchar * member)
{
    DBusMessage *message;
    DBusMessage *replymessage;
    const gchar *localmember;
    DBusError error;
    gchar *result = NULL;
    gchar *path;
    gchar *dest;
    gint controlid;

    //printf("Requesting %s to connection %p\n", member, instance->connection);
    if (instance == NULL)
        return result;

    if (item != NULL && strlen(item->path) > 0) {
        path = item->path;
        controlid = item->controlid;
    } else {
        path = instance->path;
        controlid = instance->controlid;
    }

    dest = g_strdup_printf("com.gnome.mplayer.cid%i", controlid);

    if (instance->playerready && instance->connection != NULL) {
        localmember = g_strdup(member);
        message = dbus_message_new_method_call(dest, path, "com.gnome.mplayer", localmember);
        dbus_error_init(&error);
        replymessage =
            dbus_connection_send_with_reply_and_block(instance->connection, message, -1, &error);
        if (dbus_error_is_set(&error)) {
            printf("Error message = %s\n", error.message);
        }
        dbus_message_get_args(replymessage, &error, DBUS_TYPE_STRING, &result, DBUS_TYPE_INVALID);
        dbus_message_unref(message);
        dbus_message_unref(replymessage);
    }
    g_free(dest);

    return result;
}


gboolean is_valid_path(CPlugin * instance, const char *message)
{
    gboolean result = FALSE;
    ListItem *item;
    GList *iter;

    if (instance == NULL)
        return result;

    if (g_ascii_strcasecmp(message, instance->path) == 0) {

        result = TRUE;

    } else if (g_ascii_strcasecmp(message, "/DEBUG") == 0) {

        result = TRUE;

    } else {

        if (instance->playlist != NULL) {
            for (iter = instance->playlist; iter != NULL; iter = g_list_next(iter)) {
                item = (ListItem *) iter->data;
                if (item != NULL) {
                    if (g_ascii_strcasecmp(message, item->path) == 0) {
                        result = TRUE;
                    }
                }
            }
        }

    }

    return result;
}

gint request_bitrate(CPlugin * instance, ListItem * item, gchar * name)
{
    DBusMessage *message;
    DBusMessage *replymessage;
    const gchar *localmember;
    const gchar *localname;
    DBusError error;
    gint result = 0;
    gchar *path;
    gchar *dest;
    gint controlid;

    //printf("Requesting %s to connection %p\n", member, instance->connection);
    if (instance == NULL)
        return result;

    if (item != NULL && strlen(item->path) > 0) {
        path = item->path;
        controlid = item->controlid;
    } else {
        path = instance->path;
        controlid = instance->controlid;
    }

    dest = g_strdup_printf("com.gnome.mplayer.cid%i", controlid);

    if (instance->playerready && instance->connection != NULL) {
        localmember = g_strdup("GetBitrate");
        localname = g_strdup(name);
        message = dbus_message_new_method_call(dest, path, "com.gnome.mplayer", localmember);
        dbus_message_append_args(message, DBUS_TYPE_STRING, &localname, DBUS_TYPE_INVALID);
        dbus_error_init(&error);
        replymessage =
            dbus_connection_send_with_reply_and_block(instance->connection, message, -1, &error);
        if (dbus_error_is_set(&error)) {
            printf("Error message = %s\n", error.message);
        }
        if (replymessage != NULL) {
            dbus_message_get_args(replymessage, &error, DBUS_TYPE_INT32, &result,
                                  DBUS_TYPE_INVALID);
            dbus_message_unref(replymessage);
        }
        dbus_message_unref(message);
    }
    g_free(dest);

    return result;
}
