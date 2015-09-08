/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
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


#ifndef __TIZEN_SYSTEM_BATTERY_H__
#define __TIZEN_SYSTEM_BATTERY_H__

#include <stdbool.h>
#include "device-error.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup CAPI_SYSTEM_DEVICE_BATTERY_MODULE
 * @{
 */

/**
 * @brief Enumeration for the battery level status.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum
{
    DEVICE_BATTERY_LEVEL_EMPTY = 0,  /**< The battery goes empty. Prepare for the safe termination of the application, because the device starts a shutdown process soon after entering this level. */
    DEVICE_BATTERY_LEVEL_CRITICAL,   /**< The battery charge is at a critical state. You may have to stop using multimedia features, because they are not guaranteed to work correctly at this battery status. */
    DEVICE_BATTERY_LEVEL_LOW,        /**< The battery has little charge left. */
    DEVICE_BATTERY_LEVEL_HIGH,       /**< The battery status is not to be careful. */
    DEVICE_BATTERY_LEVEL_FULL,       /**< The battery status is full. */
} device_battery_level_e;

/**
 * @brief Gets the battery charge percentage.
 * @details It returns an integer value from @c 0 to @c 100 that indicates remaining battery charge
 *          as a percentage of the maximum level.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * 
 * @remarks In order to be notified when the battery state changes, use system_info_set_changed_cb().
 *
 * @param[out] percent The remaining battery charge percentage (@c 0 ~ @c 100)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DEVICE_ERROR_NONE               Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED   Operation failed
 */
int device_battery_get_percent(int *percent);

/**
 * @brief Gets the charging state.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[out] charging The battery charging state
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DEVICE_ERROR_NONE               Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED   Operation failed
 *
 * @see device_add_callback
 * @see device_remove_callback
 * @see #DEVICE_CALLBACK_BATTERY_CHARGING
 */
int device_battery_is_charging(bool *charging);

/**
 * @brief Gets the battery level status.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[out] status The battery level status
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DEVICE_ERROR_NONE               Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED   Operation failed
 *
 * @see device_battery_level_e
 * @see device_add_callback
 * @see device_remove_callback
 * @see #DEVICE_CALLBACK_BATTERY_LEVEL
 */
int device_battery_get_level_status(device_battery_level_e *status);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  // __TIZEN_SYSTEM_BATTERY_H__
