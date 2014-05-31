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
#define API_NAME_DEVICE_GET_MAX_BRIGHTNESS "device_get_max_brightness"
#define API_NAME_DEVICE_SET_BRIGHTNESS "device_set_brightness"
#define API_NAME_DEVICE_SET_BRIGHTNESS_FROM_SETTINGS "device_set_brightness_from_settings"

static void startup(void);
static void cleanup(void);

void (*tet_startup)(void) = startup;
void (*tet_cleanup)(void) = cleanup;


static void utc_system_device_get_display_numbers_p(void);
static void utc_system_device_get_brightness_p(void);
static void utc_system_device_get_brightness_n_1(void);
static void utc_system_device_get_brightness_n_2(void);
static void utc_system_device_get_max_brightness_p(void);
static void utc_system_device_get_max_brightness_n_1(void);
static void utc_system_device_get_max_brightness_n_2(void);
static void utc_system_device_set_brightness_p_1(void);
static void utc_system_device_set_brightness_p_2(void);
static void utc_system_device_set_brightness_n_1(void);
static void utc_system_device_set_brightness_n_2(void);
static void utc_system_device_set_brightness_from_settings_p(void);
static void utc_system_device_set_brightness_from_settings_n(void);


enum {
	POSITIVE_TC_IDX = 0x01,
	NEGATIVE_TC_IDX,
};

