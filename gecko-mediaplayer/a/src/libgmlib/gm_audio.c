/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * gm_audio.c
 * Copyright (C) Kevin DeKorte 2006 <kdekorte@gmail.com>
 * 
 * gm_audio.c is free software.
 * 
 * You may redistribute it and/or modify it under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 * 
 * gm_audio.c is distributed in the hope that it will be useful,
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

#include "gm_audio.h"

#ifdef HAVE_ASOUNDLIB
#include <asoundlib.h>
#endif
#ifdef HAVE_PULSEAUDIO
#include <pulse/pulseaudio.h>
#include <pulse/glib-mainloop.h>
#endif

#include <math.h>

GList *gm_audio_devices = NULL;
gboolean gm_audio_debug = FALSE;
void *gm_audio_server_volume_update_callback = NULL;
GmAudioType gm_audio_monitored_type = AUDIO_TYPE_UNKNOWN;

// private prototypes

#ifdef HAVE_ASOUNDLIB
gdouble get_alsa_volume(gchar * device, gchar * mixer);
gboolean set_alsa_volume(gchar * device, gchar * mixer, gdouble volume);
#endif

#ifdef HAVE_PULSEAUDIO
pa_context *gm_audio_context;
void gm_audio_context_state_callback(pa_context * gm_audio_context, gpointer data);
void gm_audio_pa_sink_update_volume_cb(pa_context * c, const pa_sink_info * i, int eol, gpointer data);
#endif


gboolean gm_audio_query_devices()
{

    gint card, err, dev;
    gchar *name = NULL;
    gchar *desc;
    gchar *mplayer_ao;
    AudioDevice *device;

#ifdef HAVE_ASOUNDLIB
    snd_pcm_stream_t stream = SND_PCM_STREAM_PLAYBACK;
    snd_ctl_t *handle;
    snd_ctl_card_info_t *info;
    snd_pcm_info_t *pcminfo;
    gchar *device_name;
#endif

    if (gm_audio_devices != NULL) {
        gm_audio_free();
    }

    device = g_new0(AudioDevice, 1);
    device->description = g_strdup(_("Default"));
    device->type = AUDIO_TYPE_SOFTVOL;
    device->mplayer_ao = g_strdup("");
    device->volume = 1.0;
    gm_audio_devices = g_list_append(gm_audio_devices, device);

    device = g_new0(AudioDevice, 1);
    device->description = g_strdup("ARTS");
    device->type = AUDIO_TYPE_SOFTVOL;
    device->mplayer_ao = g_strdup("arts");
    device->volume = 1.0;
    gm_audio_devices = g_list_append(gm_audio_devices, device);

    device = g_new0(AudioDevice, 1);
    device->description = g_strdup("ESD");
    device->type = AUDIO_TYPE_SOFTVOL;
    device->mplayer_ao = g_strdup("esd");
    device->volume = 1.0;
    gm_audio_devices = g_list_append(gm_audio_devices, device);

    device = g_new0(AudioDevice, 1);
    device->description = g_strdup("JACK");
    device->type = AUDIO_TYPE_SOFTVOL;
    device->mplayer_ao = g_strdup("jack");
    device->volume = 1.0;
    gm_audio_devices = g_list_append(gm_audio_devices, device);

    device = g_new0(AudioDevice, 1);
    device->description = g_strdup("OSS");
    device->type = AUDIO_TYPE_SOFTVOL;
    device->mplayer_ao = g_strdup("oss");
    device->volume = 1.0;
    gm_audio_devices = g_list_append(gm_audio_devices, device);

    device = g_new0(AudioDevice, 1);
    device->description = g_strdup("ALSA");
    device->type = AUDIO_TYPE_SOFTVOL;
    device->alsa_device_name = g_strdup("default");
    device->mplayer_ao = g_strdup("alsa");
    device->volume = 1.0;
    gm_audio_devices = g_list_append(gm_audio_devices, device);

    device = g_new0(AudioDevice, 1);
    device->description = g_strdup("PulseAudio");
    device->type = AUDIO_TYPE_SOFTVOL;
    device->mplayer_ao = g_strdup("pulse");
    device->volume = 1.0;
    gm_audio_devices = g_list_append(gm_audio_devices, device);

#ifdef __OpenBSD__
    device = g_new0(AudioDevice, 1);
    device->description = g_strdup("SNDIO");
    device->type = AUDIO_TYPE_SOFTVOL;
    device->mplayer_ao = g_strdup("sndio");
    device->volume = 1.0;
    gm_audio_devices = g_list_append(gm_audio_devices, device);

    device = g_new0(AudioDevice, 1);
    device->description = g_strdup("RTunes");
    device->type = AUDIO_TYPE_SOFTVOL;
    device->mplayer_ao = g_strdup("rtunes");
    device->volume = 1.0;
    gm_audio_devices = g_list_append(gm_audio_devices, device);
#endif

#ifdef HAVE_ASOUNDLIB
    snd_ctl_card_info_alloca(&info);
    snd_pcm_info_alloca(&pcminfo);

    card = -1;

    while (snd_card_next(&card) >= 0) {
        //printf("card = %i\n", card);
        if (card < 0)
            break;
        if (name != NULL) {
            free(name);
            name = NULL;
        }
        name = malloc(32);
        sprintf(name, "hw:%i", card);
        //printf("name = %s\n",name);

        if ((err = snd_ctl_open(&handle, name, 0)) < 0) {
            continue;
        }

        if ((err = snd_ctl_card_info(handle, info)) < 0) {
            snd_ctl_close(handle);
            continue;
        }

        dev = -1;
        while (1) {
            snd_ctl_pcm_next_device(handle, &dev);
            if (dev < 0)
                break;
            snd_pcm_info_set_device(pcminfo, dev);
            snd_pcm_info_set_subdevice(pcminfo, 0);
            snd_pcm_info_set_stream(pcminfo, stream);
            if ((err = snd_ctl_pcm_info(handle, pcminfo)) < 0) {
                continue;
            }

            desc = g_strdup_printf("%s (%s) (alsa)", snd_ctl_card_info_get_name(info), snd_pcm_info_get_name(pcminfo));
            device_name = g_strdup_printf("hw:%i", card);
            mplayer_ao = g_strdup_printf("alsa:device=hw=%i.%i", card, dev);

            device = g_new0(AudioDevice, 1);
            device->description = g_strdup(desc);
            device->type = AUDIO_TYPE_ALSA;
            device->alsa_card = card;
            device->alsa_device = dev;
            device->alsa_device_name = g_strdup(device_name);
            device->mplayer_ao = g_strdup(mplayer_ao);
            gm_audio_devices = g_list_append(gm_audio_devices, device);

            g_free(desc);
            g_free(mplayer_ao);

        }

        snd_ctl_close(handle);
        free(name);
        name = NULL;

    }

#endif


#ifdef HAVE_PULSEAUDIO

	pa_glib_mainloop *loop = pa_glib_mainloop_new(g_main_context_default());
    gm_audio_context = pa_context_new(pa_glib_mainloop_get_api(loop), "gm_audio_context");
    if (gm_audio_context) {
        pa_context_connect(gm_audio_context, NULL, 0, NULL);
        pa_context_set_state_callback(gm_audio_context, gm_audio_context_state_callback, gm_audio_devices);
    }
    // make sure the pulse events are done before we exit this function
    while (gtk_events_pending())
        gtk_main_iteration();

#endif

    return TRUE;
}

