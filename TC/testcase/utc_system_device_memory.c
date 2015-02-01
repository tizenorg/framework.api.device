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

#define API_NAME_DEVICE_MEMORY_GET_TOTAL "device_memory_get_total"
#define API_NAME_DEVICE_MEMORY_GET_AVAILABLE "device_memory_get_available"

static void startup(void);
static void cleanup(void);

void (*tet_startup)(void) = startup;
void (*tet_cleanup)(void) = cleanup;


static void utc_system_device_memory_get_total_p(void);
static void utc_system_device_memory_get_total_n(void);
static void utc_system_device_memory_get_available_p(void);
static void utc_system_device_memory_get_available_n(void);


enum {
	POSITIVE_TC_IDX = 0x01,
	NEGATIVE_TC_IDX,
};

struct tet_testlist tet_testlist[] = {
	{ utc_system_device_memory_get_total_p, POSITIVE_TC_IDX },
	{ utc_system_device_memory_get_total_n, NEGATIVE_TC_IDX },
	{ utc_system_device_memory_get_available_p, POSITIVE_TC_IDX },
	{ utc_system_device_memory_get_available_n, NEGATIVE_TC_IDX },
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
 * @brief Positive test case of device_memory_get_total()
 */
static void utc_system_device_memory_get_total_p(void)
{
	unsigned int total;
	int error = DEVICE_ERROR_NONE;
	error = device_memory_get_total(&total);

	dts_check_eq(API_NAME_DEVICE_MEMORY_GET_TOTAL, error, DEVICE_ERROR_NONE);
}

/**
 * @brief Negative test case of device_memory_get_total()
 */
static void utc_system_device_memory_get_total_n(void)
{
	int error = DEVICE_ERROR_NONE;
	error = device_memory_get_total(NULL);

	dts_check_ne(API_NAME_DEVICE_MEMORY_GET_TOTAL, error, DEVICE_ERROR_NONE);
}

/**
 * @brief Positive test case of device_memory_get_available()
 */
static void utc_system_device_memory_get_available_p(void)
{
	unsigned int avail;
	int error = DEVICE_ERROR_NONE;
	error = device_memory_get_available(&avail);

	dts_check_eq(API_NAME_DEVICE_MEMORY_GET_AVAILABLE, error, DEVICE_ERROR_NONE);
}

/**
 * @brief Negative test case of device_memory_get_available()
 */
static void utc_system_device_memory_get_available_n(void)
{
	int error = DEVICE_ERROR_NONE;
	error = device_memory_get_available(NULL);

	dts_check_ne(API_NAME_DEVICE_MEMORY_GET_AVAILABLE, error, DEVICE_ERROR_NONE);
}
