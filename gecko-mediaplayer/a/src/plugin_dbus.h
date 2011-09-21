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

#ifndef _PLUGIN_DBUS_H
#define _PLUGIN_DBUS_H

#include <stdio.h>
#include <glib.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib-lowlevel.h>
#include <unistd.h>
// #include <dbus/dbus-qt.h>
#include "plugin.h"
#include "plugin_list.h"

// extern void dbus_connection_setup_with_qt_main (DBusConnection *connection);

DBusConnection *dbus_hookup(CPlugin * instance);
DBusConnection *dbus_unhook(DBusConnection * connection, CPlugin * instance);
void open_location(CPlugin * instance, ListItem * item, gboolean uselocal);
void resize_window(CPlugin * instance, ListItem * item, gint x, gint y);
void send_signal(CPlugin * instance, ListItem * item, const gchar * signal);
void send_signal_when_ready(CPlugin * instance, ListItem * item, const gchar * signal);
void send_signal_with_string(CPlugin * instance, ListItem * item, const gchar * signal,
                             gchar * str);
void send_signal_with_double(CPlugin * instance, ListItem * item, const gchar * signal,
                             gdouble dbl);
void send_signal_with_boolean(CPlugin * instance, ListItem * item, const gchar * signal,
                              gboolean boolean);
void send_signal_with_integer(CPlugin * instance, ListItem * item, const gchar * signal, gint val);
gboolean request_boolean_value(CPlugin * instance, ListItem * item, const gchar * member);
gdouble request_double_value(CPlugin * instance, ListItem * item, const gchar * member);
gint request_int_value(CPlugin * instance, ListItem * item, const gchar * member);
gchar *request_string_value(CPlugin * instance, ListItem * item, const gchar * member);
gboolean is_valid_path(CPlugin * instance, const char *message);
gint request_bitrate(CPlugin * instance, ListItem * item, gchar * name);

#endif                          // _PLUGIN_DBUS_H
