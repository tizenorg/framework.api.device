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
#include <power.h>

#define API_NAME_DEVICE_GET_DISPLAY_COUNT "device_get_display_numbers"
#define API_NAME_DEVICE_GET_BRIGHTNESS "device_get_brightness"
#define API_NAME_DEVICE_SET_BRIGHTNESS "device_set_brightness"
#define API_NAME_DEVICE_GET_MAX_BRIGHTNESS "device_get_max_brightness"
#define API_NAME_DEVICE_SET_BRIGHTNESS_FROM_SETTINGS "device_set_brightness_from_settings"
#define API_NAME_DEVICE_SET_BRIGHTNESS_TO_SETTINGS "device_set_brightness_to_settings"

static void startup(void);
static void cleanup(void);

void (*tet_startup)(void) = startup;
void (*tet_cleanup)(void) = cleanup;


static void utc_system_device_get_display_numbers_p(void);
static void utc_system_device_get_display_numbers_n(void);
static void utc_system_device_get_brightness_p(void);
static void utc_system_device_get_brightness_n_1(void);
static void utc_system_device_get_brightness_n_2(void);
static void utc_system_device_set_brightness_p(void);
static void utc_system_device_set_brightness_n_1(void);
static void utc_system_device_set_brightness_n_2(void);
static void utc_system_device_get_max_brightness_p(void);
static void utc_system_device_get_max_brightness_n_1(void);
static void utc_system_device_get_max_brightness_n_2(void);
static void utc_system_device_set_brightness_from_settings_p(void);
static void utc_system_device_set_brightness_from_settings_n(void);
static void utc_system_device_set_brightness_to_settings_p(void);
static void utc_system_device_set_brightness_to_settings_n(void);

enum {
	POSITIVE_TC_IDX = 0x01,
	NEGATIVE_TC_IDX,
};

struct tet_testlist tet_testlist[] = {
	{ utc_system_device_get_display_numbers_p, POSITIVE_TC_IDX},
	{ utc_system_device_get_display_numbers_n, NEGATIVE_TC_IDX },
	{ utc_system_device_get_brightness_p, POSITIVE_TC_IDX },
	{ utc_system_device_get_brightness_n_1, NEGATIVE_TC_IDX },
	{ utc_system_device_get_brightness_n_2, NEGATIVE_TC_IDX },
	{ utc_system_device_set_brightness_p, POSITIVE_TC_IDX },
	{ utc_system_device_set_brightness_n_1, NEGATIVE_TC_IDX },
	{ utc_system_device_set_brightness_n_2, NEGATIVE_TC_IDX },
	{ utc_system_device_get_max_brightness_p, POSITIVE_TC_IDX },
	{ utc_system_device_get_max_brightness_n_1, NEGATIVE_TC_IDX },
	{ utc_system_device_get_max_brightness_n_2, NEGATIVE_TC_IDX },
	{ utc_system_device_set_brightness_from_settings_p, POSITIVE_TC_IDX },
	{ utc_system_device_set_brightness_from_settings_n, NEGATIVE_TC_IDX },
	{ utc_system_device_set_brightness_to_settings_p, POSITIVE_TC_IDX },
	{ utc_system_device_set_brightness_to_settings_n, NEGATIVE_TC_IDX },
	{ NULL, 0},
};

static int cnt;

static void startup(void)
{
	/* start of TC */
	power_wakeup(false);
	power_lock_state(POWER_STATE_NORMAL, 0);

	device_get_display_numbers(&cnt);
}

static void cleanup(void)
{
	/* end of TC */
	power_unlock_state(POWER_STATE_NORMAL);
}

/**
 * @brief Positive test case of device_get_display_numbers()
 */
static void utc_system_device_get_display_numbers_p(void)
{
	int _cnt, error = DEVICE_ERROR_NONE;
	error = device_get_display_numbers(&_cnt);

	dts_check_eq(API_NAME_DEVICE_GET_DISPLAY_COUNT, error, DEVICE_ERROR_NONE);
}

/**
 * @brief Negative test case of device_get_display_numbers()
 */
static void utc_system_device_get_display_numbers_n(void)
{
	int error = DEVICE_ERROR_NONE;
	error = device_get_display_numbers(NULL);

	dts_check_ne(API_NAME_DEVICE_GET_DISPLAY_COUNT, error, DEVICE_ERROR_NONE);
}

/**
 * @brief Positive test case of device_get_brightness()
 */
static void utc_system_device_get_brightness_p(void)
{
	int i;
	int value = 0;
	int error = DEVICE_ERROR_NONE;

	for (i = 0; i < cnt; i++) {
		error = device_get_brightness(i, &value);
		dts_check_eq(API_NAME_DEVICE_GET_BRIGHTNESS, error, DEVICE_ERROR_NONE);
	}
}

/**
 * @brief Negative test case of device_get_brightness() with bad display parameter
 */
static void utc_system_device_get_brightness_n_1(void)
{
	int value = 0;
	int error = DEVICE_ERROR_NONE;

	error = device_get_brightness(cnt+1, &value);
	dts_check_ne(API_NAME_DEVICE_GET_BRIGHTNESS, error, DEVICE_ERROR_NONE);
}

/**
 * @brief Negative test case of device_get_brightness() with null pointer
 */
static void utc_system_device_get_brightness_n_2(void)
{
	int i;
	int error = DEVICE_ERROR_NONE;

	for (i = 0; i < cnt; i++) {
		error = device_get_brightness(i, NULL);
		dts_check_ne(API_NAME_DEVICE_GET_BRIGHTNESS, error, DEVICE_ERROR_NONE);
	}
}

