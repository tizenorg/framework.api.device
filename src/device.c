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
#include <string.h>
#include <errno.h>
#include <vconf.h>

#include "device.h"
#include "common.h"
#include "dbus.h"
#include "display.h"

#define METHOD_HOLD_BRIGHTNESS          "HoldBrightness"
#define METHOD_RELEASE_BRIGHTNESS       "ReleaseBrightness"

int device_get_display_numbers(int* device_number)
{
	return device_display_get_numbers(device_number);
}

int device_get_brightness(int disp_idx, int* value)
{
	return device_display_get_brightness(disp_idx, value);
}

int device_set_brightness(int disp_idx, int new_value)
{
	int max_value, max_id, ret;
	char str_val[32];
	char *arr[1];

	if(new_value < 0)
		return DEVICE_ERROR_INVALID_PARAMETER;

	ret = device_get_display_numbers(&max_id);
	if (ret != DEVICE_ERROR_NONE)
		return ret;

	if(disp_idx < 0 || disp_idx >= max_id)
		return DEVICE_ERROR_INVALID_PARAMETER;

	ret = device_get_max_brightness(disp_idx, &max_value);
	if (ret != DEVICE_ERROR_NONE)
		return ret;

	if(new_value > max_value)
		return DEVICE_ERROR_INVALID_PARAMETER;

	snprintf(str_val, sizeof(str_val), "%d", new_value);
	arr[0] = str_val;

	ret = dbus_method_sync(DEVICED_BUS_NAME,
			DEVICED_PATH_DISPLAY, DEVICED_INTERFACE_DISPLAY,
			METHOD_HOLD_BRIGHTNESS, "i", arr);
	return errno_to_device_error(ret);
}

int device_get_max_brightness(int disp_idx, int* max_value)
{
	return device_display_get_max_brightness(disp_idx, max_value);
}

int device_set_brightness_from_settings(int disp_idx)
{
	int max_id, ret;

	ret = device_get_display_numbers(&max_id);
	if (ret != DEVICE_ERROR_NONE)
		return ret;

	if(disp_idx < 0 || disp_idx >= max_id)
		return DEVICE_ERROR_INVALID_PARAMETER;

	ret = dbus_method_sync(DEVICED_BUS_NAME,
			DEVICED_PATH_DISPLAY, DEVICED_INTERFACE_DISPLAY,
			METHOD_RELEASE_BRIGHTNESS, NULL, NULL);
	return errno_to_device_error(ret);
}

int device_set_brightness_to_settings(int disp_idx, int new_value)
{
	return device_display_set_brightness(disp_idx, new_value);
}

int device_battery_is_full(bool* full)
{
	device_battery_level_e status;
	int ret;

	if (full == NULL)
		return DEVICE_ERROR_INVALID_PARAMETER;

	ret = device_battery_get_level_status(&status);
	if (ret < 0)
		return ret;

	if (status == DEVICE_BATTERY_LEVEL_FULL)
		*full = true;
	else
		*full = false;

	return DEVICE_ERROR_NONE;
}

static device_battery_cb changed_callback = NULL;
static void* changed_callback_user_data = NULL;

static void battery_changed_inside_cb(keynode_t* key, void* user_data)
{
	const char* keyname = vconf_keynode_get_name(key);

	if (keyname != NULL && changed_callback != NULL && strcmp(keyname, VCONFKEY_SYSMAN_BATTERY_CAPACITY) == 0) {
		int percent = 0;
		if (vconf_get_int(VCONFKEY_SYSMAN_BATTERY_CAPACITY, &percent) == 0) {
			changed_callback(percent, changed_callback_user_data);
		}
	}
}

int device_battery_set_cb(device_battery_cb callback, void* user_data)
{
	// VCONFKEY_SYSMAN_BATTERY_CAPACITY
	int err;
	if (callback == NULL)
		return DEVICE_ERROR_INVALID_PARAMETER;

	changed_callback = callback;
	changed_callback_user_data = user_data;

	err = vconf_notify_key_changed(VCONFKEY_SYSMAN_BATTERY_CAPACITY, battery_changed_inside_cb, NULL);
	if (err < 0)
		return DEVICE_ERROR_INVALID_PARAMETER;

	return DEVICE_ERROR_NONE;
}

int device_battery_unset_cb(void)
{
	int err = vconf_ignore_key_changed(VCONFKEY_SYSMAN_BATTERY_CAPACITY, battery_changed_inside_cb);
	if (err < 0)
		return DEVICE_ERROR_OPERATION_FAILED;

	changed_callback = NULL;
	changed_callback_user_data = NULL;

	return DEVICE_ERROR_NONE;
}

int device_battery_get_warning_status(device_battery_warn_e *status)
{
	if (status == NULL)
		return DEVICE_ERROR_INVALID_PARAMETER;

	int value, err;

	err = vconf_get_int(VCONFKEY_SYSMAN_BATTERY_STATUS_LOW, &value);
	if (err < 0)
		return DEVICE_ERROR_OPERATION_FAILED;

	if (value == VCONFKEY_SYSMAN_BAT_POWER_OFF) {
		*status = DEVICE_BATTERY_WARN_EMPTY;
	} else if (value == VCONFKEY_SYSMAN_BAT_CRITICAL_LOW) {
		*status = DEVICE_BATTERY_WARN_CRITICAL;
	} else if (value == VCONFKEY_SYSMAN_BAT_WARNING_LOW) {
		*status = DEVICE_BATTERY_WARN_LOW;
	} else if (value == VCONFKEY_SYSMAN_BAT_NORMAL) {
		*status = DEVICE_BATTERY_WARN_NORMAL;
	} else if (value == VCONFKEY_SYSMAN_BAT_FULL) {
		*status = DEVICE_BATTERY_WARN_FULL;
	} else {
		return DEVICE_ERROR_OPERATION_FAILED;
	}

	return DEVICE_ERROR_NONE;
}

static device_battery_warn_cb warn_changed_callback = NULL;
static void* warn_changed_callback_user_data = NULL;

static void battery_warn_changed_inside_cb(keynode_t* key, void* user_data)
{
	const char* keyname = vconf_keynode_get_name(key);

	if (keyname != NULL && warn_changed_callback != NULL && strcmp(keyname, VCONFKEY_SYSMAN_BATTERY_STATUS_LOW) == 0) {
		int bat_state = 0;
		if (vconf_get_int(VCONFKEY_SYSMAN_BATTERY_STATUS_LOW, &bat_state) == 0) {
			warn_changed_callback(bat_state-1, warn_changed_callback_user_data);
		}
	}
}

int device_battery_warning_set_cb(device_battery_warn_cb callback, void* user_data)
{
	// VCONFKEY_SYSMAN_BATTERY_STATUS_LOW
	int err;

	if (callback == NULL)
		return DEVICE_ERROR_INVALID_PARAMETER;

	warn_changed_callback = callback;
	warn_changed_callback_user_data = user_data;

	err = vconf_notify_key_changed(VCONFKEY_SYSMAN_BATTERY_STATUS_LOW, battery_warn_changed_inside_cb, NULL);
	if (err < 0)
		return DEVICE_ERROR_INVALID_PARAMETER;

	return DEVICE_ERROR_NONE;
}

int device_battery_warning_unset_cb(void)
{
	int err = vconf_ignore_key_changed(VCONFKEY_SYSMAN_BATTERY_STATUS_LOW, battery_warn_changed_inside_cb);
	if (err < 0)
		return DEVICE_ERROR_OPERATION_FAILED;

	warn_changed_callback = NULL;
	warn_changed_callback_user_data = NULL;

	return DEVICE_ERROR_NONE;
}
