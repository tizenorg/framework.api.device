/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <vconf.h>
#include <gio/gio.h>

#include "callback.h"
#include "battery.h"
#include "display.h"
#include "common.h"
#include "dbus.h"
#include "list.h"

#define SIGNAL_FLASH_STATE  "ChangeFlashState"

struct device_cb_info {
	device_changed_cb cb;
	void *data;
};

static dd_list *device_cb_list[DEVICE_CALLBACK_MAX];
static int flash_sigid;

static void battery_capacity_cb(keynode_t *key, void *data)
{
	static device_callback_e type = DEVICE_CALLBACK_BATTERY_CAPACITY;
	struct device_cb_info *cb_info;
	dd_list *elem;
	int val;

	val = vconf_keynode_get_int(key);

	/* invoke the each callback with value */
	DD_LIST_FOREACH(device_cb_list[type], elem, cb_info)
		cb_info->cb(type, (void*)val, cb_info->data);
}

static void battery_charging_cb(keynode_t *key, void *data)
{
	static device_callback_e type = DEVICE_CALLBACK_BATTERY_CHARGING;
	struct device_cb_info *cb_info;
	dd_list *elem;
	int val;

	val = vconf_keynode_get_int(key);

	/* invoke the each callback with value */
	DD_LIST_FOREACH(device_cb_list[type], elem, cb_info)
		cb_info->cb(type, (void*)val, cb_info->data);
}

static void battery_level_cb(keynode_t *key, void *data)
{
	static device_callback_e type = DEVICE_CALLBACK_BATTERY_LEVEL;
	struct device_cb_info *cb_info;
	dd_list *elem;
	int val, status;

	val = vconf_keynode_get_int(key);

	if (val == VCONFKEY_SYSMAN_BAT_LEVEL_EMPTY)
		status = DEVICE_BATTERY_LEVEL_EMPTY;
	else if (val == VCONFKEY_SYSMAN_BAT_LEVEL_CRITICAL)
		status = DEVICE_BATTERY_LEVEL_CRITICAL;
	else if (val == VCONFKEY_SYSMAN_BAT_LEVEL_LOW)
		status = DEVICE_BATTERY_LEVEL_LOW;
	else if (val == VCONFKEY_SYSMAN_BAT_LEVEL_HIGH)
		status = DEVICE_BATTERY_LEVEL_HIGH;
	else if (val == VCONFKEY_SYSMAN_BAT_LEVEL_FULL)
		status = DEVICE_BATTERY_LEVEL_FULL;
	else
		status = -1;

	/* invoke the each callback with value */
	DD_LIST_FOREACH(device_cb_list[type], elem, cb_info)
		cb_info->cb(type, (void*)status, cb_info->data);
}

static void display_changed_cb(keynode_t *key, void *data)
{
	static device_callback_e type = DEVICE_CALLBACK_DISPLAY_STATE;
	struct device_cb_info *cb_info;
	dd_list *elem;
	display_state_e state;
	int val;

	val = vconf_keynode_get_int(key);

	switch(val) {
	case 1: state = DISPLAY_STATE_NORMAL;
			break;
	case 2: state = DISPLAY_STATE_SCREEN_DIM;
			break;
	case 3: state = DISPLAY_STATE_SCREEN_OFF;
			break;
	default: state = -1;
			break;
	}

	/* invoke the each callback with value */
	DD_LIST_FOREACH(device_cb_list[type], elem, cb_info)
		cb_info->cb(type, (void*)state, cb_info->data);
}

static void flash_state_cb(GDBusConnection *conn,
		const gchar *sender,
		const gchar *object,
		const gchar *interface,
		const gchar *signal,
		GVariant *parameters,
		gpointer user_data)
{
	static int type = DEVICE_CALLBACK_FLASH_BRIGHTNESS;
	struct device_cb_info *cb_info;
	dd_list *elem;
	int val;

	if (strncmp(signal, SIGNAL_FLASH_STATE,
				sizeof(SIGNAL_FLASH_STATE)) != 0) {
		_E("wrong parameter : signal(%s)", signal);
		return;
	}

	/* get camera value */
	g_variant_get(parameters, "(i)", &val);
	_D("%s - %d", signal, val);

	/* invoke the each callback with value */
	DD_LIST_FOREACH(device_cb_list[type], elem, cb_info)
		cb_info->cb(type, (void*)val, cb_info->data);
}

static int register_signal(const char *bus_name,
		const char *object_path,
		const char *interface_name,
		const char *signal,
		GDBusSignalCallback callback,
		int *sig_id)
{
	GError *err = NULL;
	GDBusConnection *conn;
	int id;

#if !GLIB_CHECK_VERSION(2,35,0)
	g_type_init();
#endif

	conn = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &err);
	if (!conn) {
		_E("fail to get dbus connection : %s", err->message);
		g_clear_error(&err);
		return -EPERM;
	}

	/* subscribe signal */
	id = g_dbus_connection_signal_subscribe(conn,
			bus_name,
			interface_name,
			signal,		/* null to match on all signals */
			object_path,
			NULL,		/* null to match on all kinds of arguments */
			G_DBUS_SIGNAL_FLAGS_NONE,
			callback,
			NULL,
			NULL);
	if (id == 0) {
		_E("fail to connect %s signal", signal);
		return -EPERM;
	}

	if (sig_id)
		*sig_id = id;

	return 0;
}

