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
#include <errno.h>
#include <vconf.h>

#include "display.h"
#include "common.h"
#include "dbus.h"

#define METHOD_GET_DISPLAY_COUNT    "GetDisplayCount"
#define METHOD_GET_MAX_BRIGHTNESS   "GetMaxBrightness"
#define METHOD_GET_BRIGHTNESS   "GetBrightness"
#define METHOD_SET_BRIGHTNESS   "SetBrightness"
#define METHOD_CHANGE_STATE		"changestate"

#define STR_LCD_OFF   "lcdoff"
#define STR_LCD_DIM   "lcddim"
#define STR_LCD_ON    "lcdon"

static int display_cnt = -1;
struct display {
	int max;
} *display_arr;

static int alloc_display(void)
{
	int i;

	if (display_cnt < 0)
		return -ENODEV;

	display_arr = malloc(sizeof(struct display) * display_cnt);
	if (!display_arr)
		return -ENOMEM;

	for (i = 0; i < display_cnt; ++i)
		display_arr[i].max = -1;

	return 0;
}

int device_display_get_numbers(int *device_number)
{
	int ret;

	if (!device_number)
		return DEVICE_ERROR_INVALID_PARAMETER;

	/* if it is a first request */
	if (display_cnt < 0) {
		ret = dbus_method_sync(DEVICED_BUS_NAME,
				DEVICED_PATH_DISPLAY, DEVICED_INTERFACE_DISPLAY,
				METHOD_GET_DISPLAY_COUNT, NULL, NULL);
		if (ret < 0)
			return errno_to_device_error(ret);
		display_cnt = ret;
		alloc_display();
	}

	*device_number = display_cnt;
	_I("device_number : %d", *device_number);
	return DEVICE_ERROR_NONE;
}

int device_display_get_max_brightness(int display_index, int *max_brightness)
{
	int ret;

	if (!max_brightness)
		return DEVICE_ERROR_INVALID_PARAMETER;

	if (display_cnt < 0)
		device_display_get_numbers(&display_cnt);

	if (display_index < 0 || display_index >= display_cnt)
		return DEVICE_ERROR_INVALID_PARAMETER;

	if (!display_arr && alloc_display() < 0)
		return DEVICE_ERROR_OPERATION_FAILED;

	if (display_arr[display_index].max < 0) {
		ret = dbus_method_sync(DEVICED_BUS_NAME,
				DEVICED_PATH_DISPLAY, DEVICED_INTERFACE_DISPLAY,
				METHOD_GET_MAX_BRIGHTNESS, NULL, NULL);
		if (ret < 0)
			return errno_to_device_error(ret);
		display_arr[display_index].max = ret;
	}

	*max_brightness = display_arr[display_index].max;
	return DEVICE_ERROR_NONE;
}

int device_display_get_brightness(int display_index, int *brightness)
{
	int ret;

	if (!brightness)
		return DEVICE_ERROR_INVALID_PARAMETER;

	if (display_cnt < 0)
		device_display_get_numbers(&display_cnt);

	if (display_index < 0 || display_index >= display_cnt)
		return DEVICE_ERROR_INVALID_PARAMETER;

	ret = dbus_method_sync(DEVICED_BUS_NAME,
			DEVICED_PATH_DISPLAY, DEVICED_INTERFACE_DISPLAY,
			METHOD_GET_BRIGHTNESS, NULL, NULL);
	if (ret < 0)
		return errno_to_device_error(ret);

	*brightness = ret;
	return DEVICE_ERROR_NONE;
}

int device_display_set_brightness(int display_index, int brightness)
{
	char *arr[1];
	char str_val[32];
	int ret, max;

	if (display_cnt < 0)
		device_display_get_numbers(&display_cnt);

	if (display_index < 0 || display_index >= display_cnt)
		return DEVICE_ERROR_INVALID_PARAMETER;

	if (display_arr[display_index].max < 0)
		device_display_get_max_brightness(display_index, &max);

	if (brightness < 0 || brightness > display_arr[display_index].max)
		return DEVICE_ERROR_INVALID_PARAMETER;

	snprintf(str_val, sizeof(str_val), "%d", brightness);
	arr[0] = str_val;

	ret = dbus_method_sync(DEVICED_BUS_NAME,
			DEVICED_PATH_DISPLAY, DEVICED_INTERFACE_DISPLAY,
			METHOD_SET_BRIGHTNESS, "i", arr);
	if (ret < 0)
		return errno_to_device_error(ret);

	return DEVICE_ERROR_NONE;
}

int device_display_get_state(display_state_e *state)
{
	int ret, val;

	if (!state)
		return DEVICE_ERROR_INVALID_PARAMETER;

	ret = vconf_get_int(VCONFKEY_PM_STATE, &val);
	if (ret < 0)
		return DEVICE_ERROR_OPERATION_FAILED;

	if (val == VCONFKEY_PM_STATE_NORMAL)
		*state = DISPLAY_STATE_NORMAL;
	else if (val == VCONFKEY_PM_STATE_LCDDIM)
		*state = DISPLAY_STATE_SCREEN_DIM;
	else if (val == VCONFKEY_PM_STATE_LCDOFF)
		*state = DISPLAY_STATE_SCREEN_OFF;
	else
		return DEVICE_ERROR_OPERATION_FAILED;

	return DEVICE_ERROR_NONE;
}

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

int device_display_change_state(display_state_e state)
{
	char *str, *arr[1];
	int ret;

	if (state < DISPLAY_STATE_NORMAL || state > DISPLAY_STATE_SCREEN_OFF)
		return DEVICE_ERROR_INVALID_PARAMETER;

	str = get_state_str(state);
	if (!str)
		return DEVICE_ERROR_INVALID_PARAMETER;

	arr[0] = str;

	ret = dbus_method_sync(DEVICED_BUS_NAME,
			DEVICED_PATH_DISPLAY, DEVICED_INTERFACE_DISPLAY,
			METHOD_CHANGE_STATE, "s", arr);
	if (ret < 0)
		return errno_to_device_error(ret);

	return DEVICE_ERROR_NONE;
}
