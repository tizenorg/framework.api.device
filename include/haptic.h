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


#ifndef __TIZEN_SYSTEM_HAPTIC_H__
#define __TIZEN_SYSTEM_HAPTIC_H__

#include "device-error.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup CAPI_SYSTEM_DEVICE_HAPTIC_MODULE
 * @{
 */

/**
 * @brief The haptic device handle.
 */
typedef void* haptic_device_h;

/**
 * @brief The haptic effect handle.
 */
typedef void* haptic_effect_h;

/**
 * @brief Gets the number of vibrators.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/haptic
 *
 * @param[in] device_number The number of vibrators
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DEVICE_ERROR_NONE               Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED   Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED      Not supported device
 */
int device_haptic_get_count(int *device_number);

/**
 * @brief Opens a haptic-vibration device.
 * @details Internally, it makes a connection to the vibrator.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/haptic
 *
 * @remarks You must close the Haptic API using device_haptic_close().
 *
 * @param[in] device_index The index of device what you want to vibrate
 * @param[out] device_handle The handle of vibrator
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DEVICE_ERROR_NONE               Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED   Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED      Not supported device
 *
 * @see device_haptic_close()
 */
int device_haptic_open(int device_index, haptic_device_h *device_handle);

/**
 * @brief Closes a haptic-vibration device.
 * @details Internally, it disconnects the connection to the vibrator.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/haptic
 *
 * @param[in] device_handle The device handle from device_haptic_open()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DEVICE_ERROR_NONE               Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED   Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED      Not supported device
 *
 * @see device_haptic_open()
 */
int device_haptic_close(haptic_device_h device_handle);

/**
 * @brief Vibrates during the specified time with a constant intensity.
 * @details This function can be used to start monotonous vibration for the specified time.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/haptic
 *
 * @remarks @a feedback level is reserved for auto changing to save variable in the settings.
 * @remarks @a effect_handle effect_handle value can be @c 0(zero).
 *
 * @param[in] device_handle The device handle from device_haptic_open()
 * @param[in] duration The play duration in milliseconds
 * @param[in] feedback The amount of the intensity variation (@c 0 ~ @c 100)
 * @param[out] effect_handle The pointer to the variable that will receive a handle to the playing effect
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DEVICE_ERROR_NONE               Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED   Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED      Not supported device
 *
 * @see device_haptic_stop()
 */

int device_haptic_vibrate(haptic_device_h device_handle,
        int duration, int feedback, haptic_effect_h *effect_handle);

/**
 * @brief Stops all vibration effects which are being played.
 * @details This function can be used to stop all effects started by device_haptic_vibrate().
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/haptic
 *
 * @param[in] device_handle The device handle from device_haptic_open()
 * @param[in] effect_handle The effect handle from device_haptic_vibrate()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DEVICE_ERROR_NONE               Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED   Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED      Not supported device
 *
 * @see device_haptic_vibrate()
 */
int device_haptic_stop(haptic_device_h device_handle, haptic_effect_h effect_handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  // __TIZEN_SYSTEM_HAPTIC_H__
