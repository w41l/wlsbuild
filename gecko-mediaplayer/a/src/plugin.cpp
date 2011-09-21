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
//#include "npupp.h"
#include "plugin_list.h"
#include "plugin_setup.h"
#include "plugin_types.h"
#include "plugin_dbus.h"
#include <nsIPrefBranch.h>
#include <nsIPrefService.h>
#include <nsIServiceManager.h>
#include <nsISupportsPrimitives.h>
#include <dlfcn.h>

static NPObject *sWindowObj;

//#include "nsIServiceManager.h"
//#include "nsISupportsUtils.h"   // some usefule macros are defined here

#define MIME_TYPES_HANDLED  "application/scriptable-plugin"
#define PLUGIN_NAME         "Scriptable Example Plugin for Mozilla"
#define MIME_TYPES_DESCRIPTION  MIME_TYPES_HANDLED":scr:"PLUGIN_NAME
#define PLUGIN_DESCRIPTION  PLUGIN_NAME " (Plug-ins SDK sample)"

int32 STREAMBUFSIZE = 0X0FFFFFFF;

// methods
static NPIdentifier Play_id;
static NPIdentifier PlayAt_id;
static NPIdentifier Pause_id;
static NPIdentifier PlayPause_id;
static NPIdentifier Stop_id;
static NPIdentifier quit_id;
static NPIdentifier DoPlay_id;
static NPIdentifier DoPause_id;
static NPIdentifier FastForward_id;
static NPIdentifier FastReverse_id;
static NPIdentifier ff_id;
static NPIdentifier rew_id;
static NPIdentifier rewind_id;
static NPIdentifier Seek_id;
static NPIdentifier Open_id;
static NPIdentifier SetVolume_id;
static NPIdentifier GetVolume_id;
static NPIdentifier SetFileName_id;
static NPIdentifier GetFileName_id;
static NPIdentifier SetIsLooping_id;
static NPIdentifier GetIsLooping_id;
static NPIdentifier SetAutoPlay_id;
static NPIdentifier GetAutoPlay_id;
static NPIdentifier SetHREF_id;
static NPIdentifier GetHREF_id;
static NPIdentifier SetURL_id;
static NPIdentifier GetURL_id;
static NPIdentifier GetMIMEType_id;
static NPIdentifier getTime_id;
static NPIdentifier getDuration_id;
static NPIdentifier getPercent_id;
static NPIdentifier getBitrate_id;
static NPIdentifier isplaying_id;
static NPIdentifier playlistAppend_id;
static NPIdentifier playlistClear_id;
static NPIdentifier onClick_id;
static NPIdentifier onMediaComplete_id;
static NPIdentifier onMouseUp_id;
static NPIdentifier onMouseDown_id;
static NPIdentifier onMouseOut_id;
static NPIdentifier onMouseOver_id;
static NPIdentifier onDestroy_id;

static NPIdentifier controls_play_id;
static NPIdentifier controls_pause_id;
static NPIdentifier controls_stop_id;
static NPIdentifier controls_fastForward_id;
static NPIdentifier controls_fastReverse_id;
static NPIdentifier controls_step_id;

static NPIdentifier media_getItemInfo_id;


// properties
static NPIdentifier filename_id;
static NPIdentifier src_id;
static NPIdentifier ShowControls_id;
static NPIdentifier fullscreen_id;
static NPIdentifier showlogo_id;
static NPIdentifier playState_id;
static NPIdentifier controls_id;
static NPIdentifier media_id;
static NPIdentifier settings_id;
static NPIdentifier error_id;

static NPIdentifier status_id;
static NPIdentifier URL_id;
static NPIdentifier versionInfo_id;
static NPIdentifier controls_currentPosition_id;
static NPIdentifier controls_currentItem_id;
static NPIdentifier media_duration_id;
static NPIdentifier settings_volume_id;
static NPIdentifier error_errorCount_id;
//////////////////////////////////////
//
// general initialization and shutdown
//
NPError NS_PluginInitialize()
{
    return NPERR_NO_ERROR;
}

void NS_PluginShutdown()
{
}

// get values per plugin
NPError NS_PluginGetValue(NPPVariable aVariable, void *aValue)
{
    return PluginGetValue(aVariable, aValue);
}

void postDOMEvent(NPP mInstance, const gchar * id, const gchar * event)
{
    gchar *jscript;

    jscript = g_strdup_printf("javascript:obj_gmp=document.getElementById('%s');"
                              "e_gmp=document.createEvent('Events');"
                              "e_gmp.initEvent('%s',true,true);" "obj_gmp.dispatchEvent(e_gmp);",
                              id, event);
    NPN_GetURL(mInstance, jscript, NULL);
    g_free(jscript);
}

// disabled for now due to problems with certain sites
void postPlayStateChange(NPP mInstance, const gint state)
{
    gchar *jscript;

    jscript = g_strdup_printf("javascript:if (typeof OnDSPlayStateChangeEvt == 'function') {"
                              "OnDSPlayStateChangeEvt(%i);}", state);
    //NPN_GetURL(mInstance, jscript, NULL);
    g_free(jscript);
}