static int unregister_signal(int *sig_id)
{
	GError *err = NULL;
	GDBusConnection *conn;

	if (!sig_id)
		return -EINVAL;

	conn = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &err);
	if (!conn) {
		_E("fail to get dbus connection : %s", err->message);
		g_clear_error(&err);
		return -EPERM;
	}

	/* unsubscribe signal */
	g_dbus_connection_signal_unsubscribe(conn, *sig_id);
	*sig_id = 0;

	return 0;
}

static int register_request(device_callback_e type)
{
	switch (type) {
	case DEVICE_CALLBACK_BATTERY_CAPACITY:
		return vconf_notify_key_changed(VCONFKEY_SYSMAN_BATTERY_CAPACITY,
				battery_capacity_cb, NULL);
	case DEVICE_CALLBACK_BATTERY_LEVEL:
		return vconf_notify_key_changed(VCONFKEY_SYSMAN_BATTERY_LEVEL_STATUS,
				battery_level_cb, NULL);
	case DEVICE_CALLBACK_BATTERY_CHARGING:
		return vconf_notify_key_changed(VCONFKEY_SYSMAN_BATTERY_CHARGE_NOW,
				battery_charging_cb, NULL);
	case DEVICE_CALLBACK_DISPLAY_STATE:
		return vconf_notify_key_changed(VCONFKEY_PM_STATE,
				display_changed_cb, NULL);
	case DEVICE_CALLBACK_FLASH_BRIGHTNESS:
		/* sig_id begins with 1. */
		if (flash_sigid)
			return -EEXIST;
		return register_signal(DEVICED_BUS_NAME,
				DEVICED_PATH_LED,
				DEVICED_INTERFACE_LED,
				SIGNAL_FLASH_STATE, flash_state_cb, &flash_sigid);
	default:
		break;
	}

	return -EINVAL;
}

static int release_request(device_callback_e type)
{
	switch (type) {
	case DEVICE_CALLBACK_BATTERY_CAPACITY:
		return vconf_ignore_key_changed(VCONFKEY_SYSMAN_BATTERY_CAPACITY,
				battery_capacity_cb);
	case DEVICE_CALLBACK_BATTERY_LEVEL:
		return vconf_ignore_key_changed(VCONFKEY_SYSMAN_BATTERY_LEVEL_STATUS,
				battery_level_cb);
	case DEVICE_CALLBACK_BATTERY_CHARGING:
		return vconf_ignore_key_changed(VCONFKEY_SYSMAN_BATTERY_CHARGE_NOW,
				battery_charging_cb);
	case DEVICE_CALLBACK_DISPLAY_STATE:
		return vconf_ignore_key_changed(VCONFKEY_PM_STATE,
				display_changed_cb);
	case DEVICE_CALLBACK_FLASH_BRIGHTNESS:
		if (!flash_sigid)
			return -ENOENT;
		return unregister_signal(&flash_sigid);
	default:
		break;
	}

	return -EINVAL;
}

int device_add_callback(device_callback_e type, device_changed_cb cb, void *data)
{
	struct device_cb_info *cb_info;
	dd_list *elem;
	int ret, n;

	if (type < 0 || type >= DEVICE_CALLBACK_MAX)
		return DEVICE_ERROR_INVALID_PARAMETER;

	if (!cb)
		return DEVICE_ERROR_INVALID_PARAMETER;

	/* check if it is the first request */
	n = DD_LIST_LENGTH(device_cb_list[type]);
	if (n == 0) {
		ret = register_request(type);
		if (ret < 0)
			return DEVICE_ERROR_OPERATION_FAILED;
	}

	/* check for the same request */
	DD_LIST_FOREACH(device_cb_list[type], elem, cb_info) {
		if (cb_info->cb == cb)
			return DEVICE_ERROR_ALREADY_IN_PROGRESS;
	}

	/* add device changed callback to list (local) */
	cb_info = malloc(sizeof(struct device_cb_info));
	if (!cb_info)
		return DEVICE_ERROR_OPERATION_FAILED;

	cb_info->cb = cb;
	cb_info->data = data;

	DD_LIST_APPEND(device_cb_list[type], cb_info);

	return DEVICE_ERROR_NONE;
}

int device_remove_callback(device_callback_e type, device_changed_cb cb)
{
	struct device_cb_info *cb_info;
	dd_list *elem;
	int ret, n;

	if (type < 0 || type >= DEVICE_CALLBACK_MAX)
		return DEVICE_ERROR_INVALID_PARAMETER;

	if (!cb)
		return DEVICE_ERROR_INVALID_PARAMETER;

	/* search for the same element with callback */
	DD_LIST_FOREACH(device_cb_list[type], elem, cb_info) {
		if (cb_info->cb == cb)
			break;
	}

	if (!cb_info)
		return DEVICE_ERROR_INVALID_PARAMETER;

	/* remove device callback from list (local) */
	DD_LIST_REMOVE(device_cb_list[type], cb_info);
	free(cb_info);

	/* check if this callback is last element */
	n = DD_LIST_LENGTH(device_cb_list[type]);
	if (n == 0) {
		ret = release_request(type);
		if (ret < 0)
			return DEVICE_ERROR_OPERATION_FAILED;
	}

	return DEVICE_ERROR_NONE;
}
