/*
 * deviced
 *
 * Copyright (c) 2012 - 2013 Samsung Electronics Co., Ltd.
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
#include <errno.h>

#include "haptic.h"
#include "common.h"
#include "dbus.h"

#define METHOD_GET_COUNT			"GetCount"
#define METHOD_OPEN_DEVICE			"OpenDevice"
#define METHOD_CLOSE_DEVICE			"CloseDevice"
#define METHOD_STOP_DEVICE			"StopDevice"
#define METHOD_VIBRATE_MONOTONE		"VibrateMonotone"

enum feedback_e
{
	HAPTIC_FEEDBACK_MIN = 0,
	HAPTIC_FEEDBACK_MAX = 100,
};

enum priority_e
{
	HAPTIC_PRIORITY_MIN = 0,
	HAPTIC_PRIORITY_MIDDLE,
	HAPTIC_PRIORITY_HIGH,
};

int device_haptic_get_count(int *device_number)
{
	int ret;

	if (!device_number)
		return DEVICE_ERROR_INVALID_PARAMETER;

	/* request to deviced to get haptic count */
	ret = dbus_method_sync(DEVICED_BUS_NAME,
			DEVICED_PATH_HAPTIC, DEVICED_INTERFACE_HAPTIC,
			METHOD_GET_COUNT, NULL, NULL);
	if (ret < 0)
		return errno_to_device_error(ret);

	*device_number = ret;
	return DEVICE_ERROR_NONE;
}

int device_haptic_open(int device_index, haptic_device_h *device_handle)
{
	char str_index[32];
	char *arr[1];
	int ret, max;

	ret = device_haptic_get_count(&max);
	if (ret < 0)
		return ret;

	if (device_index < 0 || device_index >= max)
		return DEVICE_ERROR_INVALID_PARAMETER;

	if (!device_handle)
		return DEVICE_ERROR_INVALID_PARAMETER;

	snprintf(str_index, sizeof(str_index), "%d", device_index);
	arr[0] = str_index;

	/* request to deviced to open haptic device */
	ret = dbus_method_sync(DEVICED_BUS_NAME,
			DEVICED_PATH_HAPTIC, DEVICED_INTERFACE_HAPTIC,
			METHOD_OPEN_DEVICE, "i", arr);
	if (ret < 0)
		return errno_to_device_error(ret);

	*device_handle = (haptic_device_h)ret;
	return DEVICE_ERROR_NONE;
}

int device_haptic_close(haptic_device_h device_handle)
{
	char str_handle[32];
	char *arr[1];
	int ret;

	if (!device_handle)
		return DEVICE_ERROR_INVALID_PARAMETER;

	snprintf(str_handle, sizeof(str_handle), "%u", (unsigned int)device_handle);
	arr[0] = str_handle;

	/* request to deviced to open haptic device */
	ret = dbus_method_sync(DEVICED_BUS_NAME,
			DEVICED_PATH_HAPTIC, DEVICED_INTERFACE_HAPTIC,
			METHOD_CLOSE_DEVICE, "u", arr);
	if (ret < 0)
		return errno_to_device_error(ret);

	return DEVICE_ERROR_NONE;
}

int device_haptic_vibrate(haptic_device_h device_handle, int duration, int feedback, haptic_effect_h *effect_handle)
{
	char str_handle[32];
	char str_duration[32];
	char str_feedback[32];
	char str_priority[32];
	char *arr[4];
	int ret, priority;

	if (!device_handle)
		return DEVICE_ERROR_INVALID_PARAMETER;

	if (duration < 0)
		return DEVICE_ERROR_INVALID_PARAMETER;

	if (feedback < HAPTIC_FEEDBACK_MIN || feedback > HAPTIC_FEEDBACK_MAX)
		return DEVICE_ERROR_INVALID_PARAMETER;

	priority = HAPTIC_PRIORITY_MIN;

	snprintf(str_handle, sizeof(str_handle), "%u", (unsigned int)device_handle);
	arr[0] = str_handle;
	snprintf(str_duration, sizeof(str_duration), "%d", duration);
	arr[1] = str_duration;
	snprintf(str_feedback, sizeof(str_feedback), "%d", feedback);
	arr[2] = str_feedback;
	snprintf(str_priority, sizeof(str_priority), "%d", priority);
	arr[3] = str_priority;

	/* request to deviced to vibrate haptic device */
	ret = dbus_method_sync(DEVICED_BUS_NAME,
			DEVICED_PATH_HAPTIC, DEVICED_INTERFACE_HAPTIC,
			METHOD_VIBRATE_MONOTONE, "uiii", arr);
	if (ret < 0)
		return errno_to_device_error(ret);

	if (effect_handle)
		*effect_handle = (haptic_effect_h)ret;

	return DEVICE_ERROR_NONE;
}

int device_haptic_stop(haptic_device_h device_handle, haptic_effect_h effect_handle)
{
	char str_handle[32];
	char *arr[1];
	int ret;

	if (!device_handle)
		return DEVICE_ERROR_INVALID_PARAMETER;

	/* TODO : support to stop haptic effect */
	snprintf(str_handle, sizeof(str_handle), "%u", (unsigned int)device_handle);
	arr[0] = str_handle;

	/* request to deviced to open haptic device */
	ret = dbus_method_sync(DEVICED_BUS_NAME,
			DEVICED_PATH_HAPTIC, DEVICED_INTERFACE_HAPTIC,
			METHOD_STOP_DEVICE, "u", arr);
	if (ret < 0)
		return errno_to_device_error(ret);

	return DEVICE_ERROR_NONE;
}