////////////////////////////////////////
//
// CPlugin class implementation
//
CPlugin::CPlugin(NPP pNPInstance):
mInstance(pNPInstance),
m_pNPStream(NULL),
mInitialized(FALSE),
m_pScriptableObject(NULL),
m_pScriptableObjectControls(NULL),
m_pScriptableObjectMedia(NULL),
m_pScriptableObjectSettings(NULL),
m_pScriptableObjectError(NULL),
mWindow(0),
windowless(FALSE),
playlist(NULL),
player_launched(FALSE),
connection(NULL),
dbus_dispatch(NULL),
path(NULL),
acceptdata(TRUE),
playerready(FALSE),
nextid(1),
lastopened(NULL),
cache_size(2048),
hidden(FALSE),
autostart(1),
lastupdate(0),
show_controls(1),
name(NULL),
id(NULL),
console(NULL),
controls(NULL),
user_agent(NULL),
page_url(NULL),
player_backend(NULL),
quicktime_emulation(FALSE),
disable_context_menu(FALSE),
disable_fullscreen(FALSE),
post_dom_events(FALSE),
event_mediacomplete(NULL),
event_destroy(NULL),
event_mousedown(NULL),
event_mouseup(NULL),
event_mouseclicked(NULL), event_enterwindow(NULL), event_leavewindow(NULL), debug(FALSE),
tv_driver(NULL), tv_device(NULL), tv_input(NULL), tv_width(0), tv_height(0)
{
    GRand *rand;
    GmPrefStore *store;
    gboolean b;

    NPN_GetValue(mInstance, NPNVWindowNPObject, &sWindowObj);

    // get the page plugin was called from 
    // found at https://developer.mozilla.org/en/Getting_the_page_URL_in_NPAPI_plugin
    NPIdentifier identifier = NPN_GetStringIdentifier("location");
    // Declare a local variant value.
    NPVariant variantValue;
    // Get the location property from the window object (which is another object).
    b = NPN_GetProperty(mInstance, sWindowObj, identifier, &variantValue);
    // Get a pointer to the "location" object.
    NPObject *locationObj = variantValue.value.objectValue;
    // Create a "href" identifier.
    identifier = NPN_GetStringIdentifier("href");
    // Get the location property from the location object.
    b = NPN_GetProperty(mInstance, locationObj, identifier, &variantValue);
#ifdef HAVE_NEW_XULRUNNER
    page_url = g_strdup_printf("%s", NPVARIANT_TO_STRING(variantValue).UTF8Characters);
#else
    page_url = g_strdup_printf("%s", NPVARIANT_TO_STRING(variantValue).utf8characters);
#endif

    // register methods
    Play_id = NPN_GetStringIdentifier("Play");
    PlayAt_id = NPN_GetStringIdentifier("PlayAt");
    Pause_id = NPN_GetStringIdentifier("Pause");
    PlayPause_id = NPN_GetStringIdentifier("PlayPause");
    Stop_id = NPN_GetStringIdentifier("Stop");
    quit_id = NPN_GetStringIdentifier("quit");
    DoPlay_id = NPN_GetStringIdentifier("DoPlay");
    DoPause_id = NPN_GetStringIdentifier("DoPause");
    FastForward_id = NPN_GetStringIdentifier("FastForward");
    FastReverse_id = NPN_GetStringIdentifier("FastReverse");
    ff_id = NPN_GetStringIdentifier("ff");
    rew_id = NPN_GetStringIdentifier("rew");
    rewind_id = NPN_GetStringIdentifier("rewind");
    Seek_id = NPN_GetStringIdentifier("Seek");
    Open_id = NPN_GetStringIdentifier("Open");
    SetVolume_id = NPN_GetStringIdentifier("SetVolume");
    GetVolume_id = NPN_GetStringIdentifier("GetVolume");
    SetFileName_id = NPN_GetStringIdentifier("SetFileName");
    GetFileName_id = NPN_GetStringIdentifier("GetFileName");
    SetIsLooping_id = NPN_GetStringIdentifier("SetIsLooping");
    GetIsLooping_id = NPN_GetStringIdentifier("GetIsLooping");
    SetAutoPlay_id = NPN_GetStringIdentifier("SetAutoPlay");
    GetAutoPlay_id = NPN_GetStringIdentifier("GetAutoPlay");
    SetHREF_id = NPN_GetStringIdentifier("SetHREF");
    GetHREF_id = NPN_GetStringIdentifier("GetHREF");
    SetURL_id = NPN_GetStringIdentifier("SetURL");
    GetURL_id = NPN_GetStringIdentifier("GetURL");
    GetMIMEType_id = NPN_GetStringIdentifier("GetMIMEType");
    getTime_id = NPN_GetStringIdentifier("getTime");
    getDuration_id = NPN_GetStringIdentifier("getDuration");
    getPercent_id = NPN_GetStringIdentifier("getPercent");
    getBitrate_id = NPN_GetStringIdentifier("getBitrate");
    isplaying_id = NPN_GetStringIdentifier("isplaying");
    playlistAppend_id = NPN_GetStringIdentifier("playlistAppend");
    playlistClear_id = NPN_GetStringIdentifier("playlistClear");
    onClick_id = NPN_GetStringIdentifier("onClick");
    onMediaComplete_id = NPN_GetStringIdentifier("onMediaComplete");
    onMouseUp_id = NPN_GetStringIdentifier("onMouseUp");
    onMouseDown_id = NPN_GetStringIdentifier("onMouseDown");
    onMouseOut_id = NPN_GetStringIdentifier("onMouseOut");
    onMouseOver_id = NPN_GetStringIdentifier("onMouseOver");
    onDestroy_id = NPN_GetStringIdentifier("onDestroy");

    controls_play_id = NPN_GetStringIdentifier("play");
    controls_pause_id = NPN_GetStringIdentifier("pause");
    controls_stop_id = NPN_GetStringIdentifier("stop");
    controls_fastForward_id = NPN_GetStringIdentifier("fastForward");
    controls_fastReverse_id = NPN_GetStringIdentifier("fastReverse");
    controls_step_id = NPN_GetStringIdentifier("step");

    media_getItemInfo_id = NPN_GetStringIdentifier("getItemInfo");

    // register properties
    filename_id = NPN_GetStringIdentifier("filename");
    src_id = NPN_GetStringIdentifier("src");
    ShowControls_id = NPN_GetStringIdentifier("ShowControls");
    fullscreen_id = NPN_GetStringIdentifier("fullscreen");
    showlogo_id = NPN_GetStringIdentifier("showlogo");
    playState_id = NPN_GetStringIdentifier("playState");
    controls_id = NPN_GetStringIdentifier("controls");
    controls_currentItem_id = NPN_GetStringIdentifier("currentItem");
    media_id = NPN_GetStringIdentifier("media");
    settings_id = NPN_GetStringIdentifier("settings");
    error_id = NPN_GetStringIdentifier("error");

    status_id = NPN_GetStringIdentifier("status");
    URL_id = NPN_GetStringIdentifier("URL");
    versionInfo_id = NPN_GetStringIdentifier("versionInfo");
    controls_currentPosition_id = NPN_GetStringIdentifier("currentPosition");
    media_duration_id = NPN_GetStringIdentifier("duration");
    settings_volume_id = NPN_GetStringIdentifier("volume");
    error_errorCount_id = NPN_GetStringIdentifier("errorCount");

    // generate a random controlid
    rand = g_rand_new();
    controlid = g_rand_int_range(rand, 0, 65535);
    g_rand_free(rand);

    if (path == NULL) {
        path = g_strdup_printf("/control/%i", controlid);
        // printf("using path %s\n",path);
    }
#ifdef ENABLE_NLS
    bindtextdomain(GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    textdomain(GETTEXT_PACKAGE);
#endif
    printf(_("gecko mediaplayer v%s\n"), VERSION);

    g_type_init();
    store = gm_pref_store_new("gecko-mediaplayer");
    if (store != NULL) {
        debug_level = gm_pref_store_get_int(store, DEBUG_LEVEL);
        player_backend = gm_pref_store_get_string(store, PLAYER_BACKEND);
        printf("Using player backend of '%s'\n", player_backend);
        gm_pref_store_free(store);
    }


    if (connection == NULL) {
        connection = dbus_hookup(this);
    }
    pluginSpecific(this);

    mInitialized = TRUE;
}

CPlugin::~CPlugin()
{
    // mScriptablePeer may be also held by the browser 
    // so releasing it here does not guarantee that it is over
    // we should take precaution in case it will be called later
    // and zero its mPlugin member
    // mScriptablePeer->SetInstance(NULL);
    // NS_IF_RELEASE(mScriptablePeer);

    if (mInitialized)
        shut();
    if (sWindowObj)
        NPN_ReleaseObject(sWindowObj);

    mInstance = NULL;
/*
    if (mControlsScriptablePeer != NULL) {
        mControlsScriptablePeer->SetInstance(NULL);
        mControlsScriptablePeer->Release();
        NS_IF_RELEASE(mControlsScriptablePeer);
    }
*/
    // clearPreference(this, "general.useragent.override");

    if (m_pScriptableObjectControls) {
        NPN_ReleaseObject(m_pScriptableObjectControls);
    }

    if (m_pScriptableObject) {
        NPN_ReleaseObject(m_pScriptableObject);
    }
}



NPBool CPlugin::init(NPWindow * pNPWindow)
{
    if (pNPWindow == NULL)
        return FALSE;

    m_Window = pNPWindow;
    mInitialized = TRUE;

    return mInitialized;
}

int16 CPlugin::handleEvent(void *event)
{
#ifdef XP_MAC
    NPEvent *ev = (NPEvent *) event;
    if (m_Window) {
        Rect box = { m_Window->y, m_Window->x,
            m_Window->y + m_Window->height, m_Window->x + m_Window->width
        };
        if (ev->what == updateEvt) {
            ::TETextBox(m_String, strlen(m_String), &box, teJustCenter);
        }
    }
#endif
    return 0;
}

NPError CPlugin::SetWindow(NPWindow * aWindow)
{
    GError *error = NULL;
    gchar *argvn[255];
    gint arg = 0;
    gint ok;
    ListItem *item;
    gchar *app_name;

    if (!acceptdata)
        return NPERR_NO_ERROR;

    if (aWindow == NULL)
        return NPERR_NO_ERROR;

    mX = aWindow->x;
    mY = aWindow->y;
    mWidth = aWindow->width;
    mHeight = aWindow->height;
    if (mWindow != (Window) aWindow->window) {
        mWindow = (Window) aWindow->window;
        NPSetWindowCallbackStruct *ws_info = (NPSetWindowCallbackStruct *) aWindow->ws_info;
    }


    if (player_launched && mWidth > 0 && mHeight > 0) {
        resize_window(this, NULL, mWidth, mHeight);
    }

    if (!player_launched && mWidth > 0 && mHeight > 0) {
        app_name = NULL;
        if (player_backend != NULL) {
            app_name = g_find_program_in_path(player_backend);
        }
        if (app_name == NULL) {
            app_name = g_find_program_in_path("gnome-mplayer");
            if (app_name == NULL)
                app_name = g_find_program_in_path("gnome-mplayer-minimal");
        }

        argvn[arg++] = g_strdup_printf("%s", app_name);
        argvn[arg++] = g_strdup_printf("--window=%i", (gint) mWindow);
        argvn[arg++] = g_strdup_printf("--controlid=%i", controlid);
        argvn[arg++] = g_strdup_printf("--width=%i", mWidth);
        argvn[arg++] = g_strdup_printf("--height=%i", mHeight);
        argvn[arg++] = g_strdup_printf("--autostart=%i", autostart);
        argvn[arg++] = g_strdup_printf("--showcontrols=%i", show_controls);
        if (disable_context_menu == TRUE)
            argvn[arg++] = g_strdup_printf("--disablecontextmenu");
        if (disable_fullscreen == TRUE)
            argvn[arg++] = g_strdup_printf("--disablefullscreen");
        if (debug == TRUE)
            argvn[arg++] = g_strdup_printf("--verbose");
        if (name != NULL)
            argvn[arg++] = g_strdup_printf("--rpname=%s", name);
        if (console != NULL)
            argvn[arg++] = g_strdup_printf("--rpconsole=%s", console);
        if (controls != NULL) {
            argvn[arg++] = g_strdup_printf("--rpcontrols=%s", controls);
        }
        if (tv_device != NULL) {
            argvn[arg++] = g_strdup_printf("--tvdevice=%s", tv_device);
        }
        if (tv_driver != NULL) {
            argvn[arg++] = g_strdup_printf("--tvdriver=%s", tv_driver);
        }
        if (tv_input != NULL) {
            argvn[arg++] = g_strdup_printf("--tvinput=%s", tv_input);
        }
        if (tv_width > 0) {
            argvn[arg++] = g_strdup_printf("--tvwidth=%i", tv_width);
        }
        if (tv_height > 0) {
            argvn[arg++] = g_strdup_printf("--tvheight=%i", tv_height);
        }

        argvn[arg] = NULL;
        playerready = FALSE;
        ok = g_spawn_async(NULL, argvn, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, NULL, &error);

        if (ok) {
            player_launched = TRUE;
        } else {
            printf("Unable to launch %s: %s\n", app_name, error->message);
            g_error_free(error);
            error = NULL;
        }

        g_free(app_name);

        if (post_dom_events && id != NULL) {
            postDOMEvent(mInstance, id, "qt_begin");
        }
        //postPlayStateChange(mInstance, STATE_READY);
    }

    if (playlist != NULL) {
        item = (ListItem *) playlist->data;
        if (item && !item->play)
            item = list_find_next_playable(playlist);
        if (item && !item->requested) {
            item->cancelled = FALSE;
            if (item->streaming) {
                printf("Calling open_location with item = %p src = %s\n", item, item->src);
                open_location(this, item, FALSE);
                item->requested = 1;
            } else {
                item->requested = 1;
                printf("Calling GetURLNotify with item = %p src = %s\n", item, item->src);
                this->GetURLNotify(mInstance, item->src, NULL, item);
            }
        }
    }
    return NPERR_NO_ERROR;
}


void CPlugin::shut()
{
    ListItem *item;
    GList *iter;

    acceptdata = FALSE;
    mInitialized = FALSE;

    if (playlist != NULL) {
        for (iter = playlist; iter != NULL; iter = g_list_next(iter)) {
            item = (ListItem *) iter->data;
            if (item != NULL) {
                item->cancelled = TRUE;
                if (item->controlid != 0) {
                    send_signal_when_ready(this, item, "Terminate");
                }
            }
        }
    }
    send_signal_when_ready(this, NULL, "Terminate");
    playerready = FALSE;
    playlist = list_clear(playlist);

    if (event_destroy != NULL) {
        NPN_GetURL(mInstance, event_destroy, NULL);
    }

    if (connection != NULL) {
        connection = dbus_unhook(connection, this);
    }
}

NPBool CPlugin::isInitialized()
{
    return mInitialized;
}

NPError CPlugin::NewStream(NPMIMEType type, NPStream * stream, NPBool seekable, uint16 * stype)
{
    if (g_strrstr(stream->url, "javascript") == NULL) {
        printf("New Stream Requested - %s\n", stream->url);
    }

    if (g_strrstr(stream->url, "javascript") == NULL && stream->notifyData == NULL) {
        printf("item is NULL for %s\n", stream->url);
    }

    return NPERR_NO_ERROR;
}

NPError CPlugin::DestroyStream(NPStream * stream, NPError reason)
{
    ListItem *item;
    gint id;
    gchar *path;
    gchar *text;
    gboolean ready;
    gboolean newwindow;

    if (g_strrstr(stream->url, "javascript") == NULL)
        printf("Entering destroy stream reason = %i for %s\n", reason, stream->url);

    if (reason == NPERR_NO_ERROR) {
        item = (ListItem *) stream->notifyData;
        // item = list_find(playlist, (gchar*)stream->url);

        if (item == NULL) {
            printf("Leaving destroy stream - item not found\n");
            return NPERR_NO_ERROR;
        }

        if (item->localfp) {
            fclose(item->localfp);
            item->retrieved = TRUE;
            item->localfp = 0;
            send_signal_with_double(this, item, "SetCachePercent", 1.0);
            text = g_strdup_printf(_("Cache fill: %2.2f%%"), 100.0);
            send_signal_with_string(this, item, "SetProgressText", text);
            g_free(text);
        }

        if (!item->opened && item->play) {
            id = item->controlid;
            path = g_strdup(item->path);
            ready = item->playerready;
            newwindow = item->newwindow;
            if (!item->streaming)
                item->streaming = streaming(item->src);
            if (!item->streaming) {
                printf("in Destroy Stream\n");
                playlist = list_parse_qt(playlist, item);
                playlist = list_parse_qt2(playlist, item);
                playlist = list_parse_asx(playlist, item);
                playlist = list_parse_qml(playlist, item);
                playlist = list_parse_ram(playlist, item);
            }
            // printf("item->play = %i\n",item->play);
            // printf("item->src = %s\n", item->src);
            // printf("item->streaming = %i\n", item->streaming);
            // printf("calling open_location from DestroyStream\n");
            if (item->play) {
                item->requested = TRUE;
                open_location(this, item, TRUE);
                if (post_dom_events && this->id != NULL) {
                    postDOMEvent(mInstance, this->id, "qt_play");
                }
            } else {
                item = list_find_next_playable(playlist);
                if (item != NULL) {
                    // printf("item->play = %i\n",item->play);
                    // printf("item->src = %s\n", item->src);
                    // printf("item->streaming = %i\n", item->streaming);
                    if (!item->streaming) {
                        item->controlid = id;
                        g_strlcpy(item->path, path, 1024);
                        item->playerready = ready;
                        item->newwindow = newwindow;
                        item->cancelled = FALSE;
                        item->requested = TRUE;
                        if (item != NULL)
                            this->GetURLNotify(mInstance, item->src, NULL, item);
                    } else {
                        open_location(this, item, FALSE);
                        item->requested = TRUE;
                        if (post_dom_events && this->id != NULL) {
                            postDOMEvent(mInstance, this->id, "qt_play");
                        }
                    }
                }
            }
            g_free(path);
        }
        //printf("Leaving destroy stream src = %s\n", item->src);

    } else if (reason == NPERR_INVALID_URL) {
        item = (ListItem *) stream->notifyData;
        if (item) {
            printf("Destroy Stream, invalid url, item is %s\n", item->src);
        } else {
            if (g_strrstr(stream->url, "javascript") == NULL) {
                printf("Destroy Stream, network error, item is NULL\n");
            }
        }
    } else {
        item = (ListItem *) stream->notifyData;
        // item = list_find(playlist, (gchar*)stream->url);
        if (g_strrstr(stream->url, "javascript") == NULL)
            printf("Exiting destroy stream reason = %i for %s\n", reason, stream->url);
        if (item == NULL) {
            return NPERR_NO_ERROR;
        } else {
            if (post_dom_events && this->id != NULL) {
                postDOMEvent(mInstance, this->id, "qt_load");
            }
            postPlayStateChange(mInstance, STATE_TRANSITIONING);
        }

        if (item->localfp) {
            fclose(item->localfp);
            item->retrieved = FALSE;
            item->localfp = 0;
        }
    }

    // list_dump(playlist);
    return NPERR_NO_ERROR;
}

void CPlugin::URLNotify(const char *url, NPReason reason, void *notifyData)
{
    ListItem *item = (ListItem *) notifyData;
    //DBusMessage *message;
    //const char *file;

    printf("URL Notify url = '%s'\nreason = %i\n%s\n%s\n%s\n", url, reason, item->src, item->local,
           path);
    if (reason == NPRES_DONE) {

        if (!item->opened) {
            // open_location(this,item, TRUE);

            /*
               file = g_strdup(item->local);
               while (!playerready) {
               // printf("waiting for player\n");
               g_main_context_iteration(NULL,FALSE);   
               }
               message = dbus_message_new_signal(path,"com.gnome.mplayer","Open");
               dbus_message_append_args(message, DBUS_TYPE_STRING, &file, DBUS_TYPE_INVALID);
               dbus_connection_send(connection,message,NULL);
               dbus_message_unref(message);
             */
        }

        if (item) {
            item->played = TRUE;
            if (!item->streaming) {
                item = list_find_next_playable(playlist);
                if (item) {
                    if (item->retrieved || item->streaming) {
                        open_location(this, item, TRUE);
                        item->requested = TRUE;
                    } else {
                        this->GetURLNotify(mInstance, item->src, NULL, item);
                        item->requested = TRUE;
                    }
                }
            }
        } else {
            item = list_find_next_playable(playlist);
            if (item) {
                if (item->retrieved || item->streaming) {
                    open_location(this, item, TRUE);
                    item->requested = TRUE;
                } else {
                    this->GetURLNotify(mInstance, item->src, NULL, item);
                    item->requested = TRUE;
                }
            }
        }



    } else if (reason == NPRES_NETWORK_ERR) {
        printf("URL Notify result is Network Error\n");
    } else if (reason == NPRES_USER_BREAK) {
        printf("URL Notify result is User Break\n");
    } else {
        printf("%i is an invalid reason code in URLNotify\n", reason);
    }
}

int32 CPlugin::WriteReady(NPStream * stream)
{
    ListItem *item;
    gchar *path;
    gchar *tmp;

    // printf("WriteReady called\n");
    if (!acceptdata) {
        printf("Not accepting data\n");
        NPN_DestroyStream(mInstance, stream, NPERR_GENERIC_ERROR);
        return -1;
    }


    item = (ListItem *) stream->notifyData;
    // item = list_find(playlist, (gchar*)stream->url);
    if (item == NULL) {

        if (mode == NP_FULL) {
            // printf("adding new item %s\n",stream->url);
            /*
               item = g_new0(ListItem, 1);
               g_strlcpy(item->src, stream->url, 1024);
               item->requested = TRUE;
               item->play = TRUE;
               if (!item->streaming)
               item->streaming = streaming(item->src);
               playlist = g_list_append(playlist, item);
               stream->notifyData = item;
             */
            return -1;
        } else {
            printf("item is null\nstream url %s\n", stream->url);
            NPN_DestroyStream(mInstance, stream, NPERR_NO_ERROR);
            return -1;
        }
    } else {
        if (g_ascii_strcasecmp(item->src, stream->url) != 0) {
            g_strlcpy(item->src, stream->url, 4096);
        }
    }

    // printf("Write Ready item url = %s\n%s\n",item->src,stream->url);

    if (item->cancelled) {
        printf("cancelling WriteReady\n");
        NPN_DestroyStream(mInstance, stream, NPERR_NO_ERROR);
        return -1;
    }

    if (strlen(item->local) == 0) {
        path = g_strdup_printf("%s/gnome-mplayer/plugin", g_get_user_cache_dir());
        if (!g_file_test(path, G_FILE_TEST_IS_DIR)) {
            g_mkdir_with_parents(path, 0775);
        }
        tmp = gm_tempname(path, "gecko-mediaplayerXXXXXX");
        g_snprintf(item->local, 1024, "%s", tmp);
        g_free(tmp);
        g_free(path);
        if (strstr(mimetype, "midi") != NULL) {
            g_strlcat(item->local, ".mid", 1024);
        }
        if (strstr(mimetype, "mp3") != NULL) {
            g_strlcat(item->local, ".mp3", 1024);
        }
        if (strstr(mimetype, "audio/mpeg") != NULL) {
            g_strlcat(item->local, ".mp3", 1024);
        }
        if (strstr(mimetype, "audio/x-mod") != NULL) {
            g_strlcat(item->local, ".mod", 1024);
        }
        if (strstr(mimetype, "flac") != NULL) {
            g_strlcat(item->local, ".flac", 1024);
        }
    }

    if (item->retrieved) {
        printf("Duplicate request, item already retrieved\n");
        NPN_DestroyStream(mInstance, stream, NPERR_NO_ERROR);
        return -1;
    }

    return STREAMBUFSIZE;
}

int32 CPlugin::Write(NPStream * stream, int32 offset, int32 len, void *buffer)
{
    ListItem *item;
    int32 wrotebytes = -1;
    gchar *text;
    gdouble percent = 0.0;
    gdouble rate = 0.0;
    gint id;
    gchar *path;
    gboolean ready;
    gboolean newwindow;
    gboolean ok_to_play = FALSE;

    // printf("Write Called\n");
    if (!acceptdata) {
        printf("not accepting data\n");
        NPN_DestroyStream(mInstance, stream, NPERR_NO_ERROR);
        return -1;
    }

    item = (ListItem *) stream->notifyData;

    if (item == NULL) {
        printf("item is NULL\n");
        printf(_("Write unable to write because item is NULL\n"));
        NPN_DestroyStream(mInstance, stream, NPERR_GENERIC_ERROR);
        return -1;
    }

    if (item->cancelled || item->retrieved) {
        printf("cancelled\n");
        NPN_DestroyStream(mInstance, stream, NPERR_NO_ERROR);
        return -1;
    }

    if (strstr((char *) buffer, "ICY 200 OK") != NULL || strstr((char *) buffer, "Content-length:") != NULL     // If item is a block of jpeg images, just stream it
        || strstr((char *) buffer, "<HTML>") != NULL || item->streaming == TRUE) {
        //   || stream->lastmodified == 0) {    this is not valid for many sites

        // printf("BUFFER='%s'\n", buffer);

        // printf("item->streaming = %i\n", item->streaming);
        // printf("stream->lastmodified = %i\n", stream->lastmodified);
        // printf("stream->end = %i\n", stream->end);

        item->streaming = TRUE;
        open_location(this, item, FALSE);
        if (post_dom_events && this->id != NULL) {
            postDOMEvent(mInstance, this->id, "qt_play");
        }
        item->requested = TRUE;
        if (item->localfp) {
            fclose(item->localfp);
        }
        printf("Got IceCast Stream, let mplayer stream it\n");
        NPN_DestroyStream(mInstance, stream, NPERR_NO_ERROR);
        return -1;
    }

    if ((!item->localfp) && (!item->retrieved)) {
        // printf("opening %s for localcache\n", item->local);
        item->localfp = fopen(item->local, "w+");
    }
    // printf("Write item url = %s\n",item->src);

    if (item->localfp == NULL) {
        printf("Local cache file is not open, cannot write data\n");
        NPN_DestroyStream(mInstance, stream, NPERR_NO_ERROR);
        return -1;
    }
    fseek(item->localfp, offset, SEEK_SET);
    wrotebytes = fwrite(buffer, 1, len, item->localfp);
    item->localsize += wrotebytes;

    if (item->mediasize != stream->end)
        item->mediasize = stream->end;

    if (playerready) {
        percent = 0.0;
        if (item->mediasize > 0) {

            percent = (gdouble) item->localsize / (gdouble) item->mediasize;
            if (difftime(time(NULL), lastupdate) > 0.5) {
                send_signal_with_double(this, item, "SetCachePercent", percent);
                rate =
                    (gdouble) ((item->localsize -
                                item->lastsize) / 1024.0) / (gdouble) difftime(time(NULL),
                                                                               lastupdate);
                if (percent > 0.99) {
                    text =
                        g_strdup_printf(_("Caching %iK (%0.1f K/s)"), item->mediasize / 1024, rate);
                } else {
                    text =
                        g_strdup_printf(_("Cache fill: %2.2f%% (%0.1f K/s)"), percent * 100.0,
                                        rate);
                }
                send_signal_with_string(this, item, "SetProgressText", text);
                if (!item->opened)
                    send_signal_with_string(this, item, "SetURL", item->src);
                if (post_dom_events && this->id != NULL) {
                    postDOMEvent(mInstance, this->id, "qt_progress");
                    postDOMEvent(mInstance, this->id, "qt_durationchange");
                }
                postPlayStateChange(mInstance, STATE_BUFFERING);

                time(&lastupdate);
                item->lastsize = item->localsize;
            }
        }
        if (!item->opened) {
            if ((item->localsize >= (cache_size * 1024)) && (percent >= 0.2)) {
                printf("Setting to play because %i > %i\n", item->localsize, cache_size * 1024);
                ok_to_play = TRUE;
            }
            if (ok_to_play == FALSE && (item->localsize > (cache_size * 2 * 1024))
                && (cache_size >= 512)) {
                printf("Setting to play because %i > %i (double cache)\n", item->localsize,
                       cache_size * 2 * 1024);
                ok_to_play = TRUE;
            }
            if (ok_to_play == FALSE) {
                if (item->bitrate == 0 && item->bitrate_requests < 5
                    && ((gint) (percent * 100) > item->bitrate_requests)) {
                    item->bitrate = request_bitrate(this, item, item->local);
                    item->bitrate_requests++;
                }
                if (item->bitrate > 0) {
                    if (item->localsize / item->bitrate >= 10 && (percent >= 0.2)) {
                        printf("Setting to play becuase %i >= 10\n",
                               item->localsize / item->bitrate);
                        ok_to_play = TRUE;
                        if (post_dom_events && this->id != NULL) {
                            postDOMEvent(mInstance, this->id, "qt_canplay");
                        }
                    }
                }
            }

        }
        // if not opened, over cache level and not an href target then try and open it
        if ((!item->opened) && ok_to_play == TRUE) {
            id = item->controlid;
            path = g_strdup(item->path);
            ready = item->playerready;
            newwindow = item->newwindow;
            if (!item->streaming)
                item->streaming = streaming(item->src);
            if (!item->streaming) {
                playlist = list_parse_qt(playlist, item);
                playlist = list_parse_asx(playlist, item);
                playlist = list_parse_qml(playlist, item);
                playlist = list_parse_ram(playlist, item);
            }
            // printf("item->play = %i\n",item->play);
            // printf("item->src = %i\n", item->src);
            // printf("calling open_location from Write\n"); 
            if (item->play) {
                send_signal_with_integer(this, item, "SetGUIState", PLAYING);
                open_location(this, item, TRUE);
                item->requested = TRUE;
                if (post_dom_events && this->id != NULL) {
                    postDOMEvent(mInstance, this->id, "qt_loadedfirstframe");
                    postDOMEvent(mInstance, this->id, "qt_canplay");
                    postDOMEvent(mInstance, this->id, "qt_play");
                }
            } else {
                item = list_find_next_playable(playlist);
                if (item != NULL) {
                    item->controlid = id;
                    g_strlcpy(item->path, path, 1024);
                    item->playerready = ready;
                    item->newwindow = newwindow;
                    item->cancelled = FALSE;
                    // printf("opening next playable items on the playlist\n");
                    if (item->streaming) {
                        open_location(this, item, FALSE);
                        item->requested = TRUE;
                    } else {
                        this->GetURLNotify(mInstance, item->src, NULL, item);
                        item->requested = TRUE;
                    }
                }
            }
            g_free(path);
        }

    }

    return wrotebytes;
}


void CPlugin::Play()
{
    send_signal(this, this->lastopened, "Play");
    if (post_dom_events && this->id != NULL) {
        postDOMEvent(mInstance, this->id, "qt_play");
    }
}

void CPlugin::Pause()
{
    send_signal(this, this->lastopened, "Pause");
    if (post_dom_events && this->id != NULL) {
        postDOMEvent(mInstance, this->id, "qt_pause");
    }
}

void CPlugin::Stop()
{
    send_signal(this, this->lastopened, "Stop");
}

void CPlugin::FastForward()
{
    send_signal(this, this->lastopened, "FastForward");
    postPlayStateChange(mInstance, STATE_SCANFORWARD);
}

void CPlugin::FastReverse()
{
    send_signal(this, this->lastopened, "FastReverse");
    postPlayStateChange(mInstance, STATE_SCANREVERSE);
}

void CPlugin::Seek(double counter)
{
    send_signal_with_double(this, this->lastopened, "Seek", counter);
}

void CPlugin::SetShowControls(bool value)
{
    send_signal_with_boolean(this, this->lastopened, "SetShowControls", value);
}

void CPlugin::SetFullScreen(bool value)
{
    send_signal_with_boolean(this, this->lastopened, "SetFullScreen", value);
}

void CPlugin::SetVolume(double value)
{
    send_signal_with_double(this, this->lastopened, "Volume", value);
    if (post_dom_events && this->id != NULL) {
        postDOMEvent(mInstance, this->id, "qt_volumechange");
    }
}

void CPlugin::GetVolume(double *_retval)
{
    *_retval = request_double_value(this, this->lastopened, "GetVolume");
}

void CPlugin::GetFullScreen(bool * _retval)
{
    *_retval = request_boolean_value(this, this->lastopened, "GetFullScreen");
}

void CPlugin::GetShowControls(bool * _retval)
{
    *_retval = request_boolean_value(this, this->lastopened, "GetShowControls");
}

void CPlugin::GetTime(double *_retval)
{
    *_retval = request_double_value(this, this->lastopened, "GetTime");
}

void CPlugin::GetDuration(double *_retval)
{
    *_retval = request_double_value(this, this->lastopened, "GetDuration");
}

void CPlugin::GetPercent(double *_retval)
{
    *_retval = request_double_value(this, this->lastopened, "GetPercent");
}

void CPlugin::GetBitrate(int *_retval)
{
    *_retval = request_int_value(this, this->lastopened, "GetBitrate");
}


void CPlugin::SetFilename(const char *filename)
{
    ListItem *item;

    if (filename == NULL)
        return;

    item = g_new0(ListItem, 1);
    g_strlcpy(item->src, filename, 1024);
    item->streaming = streaming(item->src);
    item->play = TRUE;
    item->id = nextid++;
    playlist = g_list_append(playlist, item);

    send_signal(this, this->lastopened, "Quit");

    if (item->streaming) {
        open_location(this, item, FALSE);
        item->requested = 1;
    } else {
        item->requested = 1;
        this->GetURLNotify(mInstance, item->src, NULL, item);
    }
}

void CPlugin::GetFilename(char **filename)
{
    ListItem *item = NULL;

    if (this->lastopened != NULL) {
        *filename = g_strdup(this->lastopened->src);
    } else {
        if (playlist != NULL)
            item = (ListItem *) playlist->data;
        if (item != NULL) {
            *filename = g_strdup(item->src);
        } else {
            *filename = NULL;
        }
    }
}

void CPlugin::GetMIMEType(char **_retval)
{
    *_retval = g_strdup(mimetype);
}

void CPlugin::GetPlayState(PRInt32 * playstate)
{
    *playstate = request_int_value(this, this->lastopened, "GetPlayState");
}

void CPlugin::GetLoop(bool * _retval)
{
    if (lastopened != NULL) {
        *_retval = (bool) lastopened->loop;
    } else {
        *_retval = FALSE;
    }

}

void CPlugin::SetLoop(bool value)
{
    if (lastopened != NULL) {
        lastopened->loop = (int) value;
        lastopened->loopcount = -1;
    }
}

void CPlugin::PlayPause()
{
    gint state;

    state = request_int_value(this, this->lastopened, "GetPlayState");
    if (state == STATE_PAUSED) {
        send_signal(this, this->lastopened, "Play");
    }

    if (state == STATE_PLAYING) {
        send_signal(this, this->lastopened, "Pause");
    }
}


void CPlugin::SetOnClick(const char *event)
{
    if (event_mouseclicked != NULL) {
        g_free(event_mouseclicked);
    }

    if (g_ascii_strncasecmp(event, "javascript:", 11) == 0) {
        event_mouseclicked = g_strdup_printf("%s", event);
    } else {
        event_mouseclicked = g_strdup_printf("javascript:%s", event);
    }
}

void CPlugin::SetOnMediaComplete(const char *event)
{
    if (event_mediacomplete != NULL) {
        g_free(event_mediacomplete);
    }

    if (g_ascii_strncasecmp(event, "javascript:", 11) == 0) {
        event_mediacomplete = g_strdup_printf("%s", event);
    } else {
        event_mediacomplete = g_strdup_printf("javascript:%s", event);
    }

}

void CPlugin::SetOnMouseUp(const char *event)
{
    if (event_mouseup != NULL) {
        g_free(event_mouseup);
    }

    if (g_ascii_strncasecmp(event, "javascript:", 11) == 0) {
        event_mouseup = g_strdup_printf("%s", event);
    } else {
        event_mouseup = g_strdup_printf("javascript:%s", event);
    }
}

void CPlugin::SetOnMouseDown(const char *event)
{
    if (event_mousedown != NULL) {
        g_free(event_mousedown);
    }

    if (g_ascii_strncasecmp(event, "javascript:", 11) == 0) {
        event_mousedown = g_strdup_printf("%s", event);
    } else {
        event_mousedown = g_strdup_printf("javascript:%s", event);
    }
}

void CPlugin::SetOnMouseOut(const char *event)
{
    if (event_leavewindow != NULL) {
        g_free(event_leavewindow);
    }

    if (g_ascii_strncasecmp(event, "javascript:", 11) == 0) {
        event_leavewindow = g_strdup_printf("%s", event);
    } else {
        event_leavewindow = g_strdup_printf("javascript:%s", event);
    }
}

void CPlugin::SetOnMouseOver(const char *event)
{
    if (event_enterwindow != NULL) {
        g_free(event_enterwindow);
    }

    if (g_ascii_strncasecmp(event, "javascript:", 11) == 0) {
        event_enterwindow = g_strdup_printf("%s", event);
    } else {
        event_enterwindow = g_strdup_printf("javascript:%s", event);
    }
}

void CPlugin::SetOnDestroy(const char *event)
{
    if (event_destroy != NULL) {
        g_free(event_destroy);
    }

    if (g_ascii_strncasecmp(event, "javascript:", 11) == 0) {
        event_destroy = g_strdup_printf("%s", event);
    } else {
        event_destroy = g_strdup_printf("javascript:%s", event);
    }
}

#ifdef HAVE_CURL

int progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
    ListItem *item = (ListItem *) clientp;
    gdouble percent, rate;
    gchar *text;
    gboolean ok_to_play;
    gint id;
    gboolean ready;
    gboolean newwindow;
    CPlugin *plugin = (CPlugin *) item->plugin;
    gchar *path;

    // skip divide by zero issues
    if (dltotal == 0)
        return 0;               // keeps downloading

    if (item->cancelled) {
        printf("cancelling download at %f for %s\n", dlnow, item->src);
        return -1;              // cancels download
    }
    //printf("item ready = %i,player ready = %i,%f,%f,%f\n", item->playerready, plugin->playerready,
    //       dlnow, dltotal, dlnow / dltotal);

    item->localsize = dlnow;

    if (item->mediasize != dltotal)
        item->mediasize = dltotal;

    if (plugin->playerready) {
        percent = 0.0;
        if (item->mediasize > 0) {

            percent = (gdouble) item->localsize / (gdouble) item->mediasize;
            if (difftime(time(NULL), plugin->lastupdate) > 0.5) {
                printf("updating display id = %i\n", item->id);
                send_signal_with_double(plugin, item, "SetCachePercent", percent);
                rate =
                    (gdouble) ((item->localsize -
                                item->lastsize) / 1024.0) / (gdouble) difftime(time(NULL),
                                                                               plugin->lastupdate);
                if (percent > 0.99) {
                    text =
                        g_strdup_printf(_("Caching %iK (%0.1f K/s)"), item->mediasize / 1024, rate);
                } else {
                    text =
                        g_strdup_printf(_("Cache fill: %2.2f%% (%0.1f K/s)"), percent * 100.0,
                                        rate);
                }
                send_signal_with_string(plugin, item, "SetProgressText", text);
                if (!item->opened)
                    send_signal_with_string(plugin, item, "SetURL", item->src);
                if (plugin->post_dom_events && plugin->id != NULL) {
                    postDOMEvent(plugin->mInstance, plugin->id, "qt_progress");
                    postDOMEvent(plugin->mInstance, plugin->id, "qt_durationchange");
                }
                postPlayStateChange(plugin->mInstance, STATE_BUFFERING);

                time(&(plugin->lastupdate));
                item->lastsize = item->localsize;
            }
        }
        if (!item->opened) {
            if ((item->localsize >= (plugin->cache_size * 1024)) && (percent >= 0.2)) {
                printf("Setting to play because %i > %i\n", item->localsize,
                       plugin->cache_size * 1024);
                ok_to_play = TRUE;
            }
            if (ok_to_play == FALSE && (item->localsize > (plugin->cache_size * 2 * 1024))
                && (plugin->cache_size >= 512)) {
                printf("Setting to play because %i > %i (double cache)\n", item->localsize,
                       plugin->cache_size * 2 * 1024);
                ok_to_play = TRUE;
            }
            if (ok_to_play == FALSE) {
                if (item->bitrate == 0 && item->bitrate_requests < 5
                    && ((gint) (percent * 100) > item->bitrate_requests)) {
                    item->bitrate = request_bitrate(plugin, item, item->local);
                    item->bitrate_requests++;
                }
                if (item->bitrate > 0) {
                    if (item->localsize / item->bitrate >= 10 && (percent >= 0.2)) {
                        printf("Setting to play becuase %i >= 10\n",
                               item->localsize / item->bitrate);
                        ok_to_play = TRUE;
                        if (plugin->post_dom_events && plugin->id != NULL) {
                            postDOMEvent(plugin->mInstance, plugin->id, "qt_canplay");
                        }
                    }
                }
            }

        }
        // if not opened, over cache level and not an href target then try and open it
        if ((!item->opened) && ok_to_play == TRUE) {
            id = item->controlid;
            path = g_strdup(item->path);
            ready = item->playerready;
            newwindow = item->newwindow;
            if (!item->streaming)
                item->streaming = streaming(item->src);
            if (!item->streaming) {
                printf("in progress_callback\n");
                plugin->playlist = list_parse_qt(plugin->playlist, item);
                plugin->playlist = list_parse_asx(plugin->playlist, item);
                plugin->playlist = list_parse_qml(plugin->playlist, item);
                plugin->playlist = list_parse_ram(plugin->playlist, item);
            }
            printf("item->play = %i\n", item->play);
            printf("item->src = %s\n", item->src);
            printf("calling open_location from progress_callback\n");
            if (item->play) {
                send_signal_with_integer(plugin, item, "SetGUIState", PLAYING);
                open_location(plugin, item, TRUE);
                item->requested = TRUE;
                if (plugin->post_dom_events && plugin->id != NULL) {
                    postDOMEvent(plugin->mInstance, plugin->id, "qt_loadedfirstframe");
                    postDOMEvent(plugin->mInstance, plugin->id, "qt_canplay");
                    postDOMEvent(plugin->mInstance, plugin->id, "qt_play");
                }
            } else {
                item = list_find_next_playable(plugin->playlist);
                if (item != NULL) {
                    item->controlid = id;
                    g_strlcpy(item->path, path, 1024);
                    item->playerready = ready;
                    item->newwindow = newwindow;
                    item->cancelled = FALSE;
                    // printf("opening next playable items on the playlist\n");
                    if (item->streaming) {
                        open_location(plugin, item, FALSE);
                        item->requested = TRUE;
                    } else {
                        if (!item->requested) {
                            plugin->GetURLNotify(plugin->mInstance, item->src, NULL, item);
                            item->requested = TRUE;
                        }
                    }
                }
            }
            g_free(path);
        }

    }

    return 0;
}

