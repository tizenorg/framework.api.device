/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef __TIZEN_SYSTEM_DEVICE_DOC_H__
#define __TIZEN_SYSTEM_DEVICE_DOC_H__

/**
 * @ingroup CAPI_SYSTEM_FRAMEWORK
 * @defgroup CAPI_SYSTEM_DEVICE_MODULE Device
 * @brief  The DEVICE API provides functions to control devices or to get status of devices.
 *
 * @section CAPI_SYSTEM_DEVICE_MODULE_HEADER Required Header
 *   \#include <device/battery.h> \n
 *   \#include <device/callback.h> \n
 *   \#include <device/display.h> \n
 *   \#include <device/haptic.h> \n
 *   \#include <device/led.h> \n
 *   \#include <device/power.h>
 *
 * @section CAPI_SYSTEM_DEVICE_MODULE_OVERVIEW Overview
 * The DEVICE API provides functions to control devices or to get status of devices.
 *
 *  This API allows checking of the following parameters:
 * - Battery
 * - Display
 * - Haptic
 * - LED
 * - Power
 */

/**
 * @ingroup CAPI_SYSTEM_DEVICE_MODULE
 * @defgroup CAPI_SYSTEM_DEVICE_BATTERY_MODULE Battery
 * @brief The Battery API provides functions to get information about the battery.
 *
 * @section CAPI_SYSTEM_DEVICE_BATTERY_MODULE_HEADER Required Header
 *   \#include <device/battery.h> \n
 *   \#include <device/callback.h> \n
 *
 * @section CAPI_SYSTEM_DEVICE_BATTERY_MODULE_OVERVIEW Overview
 * The Battery API provides the way to get the current battery capacity value, battery state and charging state.
 * It also supports the API for an application to receive the battery events from the system.
 * To receive the battery event it should be described by the callback function.
 *
 */

/**
 * @ingroup CAPI_SYSTEM_DEVICE_MODULE
 * @defgroup CAPI_SYSTEM_DEVICE_DISPLAY_MODULE Display
 * @brief The Display API provides functions to control the display status.
 *
 * @section CAPI_SYSTEM_DEVICE_DISPLAY_MODULE_HEADER Required Header
 *   \#include <device/display.h> \n
 *   \#include <device/callback.h> \n
 *
 * @section CAPI_SYSTEM_DEVICE_DISPLAY_MODULE_OVERVIEW Overview
 * The Display API provides the way to get supported display count and the display brightness.
 * It also supports the API to set the display brightness.
 * Application can receive the display event by callback function from the system.
 *
 */

/**
 * @ingroup CAPI_SYSTEM_DEVICE_MODULE
 * @defgroup CAPI_SYSTEM_DEVICE_HAPTIC_MODULE Haptic
 * @brief The Haptic API provides functions to control a vibrator.
 *
 * @section CAPI_SYSTEM_DEVICE_HAPTIC_MODULE_HEADER Required Header
 *   \#include <device/haptic.h> \n
 *
 * @section CAPI_SYSTEM_DEVICE_HAPTIC_MODULE_OVERVIEW Overview
 * The Haptic API provides the way to control vibration functionality of a device.
 * It allows the management of the device's vibrator parameters, such as the vibration count and level.
 *
 */

/**
 * @ingroup CAPI_SYSTEM_DEVICE_MODULE
 * @defgroup CAPI_SYSTEM_DEVICE_LED_MODULE Led
 * @brief The Led API provides functions to control the attached led device.
 *
 * @section CAPI_SYSTEM_DEVICE_LED_MODULE_HEADER Required Header
 *   \#include <device/led.h> \n
 *
 * @section CAPI_SYSTEM_DEVICE_LED_MODULE_OVERVIEW Overview
 * The Led API provides the way to control the attached LED device such as the camera flash and service LED.
 * It supports to turn on the camera flash and set the pattern to the service LED which is located to the front of a device.
 * @section CAPI_SYSTEM_DEVICE_LED_MODULE_FEATURE Related Features
 * This API is related with the following features:\n
 *  - http://tizen.org/feature/led\n
 *  - http://tizen.org/feature/camera.back.flash\n
 *
 * It is recommended to design feature related codes in your application for reliability.\n
 *
 * You can check if a devrice supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE, thereby controlling the procedure of your application.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="https://developer.tizen.org/development/tools/native-tools/manifest-text-editor#feature"><b>Feature Element</b>.</a>
 *
 */

/**
 * @ingroup CAPI_SYSTEM_DEVICE_MODULE
 * @defgroup CAPI_SYSTEM_DEVICE_POWER_MODULE Power
 * @brief The Power API provides functions to control the power service.
 *
 * @section CAPI_SYSTEM_DEVICE_POWER_MODULE_HEADER Required Header
 *   \#include <device/power.h> \n
 *
 * @section CAPI_SYSTEM_DEVICE_POWER_MODULE_OVERVIEW Overview
 * The Power API provides the way to control the power service.
 * It can be made to hold the specific state to avoid changing display and CPU state internally.
 *
 */

/**
 * @ingroup CAPI_SYSTEM_DEVICE_MODULE
 * @defgroup CAPI_SYSTEM_DEVICE_CALLBACK_MODULE Callback
 * @brief The Callback API provides functions to observe the changing of device state.
 *
 * @section CAPI_SYSTEM_DEVICE_CALLBACK_MODULE_HEADER Required Header
 *   \#include <device/callback.h> \n
 *
 * @section CAPI_SYSTEM_DEVICE_CALLBACK_MODULE_OVERVIEW Overview
 * The Callback API provides the way to observe the changing of device state.
 *
 *  This API allows observing of the following events:
 * - Battery capacity
 * - Battery level
 * - Battery charging
 * - Display state
 * - Flash led state
 */

#endif /* __TIZEN_SYSTEM_DEVICE_DOC_H__ */
