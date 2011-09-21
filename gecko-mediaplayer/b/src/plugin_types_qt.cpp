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
    gchar MimeTypes[4000];
    GmPrefStore *store;
    gboolean qt_disabled = FALSE;

    g_type_init();
    store = gm_pref_store_new("gecko-mediaplayer");
    if (store != NULL) {
        qt_disabled = gm_pref_store_get_boolean(store, DISABLE_QT);
        gm_pref_store_free(store);
    }

    if (qt_disabled) {
        return NULL;
    } else {
        g_strlcpy(MimeTypes,
                  "video/quicktime:mov:Quicktime;"
                  "video/x-quicktime:mov:Quicktime;"
                  "image/x-quicktime:mov:Quicktime;"
                  "video/quicktime:mp4:Quicktime;"
                  "video/quicktime:sdp:Quicktime - Session Description Protocol;"
                  "application/x-quicktimeplayer:mov:Quicktime;", sizeof(MimeTypes));

        return g_strdup(MimeTypes);
    }
}

NPError PluginGetValue(NPPVariable variable, void *value)
{
    NPError err = NPERR_NO_ERROR;

    // some sites use this description to figure out what formats can be played. So we have to make sure the 
    // description matches the features

    if (variable == NPPVpluginNameString) {
        *((const char **) value) = "QuickTime Plug-in 7.6.9";
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
    //printf("calling pluginSpecific for qt plugin\n");
    //setPreference(instance, "general.useragent.override", "QuickTime/7.6.9");
    printf("Loading QuickTime compatibility\n");
    instance->quicktime_emulation = TRUE;
}