gboolean gm_audio_update_device(AudioDevice * device)
{
    gboolean ret = FALSE;
    GList *iter;
    AudioDevice *data;

    if (gm_audio_devices == NULL) {
        gm_audio_query_devices();
    }
    //printf("update device, looking for %s\n", device->description);

    device->type = AUDIO_TYPE_UNKNOWN;
    if (device->alsa_device_name != NULL) {
        g_free(device->alsa_device_name);
        device->alsa_device_name = NULL;
    }
    if (device->mplayer_ao != NULL) {
        g_free(device->mplayer_ao);
        device->mplayer_ao = NULL;
    }

    iter = gm_audio_devices;
    while (iter != NULL) {
        data = (AudioDevice *) iter->data;
        //printf("Checking %s\n", data->description);
        if ((device->description != NULL && g_ascii_strcasecmp(device->description, data->description) == 0)
            || ((device->description == NULL || g_ascii_strcasecmp(device->description, "") == 0)
                && g_ascii_strcasecmp(data->description, _("Default")) == 0)) {
            if (device->description == NULL || g_ascii_strcasecmp(device->description, "") == 0) {
                if (device->description != NULL) {
                    g_free(device->description);
                    device->description = NULL;
                }
                device->description = g_strdup(data->description);
            }
            device->type = data->type;
            device->alsa_card = data->alsa_card;
            device->alsa_device = data->alsa_device;
            device->pulse_index = data->pulse_index;
            device->pulse_channels = data->pulse_channels;
            if (device->type != AUDIO_TYPE_SOFTVOL) {
                device->volume = -1.0;
            } else {
                device->volume = data->volume;
            }
            device->alsa_device_name = g_strdup(data->alsa_device_name);
            device->mplayer_ao = g_strdup(data->mplayer_ao);
        }
        iter = iter->next;
    }

    return (device->type != AUDIO_TYPE_UNKNOWN);
}