gpointer CURLGetURLNotify(gpointer data)
{
    ListItem *item = (ListItem *) data;
    CPlugin *plugin = (CPlugin *) item->plugin;
    gchar *path;
    FILE *local;
    CURL *curl;
    gint id;
    gboolean ready;
    gboolean newwindow;

    if (item) {
        local = fopen(item->local, "wb");
        if (local) {
            curl = curl_easy_init();
            if (curl) {

                curl_easy_setopt(curl, CURLOPT_URL, item->src);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, local);
                curl_easy_setopt(curl, CURLOPT_USERAGENT, "QuickTime/7.6.9");
                curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_callback);
                curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, item);
                curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);

                curl_easy_perform(curl);
                curl_easy_cleanup(curl);
                printf("item retrieved using CURL\n");
                //plugin->URLNotify(item->src, NPRES_DONE, item);

            }
            fclose(local);
            printf("fetched %s to %s\n", item->src, item->local);
            send_signal_with_double(plugin, item, "SetCachePercent", 0.0);
            item->retrieved = TRUE;
        }


        if ((!item->opened)) {
            id = item->controlid;
            path = g_strdup(item->path);
            ready = item->playerready;
            newwindow = item->newwindow;
            if (!item->streaming)
                item->streaming = streaming(item->src);
            if (!item->streaming) {
                printf("in CURLGetURLNotify\n");
                plugin->playlist = list_parse_qt(plugin->playlist, item);
                plugin->playlist = list_parse_asx(plugin->playlist, item);
                plugin->playlist = list_parse_qml(plugin->playlist, item);
                plugin->playlist = list_parse_ram(plugin->playlist, item);
            }
            // printf("item->play = %i\n", item->play);
            // printf("item->src = %i\n", item->src);
            // printf("calling open_location from Write\n");
            if (item->play) {
                send_signal_with_integer(plugin, item, "SetGUIState", PLAYING);
                open_location(plugin, item, TRUE);
                item->requested = TRUE;
                if (plugin->post_dom_events && plugin->id != NULL) {
                    postDOMEvent(plugin->mInstance, plugin->id, "qt_loadedfirstframe");
                    postDOMEvent(plugin->mInstance, plugin->id, "qt_canplay");
                    postDOMEvent(plugin->mInstance, plugin->id, "qt_play");
                }
            } else {
                item = list_find_next_playable(plugin->playlist);
                if (item != NULL) {
                    item->controlid = id;
                    g_strlcpy(item->path, path, 1024);
                    item->playerready = ready;
                    item->newwindow = newwindow;
                    item->cancelled = FALSE;
                    // printf("opening next playable items on the playlist\n");
                    if (item->streaming) {
                        open_location(plugin, item, FALSE);
                        item->requested = TRUE;
                    } else {
                        if (!item->requested) {
                            plugin->GetURLNotify(plugin->mInstance, item->src, NULL, item);
                            item->requested = TRUE;
                        }
                    }
                }
            }
            g_free(path);

        }
    }
}