/**
 * @brief Positive test case of device_set_brightness()
 */
static void utc_system_device_set_brightness_p(void)
{
	int i, max;
	int error = DEVICE_ERROR_NONE;

	dts_message(API_NAME_DEVICE_SET_BRIGHTNESS, "!@#$ cnt=%d", cnt);

	for (i = 0; i < cnt; i++) {
		dts_message(API_NAME_DEVICE_SET_BRIGHTNESS, "index=%d", i);

		if (device_get_max_brightness(i, &max) != DEVICE_ERROR_NONE)
			dts_fail(API_NAME_DEVICE_SET_BRIGHTNESS);

		error = device_set_brightness(i, max);
		dts_check_eq(API_NAME_DEVICE_SET_BRIGHTNESS, error, DEVICE_ERROR_NONE);
	}
}

/**
 * @brief Negative test case of device_set_brightness()
 */
static void utc_system_device_set_brightness_n_1(void)
{
	int error = DEVICE_ERROR_NONE;

	error = device_set_brightness(-1, 0);
	dts_check_ne(API_NAME_DEVICE_SET_BRIGHTNESS, error, DEVICE_ERROR_NONE);
}

/**
 * @brief Negative test case of device_set_brightness()
 */
static void utc_system_device_set_brightness_n_2(void)
{
	int i;
	int error = DEVICE_ERROR_NONE;

	for (i = 0; i < cnt; i++) {
		dts_message(API_NAME_DEVICE_SET_BRIGHTNESS, "index=%d", i);

		error = device_set_brightness(i, -1);
		dts_check_ne(API_NAME_DEVICE_SET_BRIGHTNESS, error, DEVICE_ERROR_NONE);
	}
}

/**
 * @brief Positive test case of device_get_max_brightness()
 */
static void utc_system_device_get_max_brightness_p(void)
{
	int value = 0, i;
	int error = DEVICE_ERROR_NONE;

	for (i = 0; i < cnt; i++) {
		dts_message(API_NAME_DEVICE_GET_MAX_BRIGHTNESS, "index=%d", i);

		error = device_get_max_brightness(i, &value);
		dts_check_eq(API_NAME_DEVICE_GET_MAX_BRIGHTNESS, error, DEVICE_ERROR_NONE);
	}
}

/**
 * @brief Negative test case of device_get_max_brightness() with bad display parameter
 */
static void utc_system_device_get_max_brightness_n_1(void)
{
	int value = 0;
	int error = DEVICE_ERROR_NONE;
	error = device_get_max_brightness(cnt+1, &value);

	dts_check_ne(API_NAME_DEVICE_GET_MAX_BRIGHTNESS, error, DEVICE_ERROR_NONE);
}

/**
 * @brief Negative test case of device_get_max_brightness() with null pointer
 */
static void utc_system_device_get_max_brightness_n_2(void)
{
	int i;
	int error = DEVICE_ERROR_NONE;

	for (i = 0; i < cnt; i++) {
		dts_message(API_NAME_DEVICE_GET_MAX_BRIGHTNESS, "index=%d", i);

		error = device_get_max_brightness(i, NULL);
		dts_check_ne(API_NAME_DEVICE_GET_MAX_BRIGHTNESS, error, DEVICE_ERROR_NONE);
	}
}

/**
 * @brief Positive test case of device_set_brightness_from_settings()
 */
static void utc_system_device_set_brightness_from_settings_p(void)
{
	int i;
	int error = DEVICE_ERROR_NONE;

	for (i = 0; i < cnt; i++) {
		dts_message(API_NAME_DEVICE_SET_BRIGHTNESS_FROM_SETTINGS, "index=%d", i);

		error = device_set_brightness_from_settings(i);
		dts_check_eq(API_NAME_DEVICE_SET_BRIGHTNESS_FROM_SETTINGS, error, DEVICE_ERROR_NONE);
	}
}

/**
 * @brief Negative test case of device_set_brightness_from_settings() with bad display parameter
 */
static void utc_system_device_set_brightness_from_settings_n(void)
{
	int error = DEVICE_ERROR_NONE;

	error = device_set_brightness_from_settings(cnt+1);

	dts_check_ne(API_NAME_DEVICE_SET_BRIGHTNESS_FROM_SETTINGS, error, DEVICE_ERROR_NONE);
}

/**
 * @brief Positive test case of device_set_brightness_to_settings()
 */
static void utc_system_device_set_brightness_to_settings_p(void)
{
	int i, max;
	int error = DEVICE_ERROR_NONE;

	for (i = 0; i < cnt; i++) {
		dts_message(API_NAME_DEVICE_SET_BRIGHTNESS_TO_SETTINGS, "index=%d", i);

		if (device_get_max_brightness(i, &max) != DEVICE_ERROR_NONE)
			dts_fail(API_NAME_DEVICE_SET_BRIGHTNESS_TO_SETTINGS);

		error = device_set_brightness_to_settings(i, max);
		dts_check_eq(API_NAME_DEVICE_SET_BRIGHTNESS_TO_SETTINGS, error, DEVICE_ERROR_NONE);
	}
}

/**
 * @brief Negative test case of device_set_brightness_to_settings()
 */
static void utc_system_device_set_brightness_to_settings_n(void)
{
	int error = DEVICE_ERROR_NONE;

	error = device_set_brightness_to_settings(cnt+1, 0);

	dts_check_ne(API_NAME_DEVICE_SET_BRIGHTNESS_TO_SETTINGS, error, DEVICE_ERROR_NONE);
}
