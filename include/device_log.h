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

#ifndef __DEVICE_LOG_H__
#define __DEVICE_LOG_H__

#ifdef FEATURE_DEVICE_DLOG
    #define LOG_TAG "SYSTEM_DEVICE"
    #include <dlog.h>
    #define DEVICE_LOG(fmt, args...)       SLOGD(fmt, ##args)
    #define DEVICE_ERROR(fmt, args...)     SLOGE(fmt, ##args)
#else
    #define DEVICE_LOG(x, ...)
    #define DEVICE_ERROR(x, ...)
#endif

#endif /* __DEVICE_LOG_H__ */
