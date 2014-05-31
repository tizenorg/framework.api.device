/*
 * capi-system-device
 * Copyright (c) 2012 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <stdio.h>
#include <string.h>
#include <devman.h>
#include <vconf.h>
#include "device.h"
#include "device_log.h"

#define CHECK_ERR(val)	\
	do {	\
		if (val == DEVMAN_ERROR_NOT_SUPPORTED)	\
			return DEVICE_ERROR_NOT_SUPPORTED;	\
		else if (val == DEVMAN_ERROR_OPERATION_FAILED)	\
			return DEVICE_ERROR_OPERATION_FAILED;	\
	} while(0)

int device_battery_get_percent(int* percent)
{
	if (percent == NULL)
		return DEVICE_ERROR_INVALID_PARAMETER;

	int pct = device_get_battery_pct();
	CHECK_ERR(pct);

	*percent = pct;
	return DEVICE_ERROR_NONE;
}

int device_battery_get_detail(int* percent)
{
	if (percent == NULL)
		return DEVICE_ERROR_INVALID_PARAMETER;

	int pct = device_get_battery_pct_raw();
	CHECK_ERR(pct);

	*percent = pct;
	return DEVICE_ERROR_NONE;
}

int device_battery_is_full(bool* full)
{
	if (full == NULL)
		return DEVICE_ERROR_INVALID_PARAMETER;

	int f = device_is_battery_full();
	CHECK_ERR(f);

	*full = (f == 1) ? true : false;
	return DEVICE_ERROR_NONE;
}

int device_battery_is_charging(bool *charging)
{
	// VCONFKEY_SYSMAN_BATTERY_CHARGE_NOW
	int value, err;

	if (charging == NULL)
		return DEVICE_ERROR_INVALID_PARAMETER;

	err = vconf_get_int(VCONFKEY_SYSMAN_BATTERY_CHARGE_NOW, &value);
	if (err < 0)
		return DEVICE_ERROR_OPERATION_FAILED;

	if (value == 1) {
		*charging = true;
	} else if (value == 0) {
		*charging = false;
	} else {
		return DEVICE_ERROR_OPERATION_FAILED;
	}

	return DEVICE_ERROR_NONE;
}

static device_battery_cb changed_callback = NULL;
static void* changed_callback_user_data = NULL;

static void battery_changed_inside_cb(keynode_t* key, void* user_data)
{
	char* keyname = vconf_keynode_get_name(key);

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
static device_battery_level_cb level_changed_callback = NULL;
static void* level_changed_callback_user_data = NULL;

static void battery_warn_changed_inside_cb(keynode_t* key, void* user_data)
{
	char* keyname = vconf_keynode_get_name(key);

	if (keyname != NULL && warn_changed_callback != NULL && strcmp(keyname, VCONFKEY_SYSMAN_BATTERY_STATUS_LOW) == 0) {
		int bat_state = 0;
		if (vconf_get_int(VCONFKEY_SYSMAN_BATTERY_STATUS_LOW, &bat_state) == 0) {
			warn_changed_callback(bat_state-1, warn_changed_callback_user_data);
		}
	}
}

static void battery_level_changed_inside_cb(keynode_t* key, void* user_data)
{
	char* keyname;
	keyname = vconf_keynode_get_name(key);

	if (keyname != NULL && level_changed_callback != NULL &&
	    strcmp(keyname, VCONFKEY_SYSMAN_BATTERY_LEVEL_STATUS) == 0) {
		int bat_state = 0;
		if (vconf_get_int(VCONFKEY_SYSMAN_BATTERY_LEVEL_STATUS, &bat_state) == 0) {
			level_changed_callback(bat_state, level_changed_callback_user_data);
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

static device_battery_remaining_time_changed_cb timetofull_changed_cb = NULL;
static device_battery_remaining_time_changed_cb timetoempty_changed_cb = NULL;
static void* timetofull_cb_user_data = NULL;
static void* timetoempty_cb_user_data = NULL;

static void device_battery_time_timetofull_inside_cb(keynode_t* key, void* user_data)
{
	char* keyname = vconf_keynode_get_name(key);
	int time = -1, err= -1;

	if (keyname == NULL)
		return;

	if (strcmp(keyname, VCONFKEY_PM_BATTERY_TIMETOFULL) == 0) {
		if (timetofull_changed_cb != NULL) {
			err = device_battery_get_remaining_time(
				DEVICE_BATTERY_REMAINING_TIME_TO_FULLY_CHARGED, &time);
			if ((err == DEVICE_ERROR_NONE) && (time >= 0))
				timetofull_changed_cb(time, timetofull_cb_user_data);
		}
	}
}

static void device_battery_time_timetoempty_inside_cb(keynode_t* key, void* user_data)
{
	char* keyname = vconf_keynode_get_name(key);
	int time = -1, err= -1;

	if (keyname == NULL)
		return;

	if (strcmp(keyname, VCONFKEY_PM_BATTERY_TIMETOEMPTY) == 0) {
		if (timetoempty_changed_cb != NULL) {
			err = device_battery_get_remaining_time(
				DEVICE_BATTERY_REMAINING_TIME_TO_DISCHARGED, &time);
			if ((err == DEVICE_ERROR_NONE) && (time >= 0))
				timetoempty_changed_cb(time, timetoempty_cb_user_data);
		}
	}
}

int device_battery_get_remaining_time(device_battery_remaining_time_type_e type, int* time)
{
	bool charging = false;
	int err = -1;

	err = device_battery_is_charging(&charging);
	if (err != DEVICE_ERROR_NONE)
		return DEVICE_ERROR_OPERATION_FAILED;

	switch(type) {
		case DEVICE_BATTERY_REMAINING_TIME_TO_FULLY_CHARGED:
			if (charging == false) {
				return DEVICE_ERROR_OPERATION_FAILED;
			} else {
				err = vconf_get_int(VCONFKEY_PM_BATTERY_TIMETOFULL, time);
				if (err < 0 || time < 0) {
					return DEVICE_ERROR_OPERATION_FAILED;
				}
			}
			break;
		case DEVICE_BATTERY_REMAINING_TIME_TO_DISCHARGED:
			if (charging == true) {
				return DEVICE_ERROR_OPERATION_FAILED;
			} else {
				err = vconf_get_int(VCONFKEY_PM_BATTERY_TIMETOEMPTY, time);
				if (err < 0 || time < 0) {
					return DEVICE_ERROR_OPERATION_FAILED;
				}
			}
			break;
		default:
			return DEVICE_ERROR_INVALID_PARAMETER;
	}
	return DEVICE_ERROR_NONE;
}

int device_battery_set_remaining_time_changed_cb(device_battery_remaining_time_type_e type,
	device_battery_remaining_time_changed_cb callback, void* user_data)
{
	int err;

	if (callback == NULL)
		return DEVICE_ERROR_INVALID_PARAMETER;

	switch(type) {
		case DEVICE_BATTERY_REMAINING_TIME_TO_FULLY_CHARGED:
			timetofull_changed_cb = callback;
			timetofull_cb_user_data = user_data;
			err = vconf_notify_key_changed(VCONFKEY_PM_BATTERY_TIMETOFULL,
				device_battery_time_timetofull_inside_cb, NULL);
			if (err < 0)
				return DEVICE_ERROR_OPERATION_FAILED;
			break;
		case DEVICE_BATTERY_REMAINING_TIME_TO_DISCHARGED:
			timetoempty_changed_cb = callback;
			timetoempty_cb_user_data = user_data;
			err = vconf_notify_key_changed(VCONFKEY_PM_BATTERY_TIMETOEMPTY,
				device_battery_time_timetoempty_inside_cb, NULL);
			if (err < 0)
				return DEVICE_ERROR_OPERATION_FAILED;
			break;
		default:
			return DEVICE_ERROR_INVALID_PARAMETER;
	}
	return DEVICE_ERROR_NONE;
}

int device_battery_unset_remaining_time_changed_cb(device_battery_remaining_time_type_e type)
{
	int err;

	switch(type) {
		case DEVICE_BATTERY_REMAINING_TIME_TO_FULLY_CHARGED:
			err = vconf_ignore_key_changed(VCONFKEY_PM_BATTERY_TIMETOFULL,
				device_battery_time_timetofull_inside_cb);
			if (err < 0)
				return DEVICE_ERROR_OPERATION_FAILED;
			timetofull_changed_cb = NULL;
			timetofull_cb_user_data = NULL;
			break;
		case DEVICE_BATTERY_REMAINING_TIME_TO_DISCHARGED:
			err = vconf_ignore_key_changed(VCONFKEY_PM_BATTERY_TIMETOEMPTY,
				device_battery_time_timetoempty_inside_cb);
			if (err < 0)
				return DEVICE_ERROR_OPERATION_FAILED;
			timetoempty_changed_cb = NULL;
			timetoempty_cb_user_data = NULL;
			break;
		default:
			return DEVICE_ERROR_INVALID_PARAMETER;
	}
	return DEVICE_ERROR_NONE;
}

int device_battery_get_level_status(device_battery_level_e *status)
{
	int value, err;

	if (status == NULL)
		return DEVICE_ERROR_INVALID_PARAMETER;


	err = vconf_get_int(VCONFKEY_SYSMAN_BATTERY_LEVEL_STATUS, &value);
	if (err < 0)
		return DEVICE_ERROR_OPERATION_FAILED;

	if (value == VCONFKEY_SYSMAN_BAT_LEVEL_EMPTY) {
		*status = DEVICE_BATTERY_LEVEL_EMPTY;
	} else if (value == VCONFKEY_SYSMAN_BAT_LEVEL_CRITICAL) {
		*status = DEVICE_BATTERY_LEVEL_CRITICAL;
	} else if (value == VCONFKEY_SYSMAN_BAT_LEVEL_LOW) {
		*status = DEVICE_BATTERY_LEVEL_LOW;
	} else if (value == VCONFKEY_SYSMAN_BAT_LEVEL_HIGH) {
		*status = DEVICE_BATTERY_LEVEL_HIGH;
	} else if (value == VCONFKEY_SYSMAN_BAT_LEVEL_FULL) {
		*status = DEVICE_BATTERY_LEVEL_FULL;
	} else {
		return DEVICE_ERROR_OPERATION_FAILED;
	}

	return DEVICE_ERROR_NONE;
}

int device_battery_level_set_cb(device_battery_level_cb callback, void* user_data)
{
	int err;


	if (callback == NULL) {
		err = vconf_ignore_key_changed(VCONFKEY_SYSMAN_BATTERY_LEVEL_STATUS,
			battery_level_changed_inside_cb);
	} else {

		err = vconf_notify_key_changed(VCONFKEY_SYSMAN_BATTERY_LEVEL_STATUS,
			battery_level_changed_inside_cb, NULL);
	}
	if (err < 0)
		return DEVICE_ERROR_OPERATION_FAILED;

	level_changed_callback = callback;
	level_changed_callback_user_data = user_data;

	return DEVICE_ERROR_NONE;
}
