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


#ifndef __TIZEN_SYSTEM_CALLBACK_H__
#define __TIZEN_SYSTEM_CALLBACK_H__

#include <stdbool.h>
#include "device-error.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup CAPI_SYSTEM_DEVICE_CALLBACK_MODULE
 * @{
 */

/**
 * @brief Enumeration for the device state callback.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum
{
    DEVICE_CALLBACK_BATTERY_CAPACITY,    /**< Called when a battery charge percentage is changed */
    DEVICE_CALLBACK_BATTERY_LEVEL,       /**< Called when a battery level is changed */
    DEVICE_CALLBACK_BATTERY_CHARGING,    /**< Called when battery charging state is changed */
    DEVICE_CALLBACK_DISPLAY_STATE,       /**< Called when a display state is changed */
    DEVICE_CALLBACK_MAX
} device_callback_e;

/**
 * @brief Called when a device status is changed.
 * @details Each device callback has a different output param type. \n
 *          So you need to check below output param before using this function. \n
 *                  callback enum               output type \n
 *          DEVICE_CALLBACK_BATTERY_CAPACITY        int \n
 *          DEVICE_CALLBACK_BATTERY_LEVEL           int \n
 *          DEVICE_CALLBACK_BATTERY_CHARGING        bool \n
 *          DEVICE_CALLBACK_DISPLAY_STATE           int
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[out] type          The device type to monitor
 * @param[out] value         The changed value \n
 * @param[out] user_data     The user data passed from the callback registration function
 */
typedef void (*device_changed_cb)(device_callback_e type, void *value, void *user_data);

/**
 * @brief Adds a callback to the observing device state.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] type          The device type to monitor
 * @param[in] callback      The callback function to add
 * @param[in] user_data     The user data to be passed to the callback function
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DEVICE_ERROR_NONE                   Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER      Invalid parameter
 * @retval #DEVICE_ERROR_ALREADY_IN_PROGRESS    Operation already
 * @retval #DEVICE_ERROR_OPERATION_FAILED       Operation failed
 */
int device_add_callback(device_callback_e type, device_changed_cb callback, void *user_data);

/**
 * @brief Removes a device callback function.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] type          The device type to monitor
 * @param[in] callback      The callback function to remove
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DEVICE_ERROR_NONE               Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED   Operation failed
 */
int device_remove_callback(device_callback_e type, device_changed_cb callback);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  // __TIZEN_SYSTEM_CALLBACK_H__