#endif


NPError CPlugin::GetURLNotify(NPP instance, const char *url, const char *target, void *notifyData)
{
#ifdef HAVE_CURL
    ListItem *item;
    gchar *path;
    gchar *tmp;
#endif

    if (g_strrstr(url, "apple.com") == NULL /* && this->quicktime_emulation == 0 */ ) {
        return NPN_GetURLNotify(instance, url, target, notifyData);
    } else {
#ifdef HAVE_CURL
        printf("using curl to retrieve data from apple.com site\n");
        printf("quicktime_emulation = %i\n", quicktime_emulation);

        item = (ListItem *) notifyData;
        // item = list_find(playlist, (gchar*)stream->url);
        if (item == NULL) {

            if (this->mode == NP_FULL) {
                // printf("adding new item %s\n",stream->url);
                item = g_new0(ListItem, 1);
                g_strlcpy(item->src, url, 1024);
                item->requested = TRUE;
                item->play = TRUE;
                if (!item->streaming)
                    item->streaming = streaming(item->src);
                playlist = g_list_append(playlist, item);
                notifyData = item;
            } else {
                printf("item is null\nstream url %s\n", url);
                return -1;
            }
        } else {
            if (g_ascii_strcasecmp(item->src, url) != 0) {
                g_strlcpy(item->src, url, 4096);
            }
        }

        if (item->cancelled) {
            printf("item has been cancelled\n");
            return -1;
        }

        if (strlen(item->local) == 0) {
            path = g_strdup_printf("%s/gnome-mplayer/plugin", g_get_user_cache_dir());
            if (!g_file_test(path, G_FILE_TEST_IS_DIR)) {
                g_mkdir_with_parents(path, 0775);
            }
            tmp = gm_tempname(path, "gecko-mediaplayerXXXXXX");
            g_snprintf(item->local, 1024, "%s", tmp);
            g_free(tmp);
            g_free(path);
        }

        if (item->retrieved) {
            printf("item is already retrieved\n");
            return -1;
        }

        item->plugin = (void *) this;

        g_thread_create(CURLGetURLNotify, item, FALSE, NULL);

        return NPERR_NO_ERROR;
#else
        printf
            ("Unable to set user agent for pulling data from apple.com, request will probably fail\n");
        return NPN_GetURLNotify(instance, url, target, notifyData);
#endif
    }
}


