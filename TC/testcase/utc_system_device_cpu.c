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

#define API_NAME_DEVICE_CPU_GET_SYSTEM_TIME "device_cpu_get_system_time"
#define API_NAME_DEVICE_CPU_GET_COUNT "device_cpu_get_count"
#define API_NAME_DEVICE_CPU_GET_CURRENT_FREQ "device_cpu_get_current_freq"
#define API_NAME_DEVICE_CPU_GET_MAX_FREQ "device_cpu_get_max_freq"

static void startup(void);
static void cleanup(void);

void (*tet_startup)(void) = startup;
void (*tet_cleanup)(void) = cleanup;


static void utc_system_device_cpu_get_system_time_p(void);
static void utc_system_device_cpu_get_system_time_n(void);
static void utc_system_device_cpu_get_count_p(void);
static void utc_system_device_cpu_get_count_n(void);
static void utc_system_device_cpu_get_current_freq_p(void);
static void utc_system_device_cpu_get_current_freq_n_1(void);
static void utc_system_device_cpu_get_current_freq_n_2(void);
static void utc_system_device_cpu_get_max_freq_p(void);
static void utc_system_device_cpu_get_max_freq_n_1(void);
static void utc_system_device_cpu_get_max_freq_n_2(void);


enum {
	POSITIVE_TC_IDX = 0x01,
	NEGATIVE_TC_IDX,
};

struct tet_testlist tet_testlist[] = {
	{ utc_system_device_cpu_get_system_time_p, POSITIVE_TC_IDX },
	{ utc_system_device_cpu_get_system_time_n, NEGATIVE_TC_IDX },
	{ utc_system_device_cpu_get_count_p, POSITIVE_TC_IDX },
	{ utc_system_device_cpu_get_count_n, NEGATIVE_TC_IDX },
	{ utc_system_device_cpu_get_current_freq_p, POSITIVE_TC_IDX },
	{ utc_system_device_cpu_get_current_freq_n_1, NEGATIVE_TC_IDX },
	{ utc_system_device_cpu_get_current_freq_n_2, NEGATIVE_TC_IDX },
	{ utc_system_device_cpu_get_max_freq_p, POSITIVE_TC_IDX },
	{ utc_system_device_cpu_get_max_freq_n_1, NEGATIVE_TC_IDX },
	{ utc_system_device_cpu_get_max_freq_n_2, NEGATIVE_TC_IDX },
	{ NULL, 0},
};

static int cnt;

static void startup(void)
{
	/* start of TC */

	device_cpu_get_count(&cnt);
}

static void cleanup(void)
{
	/* end of TC */
}

/**
 * @brief Positive test case of device_cpu_get_system_time()
 */
static void utc_system_device_cpu_get_system_time_p(void)
{
	device_system_time_s stime;
	int error = DEVICE_ERROR_NONE;
	error = device_cpu_get_system_time(&stime);

	dts_check_eq(API_NAME_DEVICE_CPU_GET_SYSTEM_TIME, error, DEVICE_ERROR_NONE);
}

/**
 * @brief Negative test case of device_cpu_get_system_time()
 */
static void utc_system_device_cpu_get_system_time_n(void)
{
	int error = DEVICE_ERROR_NONE;
	error = device_cpu_get_system_time(NULL);

	dts_check_ne(API_NAME_DEVICE_CPU_GET_SYSTEM_TIME, error, DEVICE_ERROR_NONE);
}

/**
 * @brief Positive test case of device_cpu_get_count()
 */
static void utc_system_device_cpu_get_count_p(void)
{
	int _cnt;
	int error = DEVICE_ERROR_NONE;
	error = device_cpu_get_count(&_cnt);

	dts_check_eq(API_NAME_DEVICE_CPU_GET_COUNT, error, DEVICE_ERROR_NONE);
}

/**
 * @brief Negative test case of device_cpu_get_count()
 */
static void utc_system_device_cpu_get_count_n(void)
{
	int error = DEVICE_ERROR_NONE;
	error = device_cpu_get_count(NULL);

	dts_check_ne(API_NAME_DEVICE_CPU_GET_COUNT, error, DEVICE_ERROR_NONE);
}

/**
 * @brief Positive test case of device_cpu_get_current_freq()
 */
static void utc_system_device_cpu_get_current_freq_p(void)
{
	unsigned int freq;
	int i;
	int error = DEVICE_ERROR_NONE;

	for (i = 0; i < cnt; ++i) {
		dts_message(API_NAME_DEVICE_CPU_GET_CURRENT_FREQ, "index=%d", i);

		error = device_cpu_get_current_freq(i, &freq);
		dts_check_eq(API_NAME_DEVICE_CPU_GET_CURRENT_FREQ, error, DEVICE_ERROR_NONE);
	}
}

/**
 * @brief Negative test case of device_cpu_get_current_freq()
 */
static void utc_system_device_cpu_get_current_freq_n_1(void)
{
	unsigned int freq;
	int error = DEVICE_ERROR_NONE;
	error = device_cpu_get_current_freq(-1, &freq);

	dts_check_ne(API_NAME_DEVICE_CPU_GET_CURRENT_FREQ, error, DEVICE_ERROR_NONE);
}

/**
 * @brief Negative test case of device_cpu_get_current_freq()
 */
static void utc_system_device_cpu_get_current_freq_n_2(void)
{
	int i;
	int error = DEVICE_ERROR_NONE;

	for (i = 0; i < cnt; ++i) {
		dts_message(API_NAME_DEVICE_CPU_GET_CURRENT_FREQ, "index=%d", i);

		error = device_cpu_get_current_freq(i, NULL);
		dts_check_ne(API_NAME_DEVICE_CPU_GET_CURRENT_FREQ, error, DEVICE_ERROR_NONE);
	}
}

/**
 * @brief Positive test case of device_cpu_get_max_freq()
 */
static void utc_system_device_cpu_get_max_freq_p(void)
{
	unsigned int freq;
	int i;
	int error = DEVICE_ERROR_NONE;

	for (i = 0; i < cnt; ++i) {
		dts_message(API_NAME_DEVICE_CPU_GET_MAX_FREQ, "index=%d", i);

		error = device_cpu_get_max_freq(i, &freq);
		dts_check_eq(API_NAME_DEVICE_CPU_GET_MAX_FREQ, error, DEVICE_ERROR_NONE);
	}
}

/**
 * @brief Negative test case of device_cpu_get_max_freq()
 */
static void utc_system_device_cpu_get_max_freq_n_1(void)
{
	unsigned int freq;
	int error = DEVICE_ERROR_NONE;
	error = device_cpu_get_max_freq(-1, &freq);

	dts_check_ne(API_NAME_DEVICE_CPU_GET_MAX_FREQ, error, DEVICE_ERROR_NONE);
}

/**
 * @brief Negative test case of device_cpu_get_max_freq()
 */
static void utc_system_device_cpu_get_max_freq_n_2(void)
{
	int i;
	int error = DEVICE_ERROR_NONE;

	for (i = 0; i < cnt; ++i) {
		dts_message(API_NAME_DEVICE_CPU_GET_MAX_FREQ, "index=%d", i);

		error = device_cpu_get_max_freq(i, NULL);
		dts_check_ne(API_NAME_DEVICE_CPU_GET_MAX_FREQ, error, DEVICE_ERROR_NONE);
	}
}