struct tet_testlist tet_testlist[] = {
    { utc_system_device_get_display_numbers_p, POSITIVE_TC_IDX},
	{ utc_system_device_get_brightness_p, POSITIVE_TC_IDX },
	{ utc_system_device_get_brightness_n_1, NEGATIVE_TC_IDX },
	{ utc_system_device_get_brightness_n_2, NEGATIVE_TC_IDX },
	{ utc_system_device_get_max_brightness_p, POSITIVE_TC_IDX },
	{ utc_system_device_get_max_brightness_n_1, NEGATIVE_TC_IDX },
	{ utc_system_device_get_max_brightness_n_2, NEGATIVE_TC_IDX },
	{ utc_system_device_set_brightness_p_1, POSITIVE_TC_IDX },
	{ utc_system_device_set_brightness_p_2, POSITIVE_TC_IDX },
	{ utc_system_device_set_brightness_n_1, NEGATIVE_TC_IDX },
	{ utc_system_device_set_brightness_n_2, NEGATIVE_TC_IDX },
	{ utc_system_device_set_brightness_from_settings_p, POSITIVE_TC_IDX },
	{ utc_system_device_set_brightness_from_settings_n, NEGATIVE_TC_IDX },
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

static void utc_system_device_get_display_numbers_p(void)
{
    int _cnt, err;
    err = device_get_display_numbers(&_cnt);

    if(err < 0){
        dts_fail(API_NAME_DEVICE_GET_DISPLAY_COUNT);
    }
    else{
        dts_pass(API_NAME_DEVICE_GET_DISPLAY_COUNT);
    }
}

/**
 * @brief Positive test case of device_get_brightness()
 */
static void utc_system_device_get_brightness_p(void)
{
    int i;
    int value = 0;
    int error = DEVICE_ERROR_NONE;

    for(i=0; i<cnt; i++){
        error = device_get_brightness(i, &value);

        if (error != DEVICE_ERROR_NONE){
            dts_fail(API_NAME_DEVICE_GET_BRIGHTNESS);
        }

        if (value < 0){
            dts_fail(API_NAME_DEVICE_GET_BRIGHTNESS);
        }
    }

    dts_pass(API_NAME_DEVICE_GET_BRIGHTNESS);
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
    int value = 0, i=0;
    int error = DEVICE_ERROR_NONE;

    for(i=0; i<cnt; i++){
        error = device_get_brightness(i, NULL);

        if(error == DEVICE_ERROR_NONE){
            dts_fail(API_NAME_DEVICE_GET_BRIGHTNESS);
        }
    }
    dts_pass(API_NAME_DEVICE_GET_BRIGHTNESS);
}

/**
 * @brief Positive test case of device_get_max_brightness()
 */
static void utc_system_device_get_max_brightness_p(void)
{
    int value = 0, i=0;
    int error = DEVICE_ERROR_NONE;

    for(i=0; i<cnt; i++){
        error = device_get_max_brightness(i, &value);

        if(error != DEVICE_ERROR_NONE){
            dts_fail(API_NAME_DEVICE_GET_MAX_BRIGHTNESS);
        }
    }
    dts_pass(API_NAME_DEVICE_GET_MAX_BRIGHTNESS);
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
    int error = DEVICE_ERROR_NONE, i=0;

    for(i=0; i<cnt; i++){
        error = device_get_max_brightness(i, NULL);

        if(error == DEVICE_ERROR_NONE){
            dts_fail(API_NAME_DEVICE_GET_MAX_BRIGHTNESS);
        }
    }
    dts_pass(API_NAME_DEVICE_GET_MAX_BRIGHTNESS);
}

/**
 * @brief Positive test case of device_set_brightness()
 */
static void utc_system_device_set_brightness_p_1(void)
{
    int i=0;
    int err;

    dts_message(API_NAME_DEVICE_SET_BRIGHTNESS, "!@#$ cnt=%d", cnt);

    for(i=0; i<cnt; i++){
        dts_message(API_NAME_DEVICE_SET_BRIGHTNESS, "index=%d", i);
        err = device_set_brightness(i, 0);
        dts_message(API_NAME_DEVICE_SET_BRIGHTNESS, "error=%d", err);
        if(err != DEVICE_ERROR_NONE) {
            dts_fail(API_NAME_DEVICE_SET_BRIGHTNESS);
        }
    }
    dts_pass(API_NAME_DEVICE_SET_BRIGHTNESS);
}

/**
 * @brief Positive test case of device_set_brightness()
 */
static void utc_system_device_set_brightness_p_2(void)
{
    int setting = 0;
    int max_value = 0;
    int value = 0, i=0;

    for(i=0; i<cnt; i++){
        if( device_get_max_brightness(i, &max_value) != DEVICE_ERROR_NONE ) {
            dts_fail(API_NAME_DEVICE_SET_BRIGHTNESS);
        }

        if( max_value < 0 ) {
            dts_fail(API_NAME_DEVICE_SET_BRIGHTNESS);
        }

        setting = max_value > 1 ? max_value / 2 : 1;

        if( device_set_brightness(i, setting) != DEVICE_ERROR_NONE ) {
            dts_fail(API_NAME_DEVICE_SET_BRIGHTNESS);
        }
        dts_message(API_NAME_DEVICE_SET_BRIGHTNESS, "brightness=%d", setting);


        if ( device_get_brightness(i, &value) != DEVICE_ERROR_NONE ) {
            dts_fail(API_NAME_DEVICE_SET_BRIGHTNESS);
        }
        dts_message(API_NAME_DEVICE_SET_BRIGHTNESS, "brightness=%d", value);

        if ( value != setting ){
            dts_fail(API_NAME_DEVICE_SET_BRIGHTNESS);
        }
    }

    dts_pass(API_NAME_DEVICE_SET_BRIGHTNESS);
}

/**
 * @brief Negative test case of device_set_brightness()
 */
static void utc_system_device_set_brightness_n_1(void)
{
    int i=0;
    int max_value = 0;

    for(i=0; i<cnt; i++){
        if( device_get_max_brightness(i, &max_value) != DEVICE_ERROR_NONE ) {
            dts_fail(API_NAME_DEVICE_SET_BRIGHTNESS, "fail get_max_brightness");
        }

        if( device_set_brightness(i, max_value+1) == DEVICE_ERROR_NONE ) {
            dts_fail(API_NAME_DEVICE_SET_BRIGHTNESS, "fail set_brightness");
        }
    }

    dts_pass(API_NAME_DEVICE_SET_BRIGHTNESS);
}

/**
 * @brief Negative test case of device_set_brightness()
 */
static void utc_system_device_set_brightness_n_2(void)
{
    int i=0;

    for(i=0; i<cnt; i++){
        if( device_set_brightness(i, -1) == DEVICE_ERROR_NONE ) {
            dts_fail(API_NAME_DEVICE_SET_BRIGHTNESS);
        }
    }

    dts_pass(API_NAME_DEVICE_SET_BRIGHTNESS);
}

/**
 * @brief Positive test case of device_set_brightness_from_settings()
 */
static void utc_system_device_set_brightness_from_settings_p(void)
{
    int i, error;

    dts_message(API_NAME_DEVICE_SET_BRIGHTNESS_FROM_SETTINGS, "display cnt=%d", cnt);

    for(i=0; i<cnt; i++){
        dts_message(API_NAME_DEVICE_SET_BRIGHTNESS_FROM_SETTINGS, "index=%d", i);
        error = device_set_brightness_from_settings(i);
        dts_message(API_NAME_DEVICE_SET_BRIGHTNESS_FROM_SETTINGS, "error=%d", error);
        if(error != DEVICE_ERROR_NONE) {
            dts_fail(API_NAME_DEVICE_SET_BRIGHTNESS_FROM_SETTINGS);
        }
    }
    dts_pass(API_NAME_DEVICE_SET_BRIGHTNESS_FROM_SETTINGS);
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