// ==============================
// ! Scriptability related code !
// ==============================
//
// here the plugin is asked by Mozilla to tell if it is scriptable
// we should return a valid interface id and a pointer to 
// nsScriptablePeer interface which we should have implemented
// and which should be defined in the corressponding *.xpt file
// in the bin/components folder
class ScriptablePluginObjectBase:public NPObject {
  public:
    ScriptablePluginObjectBase(NPP npp)
    :mNpp(npp) {
    }
    virtual ~ ScriptablePluginObjectBase() {
    }

    // Virtual NPObject hooks called through this base class. Override
    // as you see fit.

    virtual void Invalidate();
    virtual bool HasMethod(NPIdentifier name);
    virtual bool Invoke(NPIdentifier name, const NPVariant * args,
                        uint32_t argCount, NPVariant * result);
    virtual bool InvokeDefault(const NPVariant * args, uint32_t argCount, NPVariant * result);
    virtual bool HasProperty(NPIdentifier name);
    virtual bool GetProperty(NPIdentifier name, NPVariant * result);
    virtual bool SetProperty(NPIdentifier name, const NPVariant * value);
    virtual bool RemoveProperty(NPIdentifier name);

  public:

    static void _Deallocate(NPObject * npobj);
    static void _Invalidate(NPObject * npobj);
    static bool _HasMethod(NPObject * npobj, NPIdentifier name);
    static bool _Invoke(NPObject * npobj, NPIdentifier name,
                        const NPVariant * args, uint32_t argCount, NPVariant * result);
    static bool _InvokeDefault(NPObject * npobj, const NPVariant * args,
                               uint32_t argCount, NPVariant * result);
    static bool _HasProperty(NPObject * npobj, NPIdentifier name);
    static bool _GetProperty(NPObject * npobj, NPIdentifier name, NPVariant * result);
    static bool _SetProperty(NPObject * npobj, NPIdentifier name, const NPVariant * value);
    static bool _RemoveProperty(NPObject * npobj, NPIdentifier name);

  protected:
    NPP mNpp;
};

#define DECLARE_NPOBJECT_CLASS_WITH_BASE(_class, ctor)                        \
static NPClass s##_class##_NPClass = {                                        \
  NP_CLASS_STRUCT_VERSION,                                                    \
  ctor,                                                                       \
  ScriptablePluginObjectBase::_Deallocate,                                    \
  ScriptablePluginObjectBase::_Invalidate,                                    \
  ScriptablePluginObjectBase::_HasMethod,                                     \
  ScriptablePluginObjectBase::_Invoke,                                        \
  ScriptablePluginObjectBase::_InvokeDefault,                                 \
  ScriptablePluginObjectBase::_HasProperty,                                   \
  ScriptablePluginObjectBase::_GetProperty,                                   \
  ScriptablePluginObjectBase::_SetProperty,                                   \
  ScriptablePluginObjectBase::_RemoveProperty                                 \
}

