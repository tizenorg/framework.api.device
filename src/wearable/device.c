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
#include <dd-display.h>
#include <dd-led.h>
#include "device.h"
#include "device_log.h"

#define CHECK_ERR(val)	\
	do {	\
		if (val < 0) {		\
			if (errno == ENODEV)	\
				return DEVICE_ERROR_NOT_SUPPORTED;	\
			return DEVICE_ERROR_OPERATION_FAILED;	\
		}	\
	} while(0)

int device_get_display_numbers(int* device_number)
{
	if(device_number == NULL)
		return DEVICE_ERROR_INVALID_PARAMETER;

	*device_number = display_get_count();
	CHECK_ERR(*device_number);

	return DEVICE_ERROR_NONE;
}

int device_get_brightness(int disp_idx, int* value)
{
	int val, max_id, ret;

	if(value == NULL)
		return DEVICE_ERROR_INVALID_PARAMETER;

	ret = device_get_display_numbers(&max_id);
	if (ret != DEVICE_ERROR_NONE)
		return ret;

	if(disp_idx < 0 || disp_idx >= max_id)
		return DEVICE_ERROR_INVALID_PARAMETER;

	val = display_get_brightness();
	CHECK_ERR(val);

	*value = val;
	return DEVICE_ERROR_NONE;
}

int device_set_brightness(int disp_idx, int new_value)
{
	int max_value, val, max_id, ret;

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

	val = display_set_brightness(new_value);
	CHECK_ERR(val);

	return DEVICE_ERROR_NONE;
}

int device_get_max_brightness(int disp_idx, int* max_value)
{
	int val, max_id, ret;

	if(max_value == NULL)
		return DEVICE_ERROR_INVALID_PARAMETER;

	ret = device_get_display_numbers(&max_id);
	if (ret != DEVICE_ERROR_NONE)
		return ret;

	if(disp_idx < 0 || disp_idx >= max_id)
		return DEVICE_ERROR_INVALID_PARAMETER;

	val = display_get_max_brightness();
	CHECK_ERR(val);

	*max_value = val;
	return DEVICE_ERROR_NONE;
}

int device_set_brightness_from_settings(int disp_idx)
{
	int max_id, val, ret;

	ret = device_get_display_numbers(&max_id);
	if (ret != DEVICE_ERROR_NONE)
		return ret;

	if(disp_idx < 0 || disp_idx >= max_id)
		return DEVICE_ERROR_INVALID_PARAMETER;

	val = display_release_brightness();
	CHECK_ERR(val);

	return DEVICE_ERROR_NONE;
}

int device_set_brightness_to_settings(int disp_idx, int new_value)
{
	int max_value, val, max_id, ret;

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

	val = display_set_brightness_with_setting(new_value);
	CHECK_ERR(val);

	return DEVICE_ERROR_NONE;
}

int device_flash_get_brightness(int *brightness)
{
	int value;

	if (brightness == NULL)
		return DEVICE_ERROR_INVALID_PARAMETER;

	value = led_get_brightness();
	CHECK_ERR(value);

	*brightness = value;
	return DEVICE_ERROR_NONE;
}

int device_flash_set_brightness(int brightness)
{
	int max_value, value, ret;

	ret = device_flash_get_max_brightness(&max_value);
	if (ret != DEVICE_ERROR_NONE)
		return ret;

	if (brightness < 0 || brightness > max_value)
		return DEVICE_ERROR_INVALID_PARAMETER;

	value = led_set_brightness(brightness);
	CHECK_ERR(value);

	return DEVICE_ERROR_NONE;
}

int device_flash_get_max_brightness(int *max_brightness)
{
	int value;

	if (max_brightness == NULL)
		return DEVICE_ERROR_INVALID_PARAMETER;

	value = led_get_max_brightness();
	CHECK_ERR(value);

	*max_brightness = value;
	return DEVICE_ERROR_NONE;
}