void free_list_item(gpointer item, gpointer data)
{
    AudioDevice *device = (AudioDevice *) item;

    g_free(device->description);
    g_free(device->alsa_mixer);
    g_free(device->alsa_device_name);
    g_free(device->mplayer_ao);
}


gboolean gm_audio_free()
{
    if (gm_audio_devices) {
        g_list_foreach(gm_audio_devices, free_list_item, NULL);
        g_list_free(gm_audio_devices);
        gm_audio_devices = NULL;
    }
    return TRUE;
}

gdouble gm_audio_get_volume(AudioDevice * device)
{

#ifdef HAVE_ASOUNDLIB
    if (device->type == AUDIO_TYPE_ALSA) {
        device->volume = get_alsa_volume(device->alsa_device_name, device->alsa_mixer);
    }
#endif

#ifdef HAVE_PULSEAUDIO
    if (device->type == AUDIO_TYPE_PULSE) {
        if (gm_audio_context) {
            device->volume = -1.0;
            pa_context_get_sink_info_by_index(gm_audio_context, device->pulse_index, gm_audio_pa_sink_update_volume_cb,
                                              device);
        }
        while (gtk_events_pending() || device->volume == -1)
            gtk_main_iteration();
    }
#endif

    return device->volume;
}

gboolean gm_audio_set_volume(AudioDevice * device, gdouble volume)
{
#ifdef HAVE_PULSEAUDIO
    pa_cvolume cv;
#endif

#ifdef HAVE_ASOUNDLIB
    if (device->type == AUDIO_TYPE_ALSA) {
        set_alsa_volume(device->alsa_device_name, device->alsa_mixer, volume);
    }
#endif

#ifdef HAVE_PULSEAUDIO
    if (device->type == AUDIO_TYPE_PULSE) {
        if (gm_audio_context) {
            pa_cvolume_set(&cv, device->pulse_channels, volume * (gdouble) PA_VOLUME_NORM);
            pa_context_set_sink_volume_by_index(gm_audio_context, device->pulse_index, &cv, NULL, NULL);
        }
    }
#endif
    return TRUE;
}

gboolean gm_audio_alsa_monitor(gpointer data)
{
    AudioDevice *device = (AudioDevice *) data;
    gdouble old_volume;

    old_volume = device->volume;
    //printf("old volume = %f '%s' '%s'\n",old_volume,device->alsa_device_name, device->alsa_mixer);

#ifdef HAVE_ASOUNDLIB
    if (device->alsa_device_name && device->alsa_mixer)
        device->volume = get_alsa_volume(device->alsa_device_name, device->alsa_mixer);
#endif

    if (gm_audio_server_volume_update_callback && old_volume != device->volume)
        g_idle_add(gm_audio_server_volume_update_callback, NULL);

    //printf("in alsa monitor %f\n",device->volume);
    return device->type == AUDIO_TYPE_ALSA;
}

void gm_audio_set_server_volume_update_callback(AudioDevice * device, void *callback)
{
    gm_audio_server_volume_update_callback = callback;
    gm_audio_monitored_type = device->type;

    if (device->type == AUDIO_TYPE_ALSA) {
        g_timeout_add(100, gm_audio_alsa_monitor, device);
    }
}

#ifdef HAVE_PULSEAUDIO

void gm_audio_pa_sink_update_volume_cb(pa_context * c, const pa_sink_info * i, int eol, gpointer data)
{
    AudioDevice *device = (AudioDevice *) data;
    GList *iter;
    gdouble old_volume;

    //printf("gm_audio_pa_sink_update_volume_cb %p, %i, %p\n",i, eol,data);
    if (i) {
        if (device) {
            device->pulse_channels = i->volume.channels;
            device->volume = (gdouble) pa_cvolume_avg(&(i->volume)) / (gdouble) PA_VOLUME_NORM;
        } else {
            if (gm_audio_monitored_type == AUDIO_TYPE_PULSE) {
                iter = gm_audio_devices;
                while (iter != NULL) {
                    device = (AudioDevice *) iter->data;
                    if (i->index == device->pulse_index) {
                        old_volume = device->volume;
                        device->volume = (gdouble) pa_cvolume_avg(&(i->volume)) / (gdouble) PA_VOLUME_NORM;
                        // printf("updated %s volume to %f\n", device->description, device->volume);
                    }
                    iter = iter->next;
                }
                if (gm_audio_server_volume_update_callback && old_volume != device->volume)
                    g_idle_add(gm_audio_server_volume_update_callback, NULL);
            }
        }
        //printf("device volume = %f\n", device->volume);
    } else {
        // eol == -1 when the index requested is not found
        if (eol == -1 && device)
            device->volume = 0.0;
    }
}