#define GET_NPOBJECT_CLASS(_class) &s##_class##_NPClass

void ScriptablePluginObjectBase::Invalidate()
{
}

bool ScriptablePluginObjectBase::HasMethod(NPIdentifier name)
{
    return false;
}

bool ScriptablePluginObjectBase::Invoke(NPIdentifier name, const NPVariant * args,
                                        uint32_t argCount, NPVariant * result)
{
    return false;
}

bool ScriptablePluginObjectBase::InvokeDefault(const NPVariant * args,
                                               uint32_t argCount, NPVariant * result)
{
    return false;
}

bool ScriptablePluginObjectBase::HasProperty(NPIdentifier name)
{
    return false;
}

bool ScriptablePluginObjectBase::GetProperty(NPIdentifier name, NPVariant * result)
{

    VOID_TO_NPVARIANT(*result);

    return false;
}

bool ScriptablePluginObjectBase::SetProperty(NPIdentifier name, const NPVariant * value)
{
    return false;
}

bool ScriptablePluginObjectBase::RemoveProperty(NPIdentifier name)
{
    return false;
}

// static
void ScriptablePluginObjectBase::_Deallocate(NPObject * npobj)
{
    // Call the virtual destructor.
    delete(ScriptablePluginObjectBase *) npobj;
}

// static
void ScriptablePluginObjectBase::_Invalidate(NPObject * npobj)
{
    ((ScriptablePluginObjectBase *) npobj)->Invalidate();
}

// static
bool ScriptablePluginObjectBase::_HasMethod(NPObject * npobj, NPIdentifier name)
{
    return ((ScriptablePluginObjectBase *) npobj)->HasMethod(name);
}

// static
bool ScriptablePluginObjectBase::_Invoke(NPObject * npobj, NPIdentifier name,
                                         const NPVariant * args, uint32_t argCount,
                                         NPVariant * result)
{
    return ((ScriptablePluginObjectBase *) npobj)->Invoke(name, args, argCount, result);
}

// static
bool ScriptablePluginObjectBase::_InvokeDefault(NPObject * npobj,
                                                const NPVariant * args,
                                                uint32_t argCount, NPVariant * result)
{
    return ((ScriptablePluginObjectBase *) npobj)->InvokeDefault(args, argCount, result);
}

// static
bool ScriptablePluginObjectBase::_HasProperty(NPObject * npobj, NPIdentifier name)
{
    return ((ScriptablePluginObjectBase *) npobj)->HasProperty(name);
}

// static
bool ScriptablePluginObjectBase::_GetProperty(NPObject * npobj, NPIdentifier name,
                                              NPVariant * result)
{
    return ((ScriptablePluginObjectBase *) npobj)->GetProperty(name, result);
}

// static
bool ScriptablePluginObjectBase::_SetProperty(NPObject * npobj, NPIdentifier name,
                                              const NPVariant * value)
{
    return ((ScriptablePluginObjectBase *) npobj)->SetProperty(name, value);
}

// static
bool ScriptablePluginObjectBase::_RemoveProperty(NPObject * npobj, NPIdentifier name)
{
    return ((ScriptablePluginObjectBase *) npobj)->RemoveProperty(name);
}

class ScriptablePluginObjectControls:public ScriptablePluginObjectBase {
  public:
    ScriptablePluginObjectControls(NPP npp)
    :ScriptablePluginObjectBase(npp) {
    }
    virtual bool HasMethod(NPIdentifier name);
    virtual bool Invoke(NPIdentifier name, const NPVariant * args,
                        uint32_t argCount, NPVariant * result);
    virtual bool InvokeDefault(const NPVariant * args, uint32_t argCount, NPVariant * result);
    virtual bool HasProperty(NPIdentifier name);
    virtual bool GetProperty(NPIdentifier name, NPVariant * result);
    virtual bool SetProperty(NPIdentifier name, const NPVariant * value);

};

static NPObject *AllocateScriptablePluginObjectControls(NPP npp, NPClass * aClass)
{
    return new ScriptablePluginObjectControls(npp);
}

DECLARE_NPOBJECT_CLASS_WITH_BASE(ScriptablePluginObjectControls,
                                 AllocateScriptablePluginObjectControls);

bool ScriptablePluginObjectControls::HasMethod(NPIdentifier name)
{
    if (name == controls_play_id ||
        name == controls_pause_id ||
        name == controls_stop_id ||
        name == controls_fastForward_id ||
        name == controls_fastReverse_id || name == controls_step_id) {
        return true;
    } else {
        return false;
    }
}

bool ScriptablePluginObjectControls::Invoke(NPIdentifier name, const NPVariant * args,
                                            uint32_t argCount, NPVariant * result)
{
    CPlugin *pPlugin = (CPlugin *) mNpp->pdata;
    if (pPlugin == NULL) {
        printf("Can't find plugin pointer\n");
        return PR_FALSE;
    }

    if (name == controls_play_id) {
        pPlugin->Play();
        return PR_TRUE;
    }

    if (name == controls_pause_id) {
        pPlugin->Pause();
        return PR_TRUE;
    }

    if (name == controls_stop_id) {
        pPlugin->Stop();
        return PR_TRUE;
    }

    return false;
}

bool ScriptablePluginObjectControls::InvokeDefault(const NPVariant * args, uint32_t argCount,
                                                   NPVariant * result)
{
    printf("ScriptablePluginObject default method called!\n");

    STRINGZ_TO_NPVARIANT(strdup("default method return val"), *result);

    return PR_TRUE;
}

bool ScriptablePluginObjectControls::HasProperty(NPIdentifier name)
{
    if (name == controls_currentPosition_id || name == controls_currentItem_id) {
        return true;
    } else {
        return false;
    }
}

bool ScriptablePluginObjectControls::GetProperty(NPIdentifier name, NPVariant * result)
{
    double position;

    CPlugin *pPlugin = (CPlugin *) mNpp->pdata;
    if (pPlugin == NULL) {
        printf("Can't find plugin pointer\n");
        VOID_TO_NPVARIANT(*result);
        return false;
    }

    if (name == controls_currentPosition_id) {
        pPlugin->GetTime(&position);
        DOUBLE_TO_NPVARIANT(position, *result);
        return true;
    }

    if (name == controls_currentItem_id) {
        OBJECT_TO_NPVARIANT(pPlugin->GetScriptableObjectMedia(), *result);
        return true;
    }

    VOID_TO_NPVARIANT(*result);
    return false;
}

bool ScriptablePluginObjectControls::SetProperty(NPIdentifier name, const NPVariant * value)
{
    CPlugin *pPlugin = (CPlugin *) mNpp->pdata;
    if (pPlugin == NULL) {
        printf("Can't find plugin pointer\n");
        return false;
    }

    if (name == controls_currentPosition_id) {
        pPlugin->Seek(NPVARIANT_TO_DOUBLE(*value));
        return true;
    }

    return false;
}

class ScriptablePluginObjectMedia:public ScriptablePluginObjectBase {
  public:
    ScriptablePluginObjectMedia(NPP npp)
    :ScriptablePluginObjectBase(npp) {
    }
    virtual bool HasMethod(NPIdentifier name);
    virtual bool Invoke(NPIdentifier name, const NPVariant * args,
                        uint32_t argCount, NPVariant * result);
    virtual bool InvokeDefault(const NPVariant * args, uint32_t argCount, NPVariant * result);
    virtual bool HasProperty(NPIdentifier name);
    virtual bool GetProperty(NPIdentifier name, NPVariant * result);
    virtual bool SetProperty(NPIdentifier name, const NPVariant * value);

};

static NPObject *AllocateScriptablePluginObjectMedia(NPP npp, NPClass * aClass)
{
    return new ScriptablePluginObjectMedia(npp);
}

DECLARE_NPOBJECT_CLASS_WITH_BASE(ScriptablePluginObjectMedia, AllocateScriptablePluginObjectMedia);

bool ScriptablePluginObjectMedia::HasMethod(NPIdentifier name)
{
    if (name == media_getItemInfo_id) {
        return true;
    } else {
        return false;
    }
}

bool ScriptablePluginObjectMedia::Invoke(NPIdentifier name, const NPVariant * args,
                                         uint32_t argCount, NPVariant * result)
{
    CPlugin *pPlugin = (CPlugin *) mNpp->pdata;
    gchar *arg;
    int i;

    if (pPlugin == NULL) {
        printf("Can't find plugin pointer\n");
        return PR_FALSE;
    }

    if (name == media_getItemInfo_id) {
#ifdef HAVE_NEW_XULRUNNER
        arg = g_strdup(NPVARIANT_TO_STRING(args[0]).UTF8Characters);
#else
        arg = g_strdup(NPVARIANT_TO_STRING(args[0]).utf8characters);
#endif

        if (g_strcasecmp(arg, "title") == 0) {
            STRINGZ_TO_NPVARIANT(strdup
                                 (request_string_value(pPlugin, pPlugin->lastopened, "GetTitle")),
                                 *result);
        } else if (g_strcasecmp(arg, "uri") == 0) {
            STRINGZ_TO_NPVARIANT(strdup
                                 (request_string_value(pPlugin, pPlugin->lastopened, "GetURI")),
                                 *result);
        } else if (g_strcasecmp(arg, "bitrate") == 0) {
            pPlugin->GetBitrate(&i);
            INT32_TO_NPVARIANT(i, *result);
        } else {
            STRINGZ_TO_NPVARIANT(strdup("Unknown"), *result);
        }

        return true;
    }

    return false;
}

bool ScriptablePluginObjectMedia::InvokeDefault(const NPVariant * args, uint32_t argCount,
                                                NPVariant * result)
{
    printf("ScriptablePluginObject default method called!\n");

    STRINGZ_TO_NPVARIANT(strdup("default method return val"), *result);

    return PR_TRUE;
}

bool ScriptablePluginObjectMedia::HasProperty(NPIdentifier name)
{
    if (name == media_duration_id) {
        return true;
    } else {
        return false;
    }
}

bool ScriptablePluginObjectMedia::GetProperty(NPIdentifier name, NPVariant * result)
{
    double d;

    CPlugin *pPlugin = (CPlugin *) mNpp->pdata;
    if (pPlugin == NULL) {
        printf("Can't find plugin pointer\n");
        VOID_TO_NPVARIANT(*result);
        return false;
    }

    if (name == media_duration_id) {
        pPlugin->GetDuration(&d);
        DOUBLE_TO_NPVARIANT(d, *result);
        return true;
    }

    VOID_TO_NPVARIANT(*result);
    return false;
}

bool ScriptablePluginObjectMedia::SetProperty(NPIdentifier name, const NPVariant * value)
{
    CPlugin *pPlugin = (CPlugin *) mNpp->pdata;
    if (pPlugin == NULL) {
        printf("Can't find plugin pointer\n");
        return false;
    }

    if (name == media_duration_id) {
        return true;
    }

    return false;
}

