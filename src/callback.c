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

#include "callback.h"
#include "battery.h"
#include "display.h"
#include "common.h"
#include "dbus.h"
#include "list.h"

struct device_cb_info {
	device_changed_cb cb;
	void *data;
};

static dd_list *device_cb_list[DEVICE_CALLBACK_MAX];

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
