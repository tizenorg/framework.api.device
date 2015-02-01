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
#include <stdbool.h>
#include <errno.h>

#include "power.h"
#include "display.h"
#include "common.h"
#include "dbus.h"

/**
 * Parameters for device_power_request_lock()
 */
#define STAY_CUR_STATE  0x1
#define GOTO_STATE_NOW  0x2
#define HOLD_KEY_BLOCK  0x4
#define STANDBY_MODE    0x8

/**
 * Parameters for device_power_request_unlock()
 */
#define PM_SLEEP_MARGIN 0x0 /**< keep guard time for unlock */
#define PM_RESET_TIMER  0x1 /**< reset timer for unlock */
#define PM_KEEP_TIMER   0x2 /**< keep timer for unlock */

#define METHOD_LOCK_STATE       "lockstate"
#define METHOD_UNLOCK_STATE     "unlockstate"
#define METHOD_CHANGE_STATE     "changestate"

#define STR_STAYCURSTATE "staycurstate"
#define STR_GOTOSTATENOW "gotostatenow"

#define STR_HOLDKEYBLOCK "holdkeyblock"
#define STR_STANDBYMODE  "standbymode"
#define STR_NULL         "NULL"

#define STR_SLEEP_MARGIN "sleepmargin"
#define STR_RESET_TIMER  "resettimer"
#define STR_KEEP_TIMER   "keeptimer"

#define STR_LCD_OFF   "lcdoff"
#define STR_LCD_DIM   "lcddim"
#define STR_LCD_ON    "lcdon"

static char *get_state_str(display_state_e state)
{
	switch (state) {
	case DISPLAY_STATE_NORMAL:
		return STR_LCD_ON;
	case DISPLAY_STATE_SCREEN_DIM:
		return STR_LCD_DIM;
	case DISPLAY_STATE_SCREEN_OFF:
		return STR_LCD_OFF;
	default:
		break;
	}
	return NULL;
}

static void lock_cb(void *data, DBusMessage *msg, DBusError *unused)
{
	DBusError err;
	int ret, val;

	if (!msg)
		return;

	dbus_error_init(&err);
	ret = dbus_message_get_args(msg, &err, DBUS_TYPE_INT32, &val, DBUS_TYPE_INVALID);
	if (!ret) {
		_E("no message [%s:%s]", err.name, err.message);
		dbus_error_free(&err);
		return;
	}

	_D("%s-%s : %d", DEVICED_INTERFACE_DISPLAY, METHOD_LOCK_STATE, val);
}

static int lock_state(display_state_e state, unsigned int flag, int timeout_ms)
{
	char *arr[4];
	char str_timeout[32];

	arr[0] = get_state_str(state);
	if (!arr[0])
		return -EINVAL;

	if (flag & GOTO_STATE_NOW)
		arr[1] = STR_GOTOSTATENOW;
	else
		arr[1] = STR_STAYCURSTATE;

	if (flag & HOLD_KEY_BLOCK)
		arr[2] = STR_HOLDKEYBLOCK;
	else if (flag & STANDBY_MODE)
		arr[2] = STR_STANDBYMODE;
	else
		arr[2] = STR_NULL;

	snprintf(str_timeout, sizeof(str_timeout), "%d", timeout_ms);
	arr[3] = str_timeout;

	return dbus_method_async_with_reply(DEVICED_BUS_NAME,
			DEVICED_PATH_DISPLAY, DEVICED_INTERFACE_DISPLAY,
			METHOD_LOCK_STATE, "sssi", arr, lock_cb, -1, NULL);
}

static void unlock_cb(void *data, DBusMessage *msg, DBusError *unused)
{
	DBusError err;
	int ret, val;

	if (!msg)
		return;

	dbus_error_init(&err);
	ret = dbus_message_get_args(msg, &err, DBUS_TYPE_INT32, &val, DBUS_TYPE_INVALID);
	if (!ret) {
		_E("no message [%s:%s]", err.name, err.message);
		dbus_error_free(&err);
		return;
	}

	_D("%s-%s : %d", DEVICED_INTERFACE_DISPLAY, METHOD_UNLOCK_STATE, val);
}

static int unlock_state(display_state_e state, unsigned int flag)
{
	char *arr[2];

	arr[0] = get_state_str(state);
	if (!arr[0])
		return -EINVAL;

	if (flag == PM_SLEEP_MARGIN)
		arr[1] = STR_SLEEP_MARGIN;
	else if (flag == PM_RESET_TIMER)
		arr[1] = STR_RESET_TIMER;
	else if (flag == PM_KEEP_TIMER)
		arr[1] = STR_KEEP_TIMER;
	else
		return -EINVAL;

	return dbus_method_async_with_reply(DEVICED_BUS_NAME,
			DEVICED_PATH_DISPLAY, DEVICED_INTERFACE_DISPLAY,
			METHOD_UNLOCK_STATE, "ss", arr, unlock_cb, -1, NULL);
}

int device_power_request_lock(power_lock_e type, int timeout_ms)
{
	int ret;

	if (timeout_ms < 0)
		return DEVICE_ERROR_INVALID_PARAMETER;

	if (type == POWER_LOCK_CPU)
		ret = lock_state(DISPLAY_STATE_SCREEN_OFF, STAY_CUR_STATE, timeout_ms);
	else if (type == POWER_LOCK_DISPLAY)
		ret = lock_state(DISPLAY_STATE_NORMAL, STAY_CUR_STATE, timeout_ms);
	else if (type == POWER_LOCK_DISPLAY_DIM)
		ret = lock_state(DISPLAY_STATE_SCREEN_DIM, STAY_CUR_STATE, timeout_ms);
	else
		return DEVICE_ERROR_INVALID_PARAMETER;

	if (ret == -ECOMM)
		return DEVICE_ERROR_PERMISSION_DENIED;
	else if (ret < 0)
		return DEVICE_ERROR_OPERATION_FAILED;

	return DEVICE_ERROR_NONE;
}

int device_power_release_lock(power_lock_e type)
{
	int ret;

	if (type == POWER_LOCK_CPU)
		ret = unlock_state(DISPLAY_STATE_SCREEN_OFF, PM_SLEEP_MARGIN);
	else if (type == POWER_LOCK_DISPLAY)
		ret = unlock_state(DISPLAY_STATE_NORMAL, PM_KEEP_TIMER);
	else if (type == POWER_LOCK_DISPLAY_DIM)
		ret = unlock_state(DISPLAY_STATE_SCREEN_DIM, PM_KEEP_TIMER);
	else
		return DEVICE_ERROR_INVALID_PARAMETER;

	if (ret == -ECOMM)
		return DEVICE_ERROR_PERMISSION_DENIED;
	else if (ret < 0)
		return DEVICE_ERROR_OPERATION_FAILED;

	return DEVICE_ERROR_NONE;
}

int device_power_wakeup(bool dim)
{
	if (dim)
		return device_display_change_state(DISPLAY_STATE_SCREEN_DIM);

	return device_display_change_state(DISPLAY_STATE_NORMAL);
}