class ScriptablePluginObjectSettings:public ScriptablePluginObjectBase {
  public:
    ScriptablePluginObjectSettings(NPP npp)
    :ScriptablePluginObjectBase(npp) {
    }
    virtual bool HasMethod(NPIdentifier name);
    virtual bool Invoke(NPIdentifier name, const NPVariant * args,
                        uint32_t argCount, NPVariant * result);
    virtual bool InvokeDefault(const NPVariant * args, uint32_t argCount, NPVariant * result);
    virtual bool HasProperty(NPIdentifier name);
    virtual bool GetProperty(NPIdentifier name, NPVariant * result);
    virtual bool SetProperty(NPIdentifier name, const NPVariant * value);

};

static NPObject *AllocateScriptablePluginObjectSettings(NPP npp, NPClass * aClass)
{
    return new ScriptablePluginObjectSettings(npp);
}

DECLARE_NPOBJECT_CLASS_WITH_BASE(ScriptablePluginObjectSettings,
                                 AllocateScriptablePluginObjectSettings);

bool ScriptablePluginObjectSettings::HasMethod(NPIdentifier name)
{
    return false;
}

bool ScriptablePluginObjectSettings::Invoke(NPIdentifier name, const NPVariant * args,
                                            uint32_t argCount, NPVariant * result)
{
    CPlugin *pPlugin = (CPlugin *) mNpp->pdata;
    if (pPlugin == NULL) {
        printf("Can't find plugin pointer\n");
        return PR_FALSE;
    }

    return false;
}

bool ScriptablePluginObjectSettings::InvokeDefault(const NPVariant * args, uint32_t argCount,
                                                   NPVariant * result)
{
    printf("ScriptablePluginObject default method called!\n");

    STRINGZ_TO_NPVARIANT(strdup("default method return val"), *result);

    return PR_TRUE;
}

bool ScriptablePluginObjectSettings::HasProperty(NPIdentifier name)
{
    if (name == settings_volume_id) {
        return true;
    } else {
        return false;
    }
}

bool ScriptablePluginObjectSettings::GetProperty(NPIdentifier name, NPVariant * result)
{
    double d;

    CPlugin *pPlugin = (CPlugin *) mNpp->pdata;
    if (pPlugin == NULL) {
        printf("Can't find plugin pointer\n");
        VOID_TO_NPVARIANT(*result);
        return false;
    }

    if (name == settings_volume_id) {
        pPlugin->GetVolume(&d);
        DOUBLE_TO_NPVARIANT(d, *result);
        return true;
    }

    VOID_TO_NPVARIANT(*result);
    return false;
}

bool ScriptablePluginObjectSettings::SetProperty(NPIdentifier name, const NPVariant * value)
{
    CPlugin *pPlugin = (CPlugin *) mNpp->pdata;
    if (pPlugin == NULL) {
        printf("Can't find plugin pointer\n");
        return false;
    }

    if (name == settings_volume_id) {
        if ((int) NPVARIANT_TO_DOUBLE(*value) == 0 && NPVARIANT_TO_INT32(*value) > 0) {
            pPlugin->SetVolume((double) NPVARIANT_TO_INT32(*value));
        } else {
            pPlugin->SetVolume(NPVARIANT_TO_DOUBLE(*value));
        }
        return true;
    }

    return false;
}

class ScriptablePluginObjectError:public ScriptablePluginObjectBase {
  public:
    ScriptablePluginObjectError(NPP npp)
    :ScriptablePluginObjectBase(npp) {
    }
    virtual bool HasMethod(NPIdentifier name);
    virtual bool Invoke(NPIdentifier name, const NPVariant * args,
                        uint32_t argCount, NPVariant * result);
    virtual bool InvokeDefault(const NPVariant * args, uint32_t argCount, NPVariant * result);
    virtual bool HasProperty(NPIdentifier name);
    virtual bool GetProperty(NPIdentifier name, NPVariant * result);
    virtual bool SetProperty(NPIdentifier name, const NPVariant * value);

};

static NPObject *AllocateScriptablePluginObjectError(NPP npp, NPClass * aClass)
{
    return new ScriptablePluginObjectError(npp);
}

DECLARE_NPOBJECT_CLASS_WITH_BASE(ScriptablePluginObjectError, AllocateScriptablePluginObjectError);

bool ScriptablePluginObjectError::HasMethod(NPIdentifier name)
{
    return false;
}

bool ScriptablePluginObjectError::Invoke(NPIdentifier name, const NPVariant * args,
                                         uint32_t argCount, NPVariant * result)
{
    CPlugin *pPlugin = (CPlugin *) mNpp->pdata;
    if (pPlugin == NULL) {
        printf("Can't find plugin pointer\n");
        return PR_FALSE;
    }

    return false;
}

bool ScriptablePluginObjectError::InvokeDefault(const NPVariant * args, uint32_t argCount,
                                                NPVariant * result)
{
    printf("ScriptablePluginObject default method called!\n");

    STRINGZ_TO_NPVARIANT(strdup("default method return val"), *result);

    return PR_TRUE;
}

bool ScriptablePluginObjectError::HasProperty(NPIdentifier name)
{
    if (name == error_errorCount_id) {
        return true;
    } else {
        return false;
    }
}

bool ScriptablePluginObjectError::GetProperty(NPIdentifier name, NPVariant * result)
{
    CPlugin *pPlugin = (CPlugin *) mNpp->pdata;
    if (pPlugin == NULL) {
        printf("Can't find plugin pointer\n");
        VOID_TO_NPVARIANT(*result);
        return false;
    }

    if (name == error_errorCount_id) {
        INT32_TO_NPVARIANT(0, *result);
        return true;
    }

    VOID_TO_NPVARIANT(*result);
    return false;
}

bool ScriptablePluginObjectError::SetProperty(NPIdentifier name, const NPVariant * value)
{
    CPlugin *pPlugin = (CPlugin *) mNpp->pdata;
    if (pPlugin == NULL) {
        printf("Can't find plugin pointer\n");
        return false;
    }

    if (name == settings_volume_id) {
        if ((int) NPVARIANT_TO_DOUBLE(*value) == 0 && NPVARIANT_TO_INT32(*value) > 0) {
            pPlugin->SetVolume((double) NPVARIANT_TO_INT32(*value));
        } else {
            pPlugin->SetVolume(NPVARIANT_TO_DOUBLE(*value));
        }
        return true;
    }

    return false;
}


class ScriptablePluginObject:public ScriptablePluginObjectBase {
  public:
    ScriptablePluginObject(NPP npp)
    :ScriptablePluginObjectBase(npp) {
    }
    virtual bool HasMethod(NPIdentifier name);
    virtual bool Invoke(NPIdentifier name, const NPVariant * args,
                        uint32_t argCount, NPVariant * result);
    virtual bool InvokeDefault(const NPVariant * args, uint32_t argCount, NPVariant * result);
    virtual bool HasProperty(NPIdentifier name);
    virtual bool GetProperty(NPIdentifier name, NPVariant * result);
    virtual bool SetProperty(NPIdentifier name, const NPVariant * value);

};

static NPObject *AllocateScriptablePluginObject(NPP npp, NPClass * aClass)
{
    return new ScriptablePluginObject(npp);
}

DECLARE_NPOBJECT_CLASS_WITH_BASE(ScriptablePluginObject, AllocateScriptablePluginObject);


bool ScriptablePluginObject::HasMethod(NPIdentifier name)
{
    if (name == Play_id ||
        name == PlayAt_id ||
        name == Pause_id ||
        name == PlayPause_id ||
        name == Stop_id ||
        name == quit_id ||
        name == DoPlay_id ||
        name == DoPause_id ||
        name == FastForward_id ||
        name == FastReverse_id ||
        name == ff_id ||
        name == rew_id ||
        name == rewind_id ||
        name == Seek_id ||
        name == Open_id ||
        name == SetVolume_id ||
        name == GetVolume_id ||
        name == SetFileName_id ||
        name == GetFileName_id ||
        name == SetIsLooping_id ||
        name == GetIsLooping_id ||
        name == SetAutoPlay_id ||
        name == GetAutoPlay_id ||
        name == SetHREF_id ||
        name == GetHREF_id ||
        name == SetURL_id ||
        name == GetURL_id ||
        name == GetMIMEType_id ||
        name == getTime_id ||
        name == getDuration_id ||
        name == getPercent_id ||
        name == getBitrate_id ||
        name == isplaying_id ||
        name == playlistAppend_id ||
        name == playlistClear_id ||
        name == onClick_id ||
        name == onMediaComplete_id ||
        name == onMouseUp_id ||
        name == onMouseDown_id ||
        name == onMouseOut_id || name == onMouseOver_id || name == onDestroy_id) {
        return true;
    } else {
        return false;
    }
}