void gm_audio_pa_sink_cb(pa_context * c, const pa_sink_info * i, int eol, gpointer data)
{
    gchar *desc;
    gchar *mplayer_ao;
    AudioDevice *device;

    if (i) {
        desc = g_strdup_printf("%s (PulseAudio)", i->description);
        mplayer_ao = g_strdup_printf("pulse::%i", i->index);

        device = g_new0(AudioDevice, 1);
        device->description = g_strdup(desc);
        device->type = AUDIO_TYPE_PULSE;
        device->pulse_index = i->index;
        device->pulse_channels = i->volume.channels;
        device->volume = (gdouble) pa_cvolume_avg(&(i->volume)) / (gdouble) PA_VOLUME_NORM;
        device->mplayer_ao = g_strdup(mplayer_ao);
        gm_audio_devices = g_list_append(gm_audio_devices, device);
        //printf("\n%s\n\nproperties\n%s",desc,pa_proplist_to_string(i->proplist));

        g_free(desc);
        g_free(mplayer_ao);


    }
}

void gm_audio_pa_subscribe_callback(pa_context * c, pa_subscription_event_type_t t, uint32_t index, void *userdata)
{
    // printf("subscribe_callback\n");
    switch (t & PA_SUBSCRIPTION_EVENT_FACILITY_MASK) {
    case PA_SUBSCRIPTION_EVENT_SINK:
        pa_context_get_sink_info_by_index(c, index, gm_audio_pa_sink_update_volume_cb, NULL);
        break;
    case PA_SUBSCRIPTION_EVENT_SERVER:
        //pa_context_get_server_info(context,pa_server_info_callback,NULL);
        break;
    default:
        //printf("index = %i\n",index);
        break;
    }
}

void gm_audio_context_state_callback(pa_context * c, gpointer data)
{
    //printf("context state callback\n");
    int i;

    switch (pa_context_get_state(c)) {
    case PA_CONTEXT_READY:{
            for (i = 0; i < 255; i++) {
                pa_context_get_sink_info_by_index(c, i, gm_audio_pa_sink_cb, data);
            }
            pa_context_set_subscribe_callback(c, gm_audio_pa_subscribe_callback, NULL);
            pa_context_subscribe(c, (pa_subscription_mask_t) (PA_SUBSCRIPTION_MASK_SINK | PA_SUBSCRIPTION_MASK_SERVER)
                                 , NULL, NULL);
        }

    default:
        return;
    }

}
#endif

#ifdef HAVE_ASOUNDLIB
gdouble get_alsa_volume(gchar * device, gchar * mixer)
{
    gdouble vol = -1.0;
    gint err;
    snd_mixer_t *mhandle;
    snd_mixer_elem_t *elem;
    snd_mixer_selem_id_t *sid;
    glong get_vol, pmin, pmax;
    gint playback;
    gfloat f_multi;
    gchar **local_mixer;

    if ((err = snd_mixer_open(&mhandle, 0)) < 0) {
        printf("Mixer open error %s\n", snd_strerror(err));
        return vol;
    }

    if ((err = snd_mixer_attach(mhandle, device)) < 0) {
        printf("Mixer attach error %s\n", snd_strerror(err));
        return vol;
    }

    if ((err = snd_mixer_selem_register(mhandle, NULL, NULL)) < 0) {
        printf("Mixer register error %s\n", snd_strerror(err));
        return vol;
    }

    if ((err = snd_mixer_load(mhandle)) < 0) {
        printf("Mixer load error %s\n", snd_strerror(err));
        return vol;
    }

    if (mixer != NULL && strlen(mixer) > 0) {
        snd_mixer_selem_id_malloc(&sid);
        local_mixer = g_strsplit(mixer, ",", 2);
        if (local_mixer[1] == NULL) {
            snd_mixer_selem_id_set_index(sid, 0);
        } else {
            snd_mixer_selem_id_set_index(sid, (gint) g_strtod(local_mixer[1], NULL));
        }
        if (local_mixer[0] == NULL) {
            snd_mixer_selem_id_set_name(sid, mixer);
        } else {
            snd_mixer_selem_id_set_name(sid, local_mixer[0]);
        }
        if (local_mixer != NULL)
            g_strfreev(local_mixer);

        elem = snd_mixer_find_selem(mhandle, sid);
        if (elem) {
            snd_mixer_selem_get_playback_volume_range(elem, &pmin, &pmax);
            f_multi = (100 / (float) (pmax - pmin));
            if (snd_mixer_selem_has_playback_switch(elem)) {
                snd_mixer_selem_get_playback_switch(elem, 0, &playback);
            } else {
                playback = 1;
            }
            snd_mixer_selem_get_playback_volume(elem, 0, &get_vol);
            if (playback == 1) {
                vol = (gdouble) ((get_vol - pmin) * f_multi);
            } else {
                vol = 0;
            }
            if (gm_audio_debug) {
                printf("Getting Volume \n");
                printf("%s Playback is %i\n", mixer, playback);
                printf("%s Range is %li to %li \n", mixer, pmin, pmax);
                printf("%s Current Volume %li, multiplier = %f\n", mixer, get_vol, f_multi);
                printf("Scaled Volume is %lf\n", vol);
            }
        }
        snd_mixer_selem_id_free(sid);
    }

    snd_mixer_detach(mhandle, device);
    snd_mixer_close(mhandle);

    vol = floor(vol + 0.5);
    vol = vol / 100.0;

    return vol;
}

