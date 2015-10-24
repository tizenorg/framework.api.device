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


#ifndef __TIZEN_SYSTEM_LED_H__
#define __TIZEN_SYSTEM_LED_H__

#include "device-error.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup CAPI_SYSTEM_DEVICE_LED_MODULE
 * @{
 */

/**
 * @brief Gets the max brightness value of a LED that is located next to the camera.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/led
 *
 * @param[out] max_brightness The max brightness value of the LED
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
int device_flash_get_max_brightness(int *max_brightness);

/**
 * @brief Gets the brightness value of a LED that is located next to the camera.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/led
 *
 * @param[out] brightness The brightness value of LED (@c 0 ~ MAX)
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
int device_flash_get_brightness(int *brightness);

/**
 * @brief Sets the brightness value of a LED that is located next to the camera.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/led
 *
 * @remarks Since 2.4, this API check camera flash status whether camera API preempted flash or not, so it could be failed if flash was preempted by camera API. In this case, API will return #DEVICE_ERROR_RESOURCE_BUSY error.
 *
 * @param[in] brightness The brightness value of LED (@c 0 ~ MAX)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DEVICE_ERROR_NONE               Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED   Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED      Not supported device
 * @retval #DEVICE_ERROR_RESOURCE_BUSY      Device or resource busy
 */
int device_flash_set_brightness(int brightness);

/**
 * @brief Enumeration for custom LED flags.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum {
    LED_CUSTOM_DUTY_ON = 1 << 0,                /**< blink LED */
    LED_CUSTOM_DEFAULT = (LED_CUSTOM_DUTY_ON),  /**< Default flag */
} led_custom_flags;

/**
 * @brief Plays the custom effect of the service LED that is located to the front of a device.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/led
 *
 * @param[in] on    Turn on time in milliseconds
 * @param[in] off   Turn off time in milliseconds
 * @param[in] color The Color value \n
 *                  The first byte means opaque and the other 3 bytes are RGB values.
 * @param[in] flags The combination of enum #led_custom_flags
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
int device_led_play_custom(int on, int off, unsigned int color, unsigned int flags);

/**
 * @brief Stops the custom effect of the service LED that is located to the front of a device.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/led
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
int device_led_stop_custom(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  // __TIZEN_SYSTEM_LED_H__
