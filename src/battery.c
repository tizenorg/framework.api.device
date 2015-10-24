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

#include "battery.h"
#include "common.h"
#include "dbus.h"

#define METHOD_GET_PERCENT		"GetPercent"

int device_battery_get_percent(int *percent)
{
	int ret;

	if (!percent)
		return DEVICE_ERROR_INVALID_PARAMETER;

	ret = dbus_method_sync(DEVICED_BUS_NAME,
			DEVICED_PATH_BATTERY, DEVICED_INTERFACE_BATTERY,
			METHOD_GET_PERCENT, NULL, NULL);
	/* regard not suppoted as disconnected */
	if (ret == -ENOTSUP)
		ret = 0;
	else if (ret < 0)
		return errno_to_device_error(ret);

	*percent = ret;
	return DEVICE_ERROR_NONE;
}

int device_battery_is_charging(bool *charging)
{
	int ret, val;

	if (!charging)
		return DEVICE_ERROR_INVALID_PARAMETER;

	ret = vconf_get_int(VCONFKEY_SYSMAN_BATTERY_CHARGE_NOW, &val);
	/* regard not supported as disconnected */
	if (val == -ENOTSUP)
		val = 0;
	else if (ret < 0 || val < 0 || val > 1)
		return DEVICE_ERROR_OPERATION_FAILED;

	*charging = val;
	return DEVICE_ERROR_NONE;
}

int device_battery_get_level_status(device_battery_level_e *status)
{
	int val, ret;

	if (!status)
		return DEVICE_ERROR_INVALID_PARAMETER;

	ret = vconf_get_int(VCONFKEY_SYSMAN_BATTERY_LEVEL_STATUS, &val);
	if (ret < 0)
		return DEVICE_ERROR_OPERATION_FAILED;

	if (val == VCONFKEY_SYSMAN_BAT_LEVEL_EMPTY)
		*status = DEVICE_BATTERY_LEVEL_EMPTY;
	else if (val == VCONFKEY_SYSMAN_BAT_LEVEL_CRITICAL)
		*status = DEVICE_BATTERY_LEVEL_CRITICAL;
	else if (val == VCONFKEY_SYSMAN_BAT_LEVEL_LOW)
		*status = DEVICE_BATTERY_LEVEL_LOW;
	else if (val == VCONFKEY_SYSMAN_BAT_LEVEL_HIGH)
		*status = DEVICE_BATTERY_LEVEL_HIGH;
	else if (val == VCONFKEY_SYSMAN_BAT_LEVEL_FULL)
		*status = DEVICE_BATTERY_LEVEL_FULL;
	/* regard not supported as disconnected */
	else if (val == -ENOTSUP)
		*status = DEVICE_BATTERY_LEVEL_EMPTY;
	else
		return DEVICE_ERROR_OPERATION_FAILED;

	return DEVICE_ERROR_NONE;
}