gboolean set_alsa_volume(gchar * device, gchar * mixer, gdouble volume)
{

    gint err;
    snd_mixer_t *mhandle;
    snd_mixer_elem_t *elem;
    snd_mixer_selem_id_t *sid;
    glong set_vol = 0, pmin, pmax;
    gint playback;
    gfloat f_multi;
    gboolean found = FALSE;
    gchar **local_mixer;

    if ((err = snd_mixer_open(&mhandle, 0)) < 0) {
        printf("Mixer open error %s\n", snd_strerror(err));
        return found;
    }

    if ((err = snd_mixer_attach(mhandle, device)) < 0) {
        printf("Mixer attach error %s\n", snd_strerror(err));
        return found;
    }

    if ((err = snd_mixer_selem_register(mhandle, NULL, NULL)) < 0) {
        printf("Mixer register error %s\n", snd_strerror(err));
        return found;
    }

    if ((err = snd_mixer_load(mhandle)) < 0) {
        printf("Mixer load error %s\n", snd_strerror(err));
        return found;
    }

    if (mixer != NULL && strlen(mixer) > 0) {
        snd_mixer_selem_id_malloc(&sid);
        local_mixer = g_strsplit(mixer, ",", 2);
        if (local_mixer[1] == NULL) {
            snd_mixer_selem_id_set_index(sid, 0);
        } else {
            snd_mixer_selem_id_set_index(sid, (gint) g_strtod(local_mixer[1], NULL));
        }
        if (local_mixer[0] == NULL) {
            snd_mixer_selem_id_set_name(sid, mixer);
        } else {
            snd_mixer_selem_id_set_name(sid, local_mixer[0]);
        }
        if (local_mixer != NULL)
            g_strfreev(local_mixer);

        elem = snd_mixer_find_selem(mhandle, sid);
        if (elem) {
            snd_mixer_selem_get_playback_volume_range(elem, &pmin, &pmax);
            f_multi = ((float) (pmax - pmin));
            if (snd_mixer_selem_has_playback_switch(elem)) {
                if (volume == 0) {
                    snd_mixer_selem_set_playback_switch_all(elem, 0);
                } else {
                    snd_mixer_selem_set_playback_switch_all(elem, 1);
                }
                snd_mixer_selem_get_playback_switch(elem, 0, &playback);
            } else {
                playback = 1;
            }
            if (playback == 1) {
                set_vol = (gdouble) ((volume) * f_multi) + pmin;
                snd_mixer_selem_set_playback_volume_all(elem, set_vol);
            }
            if (gm_audio_debug) {
                printf("Setting Volume\n");
                printf("%s Playback is %i\n", mixer, playback);
                printf("%s Range is %li to %li \n", mixer, pmin, pmax);
                printf("%s Volume %f, multiplier = %f\n", mixer, volume, f_multi);
                printf("Scaled Volume is %li\n", set_vol);
            }
            found = TRUE;
        }
        snd_mixer_selem_id_free(sid);
    }

    snd_mixer_detach(mhandle, device);
    snd_mixer_close(mhandle);

    return found;

}
#endif
