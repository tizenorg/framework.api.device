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


#ifndef __TIZEN_SYSTEM_DEVICE_ERROR_H__
#define __TIZEN_SYSTEM_DEVICE_ERROR_H__

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
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum
{
	DEVICE_ERROR_NONE                = TIZEN_ERROR_NONE,                /**< Successful */
	DEVICE_ERROR_OPERATION_FAILED    = TIZEN_ERROR_NOT_PERMITTED,       /**< Operation not permitted */
	DEVICE_ERROR_PERMISSION_DENIED   = TIZEN_ERROR_PERMISSION_DENIED,   /**< Permission denied */
	DEVICE_ERROR_INVALID_PARAMETER   = TIZEN_ERROR_INVALID_PARAMETER,   /**< Invalid parameter */
	DEVICE_ERROR_ALREADY_IN_PROGRESS = TIZEN_ERROR_ALREADY_IN_PROGRESS, /**< Operation already in progress */
	DEVICE_ERROR_NOT_SUPPORTED       = TIZEN_ERROR_NOT_SUPPORTED,       /**< Not supported in this device */
	DEVICE_ERROR_NOT_INITIALIZED     = TIZEN_ERROR_DEVICE | 0x13,       /**< Not initialized */
} device_error_e;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  // __TIZEN_SYSTEM_DEVICE_ERROR_H__
