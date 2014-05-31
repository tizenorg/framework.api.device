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




#ifndef __TIZEN_SYSTEM_DEVICE_H__
#define __TIZEN_SYSTEM_DEVICE_H__

#include <stdbool.h>
#include <tizen_error.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup CAPI_SYSTEM_DEVICE_MODULE
 * @{
 */

/**
 * @brief Enumerations of error code for Device.
 */
typedef enum
{
    DEVICE_ERROR_NONE              = TIZEN_ERROR_NONE,                  /**< Successful */
    DEVICE_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER,   /**< Invalid parameter */
    DEVICE_ERROR_OPERATION_FAILED  = TIZEN_ERROR_SYSTEM_CLASS | 0x12, /**< Operation failed */
    DEVICE_ERROR_NOT_SUPPORTED     = TIZEN_ERROR_SYSTEM_CLASS | 0x13, /**< Not supported in this device */
} device_error_e;

/**
 * @brief Enumerations of the battery warning status
 */
typedef enum
{
    DEVICE_BATTERY_WARN_EMPTY,      /**< The battery goes empty. Prepare for the safe termination of the application, because the device starts a shutdown process soon after entering this level. */
    DEVICE_BATTERY_WARN_CRITICAL,  /**< The battery charge is at a critical state. You may have to stop using multimedia features, because they are not guaranteed to work correctly at this battery status. */
    DEVICE_BATTERY_WARN_LOW,       /**< The battery has little charge left. */
    DEVICE_BATTERY_WARN_NORMAL,    /**< The battery status is not to be careful. */
    DEVICE_BATTERY_WARN_FULL,      /**< The battery status is full. */
} device_battery_warn_e;

/**
 * @brief Enumerations of the battery remaining time type
 */
typedef enum
{
    DEVICE_BATTERY_REMAINING_TIME_TO_FULLY_CHARGED,
    DEVICE_BATTERY_REMAINING_TIME_TO_DISCHARGED
} device_battery_remaining_time_type_e;

/**
 * @brief Enumerations of the battery level status
 */
typedef enum
{
    DEVICE_BATTERY_LEVEL_EMPTY = 0,      /**< The battery goes empty. Prepare for the safe termination of the application, because the device starts a shutdown process soon after entering this level. */
    DEVICE_BATTERY_LEVEL_CRITICAL,  /**< The battery charge is at a critical state. You may have to stop using multimedia features, because they are not guaranteed to work correctly at this battery status. */
    DEVICE_BATTERY_LEVEL_LOW,       /**< The battery has little charge left. */
    DEVICE_BATTERY_LEVEL_HIGH,    /**< The battery status is not to be careful. */
    DEVICE_BATTERY_LEVEL_FULL,      /**< The battery status is full. */
} device_battery_level_e;


/**
 * @brief Structure of the time information system spent, measured in units of USER_HZ
 */
typedef struct {
	unsigned long long total;
	unsigned long long user;
	unsigned long long nice;
	unsigned long long system;
	unsigned long long idle;
	unsigned long long iowait;
	unsigned long long irq;
	unsigned long long softirq;
} device_system_time_s;

/**
 * @}
*/

 /**
 * @addtogroup CAPI_SYSTEM_DEVICE_MODULE
 * @{
 */

/**
 * @brief Called when an battery charge percentage changed
 *
 * @param[in] percent       The remaining battery charge percentage (0 ~ 100)
 * @param[in] user_data     The user data passed from the callback registration function
 *
 */
typedef void (*device_battery_cb)(int percent, void *user_data); 

/**
 * @brief Called when the device warn about the battery status.
 *
 * @param[in] status       The battery warning status
 * @param[in] user_data    The user data passed from the callback registration function
 *
 */
typedef void (*device_battery_warn_cb)(device_battery_warn_e status, void *user_data);

/**
 * @brief This callback take remained time for fully charged or discharged.
 *
 * @param[in] time         The battery remainig seconds to fully chagred or discharged
 * @param[in] user_data    The user data passed from the callback registration function
 *
 */
typedef void (*device_battery_remaining_time_changed_cb)(int time, void* user_data);

/**
 * @brief Called when an battery level changed
 *
 * @param[in] status       The remaining battery level (empty[0~1] critical[2~5] low[6~15] high[16~94] full[95~100])
 * @param[in] user_data     The user data passed from the callback registration function
 *
 */
typedef void (*device_battery_level_cb)(device_battery_level_e status, void *user_data);

/**
 * @brief Gets the battery warning status.
 *
 * @param[out] status The battery warning status.
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 *
 * @see device_battery_status_e
 * @see device_battery_status_set_cb()
 */
int device_battery_get_warning_status(device_battery_warn_e *status);

/**
 * @brief Set callback to be observing battery warning.
 *
 * @param[in] callback      The callback function to set
 * @param[in] user_data     The user data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 *
 * @see device_battery_status_e
 * @see device_battery_get_status()
 */
int device_battery_warning_set_cb(device_battery_warn_cb callback, void* user_data);

/**
 * @brief Unset battery warning callback function.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE               Successful
 * @retval #DEVICE_ERROR_OPERATION_FAILED   Operation failed
 */
int device_battery_warning_unset_cb(void);

/**
 * @brief Gets the battery charge percentage.
 * @details It returns integer value from 0 to 100 that indicates remaining battery charge
 * as a percentage of the maximum level.
 * @remarks In order to be notified when the battery state changes, use system_info_set_changed_cb().
 *
 * @param[out] percent The remaining battery charge percentage (0 ~ 100) 
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 *
 * @see device_battery_is_full()
 * @see device_battery_get_detail()
 * @see device_battery_set_cb()
 */
int device_battery_get_percent(int *percent);

/**
 * @brief Gets the battery detail charge as a per ten thousand.
 * @details It return integer value from 0 to 10000 that indicates remaining battery charge as a per ten thousand of the maximum level.
 * @remarks this function return #DEVICE_ERROR_NOT_SUPPORTED when device can not be supported detail battery information.
 *
 * @param[out] detail   The remaining battery charge as a per ten thousand. (0 ~ 10000)
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED      Not supported device
 *
 * @see device_battery_is_full()
 * @see device_battery_get_percent()
 * @see device_battery_set_cb()
 */
int device_battery_get_detail(int *detail);

/**
 * @brief Get charging state
 *
 * @param[out] charging The battery charging state.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 *
 */
int device_battery_is_charging(bool *charging);

/**
 * @brief Set callback to be observing battery charge percentage.
 *
 * @param[in] callback      The callback function to set
 * @param[in] user_data     The user data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 */
int device_battery_set_cb(device_battery_cb callback, void* user_data);

/**
 * @brief Unset battery charge percentage callback function.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 */
int device_battery_unset_cb(void);

/**
 * @brief Checks whether the battery is fully charged.
 * @remarks In order to be notified when the battery state changes, use system_info_set_changed_cb().
 *
 * @param[out] full @c true when the battery is fully charged, otherwise @c false.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 *
 * @see device_battery_get_percent()
 * @see	system_info_set_changed_cb()
 * @see system_info_get_value_int(SYSTEM_INFO_KEY_BATTERY_PERCENTAGE, ...)
 * @see system_info_get_value_int(SYSTEM_INFO_KEY_BATTERY_CHARGE, ...)
 */
int device_battery_is_full(bool *full);

/**
 * @brief Retrive the remaining time for fully charged or discharged.
 *
 * @remark @a time will be retrieved the time to fully charged or discharged depending on @a type
 *
 * @param[in]  type   The type of battery remaining time
 * @param[out] time   battery remainig seconds to fully chagred or discharged

 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 *
 * @see device_battery_set_remaining_time_changed_cb()
 * @see device_battery_unset_remaining_time_changed_cb()
 */
int device_battery_get_remaining_time(device_battery_remaining_time_type_e type, int* time);

/**
 * @brief Set callback to be return battery remaining time to fully charged or discharged.
 *
 * @remark @a callback will be retrieved the time to fully charged or discharged depending on @a type
 *
 * @param[in] callback      The callback function to set
 * @param[in] user_data     The user data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 *
 */
int device_battery_set_remaining_time_changed_cb(
        device_battery_remaining_time_type_e type,
        device_battery_remaining_time_changed_cb callback, void* user_data);

/**
 * @brief Unset battery remaining time callback function.
 *
 * @param[in] type The type of battery remainig time
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 */
int device_battery_unset_remaining_time_changed_cb(device_battery_remaining_time_type_e type);

/**
 * @brief Gets the battery level status.
 *
 * @param[out] status The battery level status.
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 *
 * @see device_battery_level_e
 * @see device_battery_level_set_cb()
 */
int device_battery_get_level_status(device_battery_level_e *status);

/**
 * @brief Set/Unset callback to be observing battery level.
 *
 * @param[in] callback      The callback function to set, if you input NULL, observing is disabled
 * @param[in] user_data     The user data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE			Successful
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 *
 * @see device_battery_level_e
 * @see device_battery_get_level_status()
 */
int device_battery_level_set_cb(device_battery_level_cb callback, void* user_data);

/**
 * @brief Gets the number of display devices.
 *
 * @return The number of display devices that the device provides.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 *
 * @see	device_get_brightness()
 * @see device_set_brightness()
 * @see device_get_max_brightness()
 * @see device_set_brightness_from_settings()
 * @see device_set_brightness_to_settings()
 */
int device_get_display_numbers(int* device_number);

/**
 * @brief Gets the display brightness value.
 *
 * @param[in] display_index	The index of the display, it be greater than or equal to 0 and less than \n
 *                          the number of displays returned by device_get_display_numbers().\n
 *                          The index zero is always assigned to the main display.
 * @param[out] brightness	The current brightness value of the display
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 *
 * @see device_get_display_numbers()
 * @see device_set_brightness()
 * @see device_get_max_brightness()
 * @see device_set_brightness_from_settings()
 * @see device_set_brightness_to_settings()
 */
int device_get_brightness(int display_index, int *brightness);

/**
 * @brief Sets the display brightness value. 
 *
 * @param[in] display_index	The index of the display, it be greater than or equal to 0 and less than \n
 *                          the number of displays returned by device_get_display_numbers().\n
 *                          The index zero is always assigned to the main display.
 * @param[in] brightness	The new brightness value to set \n
 *							The maximum value can be represented by device_get_max_brightness()
 * 
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED   Operation failed
 *
 * @see device_get_display_numbers()
 * @see device_get_max_brightness()
 * @see device_get_brightness()
 * @see device_set_brightness_from_settings()
 * @see device_set_brightness_to_settings()
 */
int device_set_brightness(int display_index, int brightness);

/**
 * @brief Gets the maximum brightness value that can be set. 
 *
 * @param[in] display_index	The index of the display, it be greater than or equal to 0 and less than \n
 *                          the number of displays returned by device_get_display_numbers().\n
 *                          The index zero is always assigned to the main display.
 * @param[out] max_brightness	The maximum brightness value of the display
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 *
 * @see device_get_display_numbers()
 * @see device_set_brightness()
 * @see device_get_brightness()
 * @see device_set_brightness_from_settings()
 * @see device_set_brightness_to_settings()
 */
int device_get_max_brightness(int display_index, int *max_brightness);

/**
 * @brief Sets the display brightness value from registed in settings.
 *
 * @details
 * This function set display brightness to condition in the settings.
 * if auto brightness option is enabled in setting, display's brightness will be changed automatically.
 *
 * @param[in] display_index	The index of the display, it be greater than or equal to 0 and less than \n
 *                          the number of displays returned by device_get_display_numbers().\n
 *                          The index zero is always assigned to the main display.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 *
 * @see device_get_display_numbers()
 * @see device_get_max_brightness()
 * @see device_set_brightness()
 * @see device_get_brightness()
 * @see device_set_brightness_to_settings()
 */
int device_set_brightness_from_settings(int display_index);

/**
 * @brief Sets the display brightness value to specific display and set to variable in settings.
 *
 * @details
 * This function set given brightness value to given index of display.
 * And also brightness variable in settings will be changed to given brightness value too.
 *
 * @param[in] display_index	The index of the display, it be greater than or equal to 0 and less than \n
 *                          the number of displays returned by device_get_display_numbers().\n
 *                          The index zero is always assigned to the main display.
 * @param[in] brightness	The new brightness value to set \n
 *							The maximum value can be represented by device_get_max_brightness()
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 *
 * @see device_get_display_numbers()
 * @see device_get_max_brightness()
 * @see device_set_brightness()
 * @see device_get_brightness()
 * @see device_set_brightness_from_settings()
 */
int device_set_brightness_to_settings(int display_index, int brightness);

/**
 * @brief Get brightness value of LED that placed to camera flash.
 *
 * @param[out] brightness brightness value of LED (0 ~ MAX)
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 */
int device_flash_get_brightness(int *brightness);

/**
 * @brief Set brightness value of LED that placed to camera flash.
 *
 * @param[in] brightness brightness value of LED (0 ~ MAX)
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 */
int device_flash_set_brightness(int brightness);

/**
 * @brief Get max brightness value of LED that placed to camera flash.
 *
 * @remark
 * Brightness control does not support yet. so this functioon always return 1. \n
 * Set function can only use to switch on/off the flash. \n
 * Get function can only use to retrive on/off state of flash.
 *
 * @param[out] max_brightness max brightness value of LED
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 */
int device_flash_get_max_brightness(int *max_brightness);

/**
 * @brief Get total amount of physical RAM, in kilobytes
 *
 * @remark
 *
 * @param[out] total_mem total amount of physical RAM
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 */
int device_memory_get_total(unsigned int *total_mem);

/**
 * @brief Get available amount of physical RAM, in kilobytes
 *
 * @remark
 * Available amount is defined by following formula currently.
 * available mem = MemFree+Buffers+Cached+SwapCached-Shmem
 *
 * @param[out] avail_mem available amount of physical RAM
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 */
int device_memory_get_available(unsigned int *avail_mem);

/**
 * @brief Get time information the CPU has spent performing work.
 *
 * @remark
 * Time units are in USER_HZ (typically hundredths of a second).
 *
 * @param[out] time structure of time information the CPU has spent
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 */
int device_cpu_get_system_time(device_system_time_s *time);

/**
 * @brief Get all of CPU count
 *
 * @remark
 *
 * @param[out] cpu_cnt total count of CPU
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 */
int device_cpu_get_count(int *cpu_cnt);

/**
 * @brief Get currently frequency of CPU
 *
 * @remark
 *
 * @param[in]  cpu the index of CPU which want to know
 * @param[out] cur_freq currently frequency value of CPU
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 */
int device_cpu_get_current_freq(int cpu, unsigned int *cur_freq);

/**
 * @brief Get max frequency of CPU
 *
 * @remark
 *
 * @param[in]  cpu the index of CPU which want to know
 * @param[out] max_freq max frequency value of CPU
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 */
int device_cpu_get_max_freq(int cpu, unsigned int *max_freq);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  // __TIZEN_SYSTEM_DEVICE_H__
