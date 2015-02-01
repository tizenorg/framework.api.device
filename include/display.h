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


#ifndef __TIZEN_SYSTEM_DISPLAY_H__
#define __TIZEN_SYSTEM_DISPLAY_H__

#include "device-error.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup CAPI_SYSTEM_DEVICE_DISPLAY_MODULE
 * @{
 */

/**
 * @brief Gets the number of display devices.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/display
 *
 * @param[out] device_number The total number of displays
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DEVICE_ERROR_NONE               Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED   Operation failed
 *
 * @see device_display_get_brightness()
 * @see device_display_set_brightness()
 * @see device_display_get_max_brightness()
 */
int device_display_get_numbers(int *device_number);

/**
 * @brief Gets the maximum brightness value that can be set.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/display
 *
 * @param[in] display_index     The index of the display \n
 *                              It can be greater than or equal to @c 0 and less than
 *                              the number of displays returned by device_display_get_numbers(). \n
 *                              The index zero is always assigned to the main display.
 * @param[out] max_brightness   The maximum brightness value of the display
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DEVICE_ERROR_NONE               Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED   Operation failed
 *
 * @see device_display_get_numbers()
 * @see device_display_set_brightness()
 * @see device_display_get_brightness()
 */
int device_display_get_max_brightness(int display_index, int *max_brightness);

/**
 * @brief Gets the display brightness value.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/display
 *
 * @param[in] display_index The index of the display \n
 *                          It can be greater than or equal to @c 0 and less than
 *                          the number of displays returned by device_display_get_numbers(). \n
 *                          The index zero is always assigned to the main display.
 * @param[out] brightness   The current brightness value of the display
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DEVICE_ERROR_NONE               Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED   Operation failed
 *
 * @see device_display_get_numbers()
 * @see device_display_set_brightness()
 * @see device_display_get_max_brightness()
 */
int device_display_get_brightness(int display_index, int *brightness);

/**
 * @brief Sets the display brightness value.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/display
 *
 * @param[in] display_index The index of the display \n
 *                          It can be greater than or equal to @c 0 and less than
 *                          the number of displays returned by device_display_get_numbers(). \n
 *                          The index zero is always assigned to the main display.
 * @param[in] brightness    The new brightness value to set \n
 *                          The maximum value can be represented by device_display_get_max_brightness().
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DEVICE_ERROR_NONE               Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED   Operation failed
 *
 * @see device_display_get_numbers()
 * @see device_display_get_max_brightness()
 * @see device_display_get_brightness()
 */
int device_display_set_brightness(int display_index, int brightness);

/**
 * @brief   Enumeration for the available display states.
 * @details An application cannot put the device into the power off state or the suspend state.
 * @since_tizen 2.3
 */
typedef enum
{
    DISPLAY_STATE_NORMAL,      /**< Normal state */
    DISPLAY_STATE_SCREEN_DIM,  /**< Screen dim state */
    DISPLAY_STATE_SCREEN_OFF,  /**< Screen off state */
} display_state_e;

/**
 * @brief Gets the current display state.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/display
 *
 * @param[out] state the display state
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
 * @see #DEVICE_CALLBACK_DISPLAY_STATE
 */
int device_display_get_state(display_state_e *state);

/**
 * @brief Changes the display state by force.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/display
 *
 * @param[in] state the display state
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
 * @see device_power_release_lock()
 * @see device_add_callback
 * @see device_remove_callback
 * @see #DEVICE_CALLBACK_DISPLAY_STATE
 *
 * @par Example
 * @code
 *  ...
 *  result = device_display_change_state(DISPLAY_STATE_SCREEN_OFF);
 *  if( result < 0 )
 *      printf("[ERROR] return value result =%d, \n",result);
 *  else
 *      printf("[SUCCESS] return value result =%d \n",result);
 *  ...
 * @endcode
 */
int device_display_change_state(display_state_e state);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  // __TIZEN_SYSTEM_DISPLAY_H__
