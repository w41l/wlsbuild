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

#include "plugin_types.h"

gchar *GetMIMEDescription()
{
    gchar MimeTypes[8192];
    GmPrefStore *store;
    gboolean midi_disabled = FALSE;

    g_type_init();
    store = gm_pref_store_new("gecko-mediaplayer");
    if (store != NULL) {
        midi_disabled = gm_pref_store_get_boolean(store, DISABLE_MIDI);
        gm_pref_store_free(store);
    }

    g_strlcpy(MimeTypes,
              "audio/x-mpegurl:m3u:MPEG Playlist;"
              "video/mpeg:mpg,mpeg:MPEG;"
              "audio/mpeg:mpg,mpeg:MPEG;"
              "video/x-mpeg:mpg,mpeg:MPEG;"
              "video/x-mpeg2:mpv2,mp2ve:MPEG2;"
              "audio/mpeg:mpg,mpeg:MPEG;"
              "audio/x-mpeg:mpg,mpeg:MPEG;"
              "audio/mpeg2:mp2:MPEG audio;"
              "audio/x-mpeg2:mp2:MPEG audio;"
              "audio/mp4:mp4:MPEG 4 audio;"
              "audio/x-mp4:mp4:MPEG 4 audio;"
              "video/mp4:mp4:MPEG 4 Video;"
              "video/x-m4v:m4v:MPEG 4 Video;"
              "video/3gpp:mp4,3gp:MPEG 4 Video;", sizeof(MimeTypes));
    g_strlcat(MimeTypes,
              "audio/mpeg3:mp3:MPEG audio;" "audio/x-mpeg3:mp3:MPEG audio;"
              "audio/x-mpegurl:m3u:MPEG url;" "audio/mp3:mp3:MPEG audio;", sizeof(MimeTypes));
    g_strlcat(MimeTypes,
              "application/x-ogg:ogg,oga,ogm:Ogg Vorbis Media;"
              "application/ogg:ogg,oga,ogm:Ogg Vorbis Media;"
              "audio/x-ogg:ogg,oga:Ogg Vorbis Audio;"
              "audio/ogg:ogg,oga:Ogg Vorbis Audio;"
              "video/x-ogg:ogg,ogm:Ogg Vorbis Video;"
              "video/ogg:ogg,ogm:Ogg Vorbis Video;", sizeof(MimeTypes));

    // VLC
    g_strlcat(MimeTypes,
              "application/x-vlc-plugin:vlc:VLC plug-in;"
              "application/x-google-vlc-plugin::Google VLC plug-in;", sizeof(MimeTypes));

    // FLAC
    g_strlcat(MimeTypes,
              "audio/flac:flac:FLAC Audio;" "audio/x-flac:flac:FLAC Audio;", sizeof(MimeTypes));

    // FLI
    g_strlcat(MimeTypes,
              "video/fli:fli,flc:FLI animation;"
              "video/x-fli:fli,flc:FLI animation;", sizeof(MimeTypes));

    // FLV
    g_strlcat(MimeTypes, "video/x-flv:flv:Flash Video;"
              "video/flv:flv:Flash Video;", sizeof(MimeTypes));

    // Vivo
    g_strlcat(MimeTypes, "video/vnd.vivo:viv,vivo:VivoActive;", sizeof(MimeTypes));

    // Matroska
    g_strlcat(MimeTypes, "audio/x-matroska:mka:Matroska Audio;", sizeof(MimeTypes));
    g_strlcat(MimeTypes, "video/x-matroska:mkv:Matroska Video;", sizeof(MimeTypes));

    // NSV
    g_strlcat(MimeTypes,
              "application/x-nsv-vp3-mp3:nsv:Nullsoft Streaming Video;", sizeof(MimeTypes));

    // Soundtracker
    g_strlcat(MimeTypes, "audio/x-mod:mod:Soundtracker;", sizeof(MimeTypes));

    // AIFF
    g_strlcat(MimeTypes, "audio/x-aiff:aif:AIFF Audio;", sizeof(MimeTypes));

    // Basic
    g_strlcat(MimeTypes,
              "audio/basic:au,snd:Basic Audio File;"
              "audio/x-basic:au,snd:Basic Audio File;", sizeof(MimeTypes));
    // MIDI
    if (!midi_disabled)
        g_strlcat(MimeTypes, "audio/midi:mid,midi,kar:MIDI Audio;", sizeof(MimeTypes));

    // Playlist
    g_strlcat(MimeTypes, "audio/x-scpls:pls:Shoutcast Playlist;", sizeof(MimeTypes));

    // MNG
    g_strlcat(MimeTypes, "video/x-mng:mng:Multiple-Image Network Graphics;", sizeof(MimeTypes));

    // WEBM
    g_strlcat(MimeTypes,
              "audio/webm:webm:WEBM audio File;"
              "audio/x-webm:webm:WEBM audio File;"
              "video/webm:webm:WEBM video File;"
              "video/x-webm:webm:WEBM video File;", sizeof(MimeTypes));

    return g_strdup(MimeTypes);
}

NPError PluginGetValue(NPPVariable variable, void *value)
{
    NPError err = NPERR_NO_ERROR;

    // some sites use this description to figure out what formats can be played. So we have to make sure the 
    // description matches the features
    if (variable == NPPVpluginNameString) {
        *((const char **) value) = "mplayerplug-in is now gecko-mediaplayer " VERSION;
    }
    if (variable == NPPVpluginDescriptionString) {
        *((const char **) value) =
            "<a href=\"http://kdekorte.googlepages.com/gecko-mediaplayer\">Gecko Media Player</a> "
            VERSION
            "<br><br>Video Player Plug-in for QuickTime, RealPlayer and Windows Media Player streams using <a href=\"http://mplayerhq.hu\">MPlayer</a>";

    }

    if (variable == NPPVpluginNeedsXEmbed) {
        *((bool *) value) = TRUE;
    }

    if ((variable != NPPVpluginNameString)
        && (variable != NPPVpluginDescriptionString)
        && (variable != NPPVpluginNeedsXEmbed)) {
        err = NPERR_INVALID_PARAM;
    }

    return err;

}

void pluginSpecific(CPlugin * instance)
{
    instance->quicktime_emulation = FALSE;
}
