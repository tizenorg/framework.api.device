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
#include <stdbool.h>
#include <errno.h>
#include <system_info.h>

#include "led.h"
#include "common.h"
#include "dbus.h"

#define METHOD_GET_MAX_BRIGHTNESS	"GetMaxBrightness"
#define METHOD_GET_BRIGHTNESS		"GetBrightness"
#define METHOD_SET_BRIGHTNESS		"SetBrightness"
#define METHOD_PLAY_CUSTOM	"playcustom"
#define METHOD_STOP_CUSTOM	"stopcustom"

#define FRONT_LED_FEATURE		"tizen.org/feature/led"
#define CAMERA_LED_FEATURE		"tizen.org/feature/camera.back.flash"

static bool support_front_led;
static bool support_camera_led;

static void __CONSTRUCTOR__ init(void)
{
	int ret;
	bool val;

	ret = system_info_get_platform_bool(FRONT_LED_FEATURE, &val);
	if (ret == SYSTEM_INFO_ERROR_NONE && val)
		support_front_led = true;

	ret = system_info_get_platform_bool(CAMERA_LED_FEATURE, &val);
	if (ret == SYSTEM_INFO_ERROR_NONE && val)
		support_camera_led = true;
}

int device_flash_get_max_brightness(int *max_brightness)
{
	int ret;

	if (!support_camera_led)
		return DEVICE_ERROR_NOT_SUPPORTED;

	if (!max_brightness)
		return DEVICE_ERROR_INVALID_PARAMETER;

	ret = dbus_method_sync(DEVICED_BUS_NAME,
			DEVICED_PATH_LED, DEVICED_INTERFACE_LED,
			METHOD_GET_MAX_BRIGHTNESS, NULL, NULL);
	if (ret < 0)
		return errno_to_device_error(ret);

	*max_brightness = ret;
	return DEVICE_ERROR_NONE;
}

int device_flash_get_brightness(int *brightness)
{
	int ret;

	if (!support_camera_led)
		return DEVICE_ERROR_NOT_SUPPORTED;

	if (!brightness)
		return DEVICE_ERROR_INVALID_PARAMETER;

	ret = dbus_method_sync(DEVICED_BUS_NAME,
			DEVICED_PATH_LED, DEVICED_INTERFACE_LED,
			METHOD_GET_BRIGHTNESS, NULL, NULL);
	if (ret < 0)
		return errno_to_device_error(ret);

	*brightness = ret;
	return DEVICE_ERROR_NONE;
}

int device_flash_set_brightness(int brightness)
{
	char *arr[2];
	char buf_val[32];
	char buf_noti[32];
	int max, ret;

	if (!support_camera_led)
		return DEVICE_ERROR_NOT_SUPPORTED;

	ret = device_flash_get_max_brightness(&max);
	if (ret < 0)
		return ret;

	if (brightness < 0 || brightness > max)
		return DEVICE_ERROR_INVALID_PARAMETER;

	snprintf(buf_val, sizeof(buf_val), "%d", brightness);
	arr[0] = buf_val;
	snprintf(buf_noti, sizeof(buf_noti), "%d", 0);
	arr[1] = buf_noti;

	/* if camera API preempt a flash device, it will return -EBUSY error. */
	ret = dbus_method_sync(DEVICED_BUS_NAME,
			DEVICED_PATH_LED, DEVICED_INTERFACE_LED,
			METHOD_SET_BRIGHTNESS, "ii", arr);
	if (ret < 0)
		return errno_to_device_error(ret);

	return DEVICE_ERROR_NONE;
}

int device_led_play_custom(int on, int off, unsigned int color, unsigned int flags)
{
	char *arr[4];
	char str_on[32], str_off[32];
	char str_color[32], str_flags[32];
	int ret;

	if (!support_front_led)
		return DEVICE_ERROR_NOT_SUPPORTED;

	if (on < 0 || off < 0)
		return DEVICE_ERROR_INVALID_PARAMETER;

	snprintf(str_on, sizeof(str_on), "%d", on);
	arr[0] = str_on;
	snprintf(str_off, sizeof(str_off), "%d", off);
	arr[1] = str_off;
	snprintf(str_color, sizeof(str_color), "%lu", (long unsigned int)color);
	arr[2] = str_color;
	snprintf(str_flags, sizeof(str_flags), "%lu", (long unsigned int)flags);
	arr[3] = str_flags;

	ret = dbus_method_sync(DEVICED_BUS_NAME,
			DEVICED_PATH_LED, DEVICED_INTERFACE_LED,
			METHOD_PLAY_CUSTOM, "iiuu", arr);
	if (ret < 0)
		return errno_to_device_error(ret);

	return DEVICE_ERROR_NONE;
}

int device_led_stop_custom(void)
{
	int ret;

	if (!support_front_led)
		return DEVICE_ERROR_NOT_SUPPORTED;

	ret = dbus_method_sync(DEVICED_BUS_NAME,
			DEVICED_PATH_LED, DEVICED_INTERFACE_LED,
			METHOD_STOP_CUSTOM, NULL, NULL);
	if (ret < 0)
		return errno_to_device_error(ret);

	return DEVICE_ERROR_NONE;
}
