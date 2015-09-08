/*
 * system-dbus
 *
 * Copyright (c) 2012 - 2013 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef __DBUS_H__
#define __DBUS_H__

#include <dbus/dbus.h>

#define DEVICED_BUS_NAME		"org.tizen.system.deviced"
#define DEVICED_OBJECT_PATH		"/Org/Tizen/System/DeviceD"
#define DEVICED_INTERFACE_NAME	DEVICED_BUS_NAME

/* Display service: start/stop display(pm), get/set brightness operations about display */
#define DEVICED_PATH_DISPLAY                DEVICED_OBJECT_PATH"/Display"
#define DEVICED_INTERFACE_DISPLAY           DEVICED_INTERFACE_NAME".display"

/* Battery service */
#define DEVICED_PATH_BATTERY                DEVICED_OBJECT_PATH"/Battery"
#define DEVICED_INTERFACE_BATTERY           DEVICED_INTERFACE_NAME".Battery"

/* Haptic service: operatioins about haptic */
#define DEVICED_PATH_HAPTIC                 DEVICED_OBJECT_PATH"/Haptic"
#define DEVICED_INTERFACE_HAPTIC            DEVICED_INTERFACE_NAME".haptic"

/* Led service: play/stop led operations about led */
#define DEVICED_PATH_LED                    DEVICED_OBJECT_PATH"/Led"
#define DEVICED_INTERFACE_LED               DEVICED_INTERFACE_NAME".Led"

/* Power service: request to reboot */
#define DEVICED_PATH_POWER                  DEVICED_OBJECT_PATH"/Power"
#define DEVICED_INTERFACE_POWER             DEVICED_INTERFACE_NAME".power"

int dbus_method_sync(const char *dest, const char *path,
		const char *interface, const char *method,
		const char *sig, char *param[]);

typedef void (*dbus_pending_cb)(void *data, DBusMessage *msg, DBusError *err);

int dbus_method_async_with_reply(const char *dest, const char *path,
		const char *interface, const char *method,
		const char *sig, char *param[], dbus_pending_cb cb, int timeout, void *data);

#endif
