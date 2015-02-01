/*
 * 
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 * PROPRIETARY/CONFIDENTIAL
 * 
 * This software is the confidential and proprietary information of SAMSUNG 
 * ELECTRONICS ("Confidential Information"). You agree and acknowledge that 
 * this software is owned by Samsung and you shall not disclose such 
 * Confidential Information and shall use it only in accordance with the terms 
 * of the license agreement you entered into with SAMSUNG ELECTRONICS. SAMSUNG 
 * make no representations or warranties about the suitability of the software, 
 * either express or implied, including but not limited to the implied 
 * warranties of merchantability, fitness for a particular purpose, or 
 * non-infringement. SAMSUNG shall not be liable for any damages suffered by 
 * licensee arising out of or related to this software.
 * 
 */
#include <tet_api.h>
#include <device.h>

#define API_NAME_DEVICE_FLASH_GET_BRIGHTNESS "device_flash_get_brightness"
#define API_NAME_DEVICE_FLASH_SET_BRIGHTNESS "device_flash_set_brightness"
#define API_NAME_DEVICE_FLASH_GET_MAX_BRIGHTNESS "device_flash_get_max_brightness"

static void startup(void);
static void cleanup(void);

void (*tet_startup)(void) = startup;
void (*tet_cleanup)(void) = cleanup;


static void utc_system_device_flash_get_brightness_p(void);
static void utc_system_device_flash_get_brightness_n(void);
static void utc_system_device_flash_set_brightness_p(void);
static void utc_system_device_flash_set_brightness_n(void);
static void utc_system_device_flash_get_max_brightness_p(void);
static void utc_system_device_flash_get_max_brightness_n(void);


enum {
	POSITIVE_TC_IDX = 0x01,
	NEGATIVE_TC_IDX,
};

struct tet_testlist tet_testlist[] = {
	{ utc_system_device_flash_get_brightness_p, POSITIVE_TC_IDX},
	{ utc_system_device_flash_get_brightness_n, NEGATIVE_TC_IDX },
	{ utc_system_device_flash_set_brightness_p, POSITIVE_TC_IDX},
	{ utc_system_device_flash_set_brightness_n, NEGATIVE_TC_IDX },
	{ utc_system_device_flash_get_max_brightness_p, POSITIVE_TC_IDX},
	{ utc_system_device_flash_get_max_brightness_n, NEGATIVE_TC_IDX },
	{ NULL, 0},
};

static void startup(void)
{
	/* start of TC */
}

static void cleanup(void)
{
	/* end of TC */
}

/**
 * @brief Positive test case of device_flash_get_brightness()
 */
static void utc_system_device_flash_get_brightness_p(void)
{
	int brightness;
	int error = DEVICE_ERROR_NONE;
	error = device_flash_get_brightness(&brightness);

	dts_check_eq(API_NAME_DEVICE_FLASH_GET_BRIGHTNESS, error, DEVICE_ERROR_NONE);
}

/**
 * @brief Negative test case of device_flash_get_brightness()
 */
static void utc_system_device_flash_get_brightness_n(void)
{
	int error = DEVICE_ERROR_NONE;
	error = device_flash_get_brightness(NULL);

	dts_check_ne(API_NAME_DEVICE_FLASH_GET_BRIGHTNESS, error, DEVICE_ERROR_NONE);
}

/**
 * @brief Positive test case of device_flash_set_brightness()
 */
static void utc_system_device_flash_set_brightness_p(void)
{
	int max;
	int error = DEVICE_ERROR_NONE;

	if (device_flash_get_max_brightness(&max) != DEVICE_ERROR_NONE)
		dts_fail(API_NAME_DEVICE_FLASH_SET_BRIGHTNESS);

	error = device_flash_set_brightness(max);

	dts_check_eq(API_NAME_DEVICE_FLASH_SET_BRIGHTNESS, error, DEVICE_ERROR_NONE);
}

/**
 * @brief Negative test case of device_flash_set_brightness()
 */
static void utc_system_device_flash_set_brightness_n(void)
{
	int error = DEVICE_ERROR_NONE;
	error = device_flash_set_brightness(-1);

	dts_check_ne(API_NAME_DEVICE_FLASH_SET_BRIGHTNESS, error, DEVICE_ERROR_NONE);
}

/**
 * @brief Positive test case of device_flash_get_max_brightness()
 */
static void utc_system_device_flash_get_max_brightness_p(void)
{
	int max;
	int error = DEVICE_ERROR_NONE;
	error = device_flash_get_max_brightness(&max);

	dts_check_eq(API_NAME_DEVICE_FLASH_GET_MAX_BRIGHTNESS, error, DEVICE_ERROR_NONE);
}

/**
 * @brief Negative test case of device_flash_get_max_brightness()
 */
static void utc_system_device_flash_get_max_brightness_n(void)
{
	int error = DEVICE_ERROR_NONE;
	error = device_flash_get_max_brightness(NULL);

	dts_check_ne(API_NAME_DEVICE_FLASH_GET_MAX_BRIGHTNESS, error, DEVICE_ERROR_NONE);
}