bool ScriptablePluginObject::Invoke(NPIdentifier name, const NPVariant * args,
                                    uint32_t argCount, NPVariant * result)
{
    double d;
    char *s;
    bool b;
    int i;

    CPlugin *pPlugin = (CPlugin *) mNpp->pdata;
    if (pPlugin == NULL) {
        printf("Can't find plugin pointer\n");
        return PR_FALSE;
    }

    if (name == Play_id || name == DoPlay_id) {
        pPlugin->Play();
        return PR_TRUE;
    }

    if (name == PlayAt_id) {
        pPlugin->Play();
        if ((int) NPVARIANT_TO_DOUBLE(args[0]) == 0 && NPVARIANT_TO_INT32(args[0]) > 0) {
            pPlugin->Seek((double) NPVARIANT_TO_INT32(args[0]));
        } else {
            pPlugin->Seek(NPVARIANT_TO_DOUBLE(args[0]));
        }
        return PR_TRUE;
    }

    if (name == Pause_id || name == DoPause_id) {
        pPlugin->Pause();
        return PR_TRUE;
    }

    if (name == PlayPause_id) {
        pPlugin->PlayPause();
        return PR_TRUE;
    }

    if (name == Stop_id) {
        pPlugin->Stop();
        return PR_TRUE;
    }

    if (name == FastForward_id || name == ff_id) {
        pPlugin->FastForward();
        return PR_TRUE;
    }

    if (name == FastReverse_id || name == rew_id || name == rewind_id) {
        pPlugin->FastReverse();
        return PR_TRUE;
    }

    if (name == Seek_id) {

        if ((int) NPVARIANT_TO_DOUBLE(args[0]) == 0 && NPVARIANT_TO_INT32(args[0]) > 0) {
            pPlugin->Seek((double) NPVARIANT_TO_INT32(args[0]));
        } else {
            pPlugin->Seek(NPVARIANT_TO_DOUBLE(args[0]));
        }
        return PR_TRUE;
    }

    if (name == Open_id || name == SetFileName_id || name == SetHREF_id || name == SetURL_id) {
#ifdef HAVE_NEW_XULRUNNER
        pPlugin->SetFilename(NPVARIANT_TO_STRING(args[0]).UTF8Characters);
#else
        pPlugin->SetFilename(NPVARIANT_TO_STRING(args[0]).utf8characters);
#endif
        return PR_TRUE;
    }

    if (name == GetFileName_id || name == GetHREF_id || name == GetURL_id) {
        pPlugin->GetFilename(&s);
        STRINGZ_TO_NPVARIANT(s, *result);
        g_free(s);
        return PR_TRUE;
    }

    if (name == SetVolume_id) {
        if ((int) NPVARIANT_TO_DOUBLE(args[0]) == 0 && NPVARIANT_TO_INT32(args[0]) > 0) {
            pPlugin->SetVolume((double) NPVARIANT_TO_INT32(args[0]));
        } else {
            pPlugin->SetVolume(NPVARIANT_TO_DOUBLE(args[0]));
        }
        return PR_TRUE;
    }

    if (name == GetVolume_id) {
        pPlugin->GetVolume(&d);
        DOUBLE_TO_NPVARIANT(d, *result);
        return PR_TRUE;
    }

    if (name == SetIsLooping_id) {
        pPlugin->SetLoop(NPVARIANT_TO_BOOLEAN(args[0]));
        return PR_TRUE;
    }

    if (name == GetIsLooping_id) {
        pPlugin->GetLoop(&b);
        BOOLEAN_TO_NPVARIANT(b, *result);
        return PR_TRUE;
    }

    if (name == SetAutoPlay_id) {
        pPlugin->autostart = NPVARIANT_TO_BOOLEAN(args[0]);
        return PR_TRUE;
    }

    if (name == GetAutoPlay_id) {
        BOOLEAN_TO_NPVARIANT(pPlugin->autostart, *result);
        return PR_TRUE;
    }

    if (name == GetMIMEType_id) {
        pPlugin->GetMIMEType(&s);
        STRINGZ_TO_NPVARIANT(s, *result);
        g_free(s);
        return PR_TRUE;
    }

    if (name == getTime_id) {
        pPlugin->GetTime(&d);
        DOUBLE_TO_NPVARIANT(d, *result);
        return PR_TRUE;
    }

    if (name == getDuration_id) {
        pPlugin->GetDuration(&d);
        DOUBLE_TO_NPVARIANT(d, *result);
        return PR_TRUE;
    }

    if (name == getPercent_id) {
        pPlugin->GetPercent(&d);
        DOUBLE_TO_NPVARIANT(d, *result);
        return PR_TRUE;
    }

    if (name == getBitrate_id) {
        pPlugin->GetBitrate(&i);
        INT32_TO_NPVARIANT(i, *result);
        return PR_TRUE;
    }

    if (name == isplaying_id || name == playlistAppend_id || name == playlistClear_id) {
        return PR_TRUE;
    }

    if (name == onClick_id) {
#ifdef HAVE_NEW_XULRUNNER
        pPlugin->SetOnClick(NPVARIANT_TO_STRING(args[0]).UTF8Characters);
#else
        pPlugin->SetOnClick(NPVARIANT_TO_STRING(args[0]).utf8characters);
#endif
        return PR_TRUE;
    }

    if (name == onMediaComplete_id) {
#ifdef HAVE_NEW_XULRUNNER
        pPlugin->SetOnMediaComplete(NPVARIANT_TO_STRING(args[0]).UTF8Characters);
#else
        pPlugin->SetOnMediaComplete(NPVARIANT_TO_STRING(args[0]).utf8characters);
#endif
        return PR_TRUE;
    }

    if (name == onMouseUp_id) {
#ifdef HAVE_NEW_XULRUNNER
        pPlugin->SetOnMouseUp(NPVARIANT_TO_STRING(args[0]).UTF8Characters);
#else
        pPlugin->SetOnMouseUp(NPVARIANT_TO_STRING(args[0]).utf8characters);
#endif
        return PR_TRUE;
    }

    if (name == onMouseDown_id) {
#ifdef HAVE_NEW_XULRUNNER
        pPlugin->SetOnMouseDown(NPVARIANT_TO_STRING(args[0]).UTF8Characters);
#else
        pPlugin->SetOnMouseDown(NPVARIANT_TO_STRING(args[0]).utf8characters);
#endif
        return PR_TRUE;
    }

    if (name == onMouseOut_id) {
#ifdef HAVE_NEW_XULRUNNER
        pPlugin->SetOnMouseOut(NPVARIANT_TO_STRING(args[0]).UTF8Characters);
#else
        pPlugin->SetOnMouseOut(NPVARIANT_TO_STRING(args[0]).utf8characters);
#endif
        return PR_TRUE;
    }

    if (name == onMouseOver_id) {
#ifdef HAVE_NEW_XULRUNNER
        pPlugin->SetOnMouseOver(NPVARIANT_TO_STRING(args[0]).UTF8Characters);
#else
        pPlugin->SetOnMouseOver(NPVARIANT_TO_STRING(args[0]).utf8characters);
#endif
        return PR_TRUE;
    }

    if (name == onDestroy_id) {
#ifdef HAVE_NEW_XULRUNNER
        pPlugin->SetOnDestroy(NPVARIANT_TO_STRING(args[0]).UTF8Characters);
#else
        pPlugin->SetOnDestroy(NPVARIANT_TO_STRING(args[0]).utf8characters);
#endif
        return PR_TRUE;
    }

    return PR_FALSE;
}

bool ScriptablePluginObject::InvokeDefault(const NPVariant * args, uint32_t argCount,
                                           NPVariant * result)
{
    printf("ScriptablePluginObject default method called!\n");

    STRINGZ_TO_NPVARIANT(strdup("default method return val"), *result);

    return PR_TRUE;
}

bool ScriptablePluginObject::HasProperty(NPIdentifier name)
{
    if (name == filename_id
        || name == src_id
        || name == ShowControls_id
        || name == fullscreen_id
        || name == showlogo_id
        || name == playState_id
        || name == URL_id
        || name == versionInfo_id
        || name == status_id
        || name == controls_id || name == media_id || name == settings_id || name == error_id) {
        return true;
    } else {
        return false;
    }
}

bool ScriptablePluginObject::GetProperty(NPIdentifier name, NPVariant * result)
{
    char *filename;
    char *version;
    char *status;
    bool setting;
    PRInt32 state;

    CPlugin *pPlugin = (CPlugin *) mNpp->pdata;
    if (pPlugin == NULL) {
        printf("Can't find plugin pointer\n");
        VOID_TO_NPVARIANT(*result);
        return false;
    }

    if (name == filename_id || name == src_id || name == URL_id) {
        pPlugin->GetFilename(&filename);
        if (filename != NULL)
            STRINGZ_TO_NPVARIANT(filename, *result);
        return true;
    }

    if (name == versionInfo_id) {
        version = g_strdup("7.0.0.7777");
        STRINGZ_TO_NPVARIANT(version, *result);
        return true;
    }

    if (name == ShowControls_id) {
        pPlugin->GetShowControls(&setting);
        BOOLEAN_TO_NPVARIANT(setting, *result);
        return true;
    }

    if (name == fullscreen_id) {
        pPlugin->GetFullScreen(&setting);
        BOOLEAN_TO_NPVARIANT(setting, *result);
        return true;
    }

    if (name == showlogo_id) {
        setting = true;
        BOOLEAN_TO_NPVARIANT(setting, *result);
        return true;
    }

    if (name == playState_id) {
        pPlugin->GetPlayState(&state);
        INT32_TO_NPVARIANT(state, *result);
        return true;
    }

    if (name == status_id) {
        pPlugin->GetPlayState(&state);
        switch(state) {
            case STATE_PLAYING:
                status = g_strdup(_("Playing"));
                break;
            case STATE_PAUSED:
                status = g_strdup(_("Paused"));
                break;
            case STATE_STOPPED:
                status = g_strdup(_("Stopped"));
                break;
            case STATE_BUFFERING:
                status = g_strdup_printf(_("Buffering %2.1lf%%"), request_double_value(pPlugin, pPlugin->lastopened, "GetCachePercent") * 100.0);
                break;
            default:
                status = g_strdup(_("Unknown Status"));
        }
        // printf("Status = %s\n", status);
        STRINGZ_TO_NPVARIANT(status, *result);
        return true;
    }

    if (name == controls_id) {
        OBJECT_TO_NPVARIANT(pPlugin->GetScriptableObjectControls(), *result);
        return true;
    }

    if (name == media_id) {
        OBJECT_TO_NPVARIANT(pPlugin->GetScriptableObjectMedia(), *result);
        return true;
    }

    if (name == settings_id) {
        OBJECT_TO_NPVARIANT(pPlugin->GetScriptableObjectSettings(), *result);
        return true;
    }

    if (name == error_id) {
        OBJECT_TO_NPVARIANT(pPlugin->GetScriptableObjectError(), *result);
        return true;
    }

    VOID_TO_NPVARIANT(*result);
    return false;
}

bool ScriptablePluginObject::SetProperty(NPIdentifier name, const NPVariant * value)
{
    CPlugin *pPlugin = (CPlugin *) mNpp->pdata;
    if (pPlugin == NULL) {
        printf("Can't find plugin pointer\n");
        return false;
    }

    if (name == filename_id || name == src_id || name == URL_id) {
#ifdef HAVE_NEW_XULRUNNER
        pPlugin->SetFilename(NPVARIANT_TO_STRING(*value).UTF8Characters);
#else
        pPlugin->SetFilename(NPVARIANT_TO_STRING(*value).utf8characters);
#endif
        return true;
    }

    if (name == ShowControls_id) {
        pPlugin->SetShowControls(NPVARIANT_TO_BOOLEAN(*value));
        return true;
    }

    if (name == fullscreen_id) {
        pPlugin->SetFullScreen(NPVARIANT_TO_BOOLEAN(*value));
        return true;
    }

    if (name == showlogo_id) {
        return true;
    }

    if (name == playState_id) {
        // readonly property
        return true;
    }

    return false;
}

NPObject *CPlugin::GetScriptableObject()
{
    if (!m_pScriptableObject) {
        m_pScriptableObject =
            NPN_CreateObject(mInstance, GET_NPOBJECT_CLASS(ScriptablePluginObject));
    }

    if (m_pScriptableObject) {
        NPN_RetainObject(m_pScriptableObject);
    }

    return m_pScriptableObject;
}

NPObject *CPlugin::GetScriptableObjectControls()
{
    if (!m_pScriptableObjectControls) {
        m_pScriptableObjectControls =
            NPN_CreateObject(mInstance, GET_NPOBJECT_CLASS(ScriptablePluginObjectControls));
    }

    if (m_pScriptableObjectControls) {
        NPN_RetainObject(m_pScriptableObjectControls);
    }

    return m_pScriptableObjectControls;
}

NPObject *CPlugin::GetScriptableObjectMedia()
{
    if (!m_pScriptableObjectMedia) {
        m_pScriptableObjectMedia =
            NPN_CreateObject(mInstance, GET_NPOBJECT_CLASS(ScriptablePluginObjectMedia));
    }

    if (m_pScriptableObjectMedia) {
        NPN_RetainObject(m_pScriptableObjectMedia);
    }

    return m_pScriptableObjectMedia;
}

NPObject *CPlugin::GetScriptableObjectSettings()
{
    if (!m_pScriptableObjectSettings) {
        m_pScriptableObjectSettings =
            NPN_CreateObject(mInstance, GET_NPOBJECT_CLASS(ScriptablePluginObjectSettings));
    }

    if (m_pScriptableObjectSettings) {
        NPN_RetainObject(m_pScriptableObjectSettings);
    }

    return m_pScriptableObjectSettings;
}

NPObject *CPlugin::GetScriptableObjectError()
{
    if (!m_pScriptableObjectError) {
        m_pScriptableObjectError =
            NPN_CreateObject(mInstance, GET_NPOBJECT_CLASS(ScriptablePluginObjectError));
    }

    if (m_pScriptableObjectError) {
        NPN_RetainObject(m_pScriptableObjectError);
    }

    return m_pScriptableObjectError;
}
