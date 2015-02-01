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


#ifndef __TIZEN_SYSTEM_POWER_H__
#define __TIZEN_SYSTEM_POWER_H__

#include <stdbool.h>
#include "device-error.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup CAPI_SYSTEM_DEVICE_POWER_MODULE
 * @{
 */

/**
 * @brief Enumeration for lock type.
 * @details Each enum ensures that the suitable device is on
 *          until all the lock requests have been released or after a timeout. \n
 *                                 CPU    Brightness \n
 *          POWER_LOCK_CPU          ON      OFF \n
 *          POWER_LOCK_DISPLAY      ON      ON(normal) \n
 *          POWER_LOCK_DISPLAY_DIM  ON      ON(dim)
 *
 * @since_tizen 2.3
 *
 * @remarks An application can lock the specific type.
 * @remarks These enums are mutually exclusive.
 * @remarks You cannot combine with an enum below.
 *
 */
typedef enum
{
    POWER_LOCK_CPU,         /**< CPU lock */
    POWER_LOCK_DISPLAY,     /**< Display normal lock */
    POWER_LOCK_DISPLAY_DIM, /**< Display dim lock */
} power_lock_e;

/**
 * @brief Locks the given lock state for a specified time.
 * @details After the given @a timeout_ms (in milliseconds), unlock the given lock state automatically.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/display
 *
 * @remarks If the process dies, then every lock will be removed.
 *
 * @param[in] type          The power type to request lock
 * @param[in] timeout_ms    The positive number in milliseconds or @c 0 for permanent lock \n
 *                          So you must release the permanent lock of power state with #device_power_release_lock() if @a timeout_ms is zero.
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DEVICE_ERROR_NONE               Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED   Operation failed
 *
 * @see device_power_release_lock()
 */
int device_power_request_lock(power_lock_e type, int timeout_ms);

/**
 * @brief Releases the given lock state locked before.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/display
 *
 * @param[in] type The power type to release lock
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DEVICE_ERROR_NONE               Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED   Operation failed
 *
 * @see device_power_request_lock()
 */
int device_power_release_lock(power_lock_e type);

/**
 * @brief Changes the current power state to the normal/dim state.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/display
 *
 * @param[in] dim Set @c true to set the dim state,
 *                otherwise set @c false to not set the dim state
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DEVICE_ERROR_NONE               Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED   Operation failed
 *
 * @post The device will be in #DISPLAY_STATE_NORMAL state.
 */
int device_power_wakeup(bool dim);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  // __TIZEN_SYSTEM_POWER_H__
