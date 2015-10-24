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

#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef FEATURE_DEVICE_DLOG
    #define LOG_TAG "SYSTEM_DEVICE"
    #include <dlog.h>
    #define _D(fmt, args...)	SLOGD(fmt, ##args)
    #define _I(fmt, args...)	SLOGI(fmt, ##args)
    #define _E(fmt, args...)	SLOGE(fmt, ##args)
#else
    #define _D(x, ...)
    #define _I(x, ...)
    #define _E(x, ...)
#endif

#ifndef __CONSTRUCTOR__
#define __CONSTRUCTOR__ __attribute__ ((constructor))
#endif

#include "device-error.h"

static inline int errno_to_device_error(int err)
{
	if (err == -EACCES)
		return DEVICE_ERROR_PERMISSION_DENIED;
	else if (err == -EBUSY)
		return DEVICE_ERROR_RESOURCE_BUSY;
	else if (err == -ENOTSUP || err == -ENODEV || err == -ENOENT)
		return DEVICE_ERROR_NOT_SUPPORTED;
	else if (err < 0)
		return DEVICE_ERROR_OPERATION_FAILED;

	return DEVICE_ERROR_NONE;
}

#endif /* __COMMON_H__ */
