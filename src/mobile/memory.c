/*
 * capi-system-device
 * Copyright (c) 2012 Samsung Electronics Co., Ltd.
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "device.h"
#include "device_log.h"

#define PROC_MEMORY		"/proc/meminfo"

enum {
	_MI_TOTAL,
	_MI_FREE,
	_MI_BUF,
	_MI_CACHED,
	_MI_SWAP,
	_MI_SHMEM,
	_MI_MAX,
};

struct meminfo {
	unsigned long l[_MI_MAX];
};

static const char *_mi_str[] = {
	[_MI_TOTAL] = "MemTotal",
	[_MI_FREE] = "MemFree",
	[_MI_BUF] = "Buffers",
	[_MI_CACHED] = "Cached",
	[_MI_SWAP] = "SwapCached",
	[_MI_SHMEM] = "Shmem",
};

static inline int _get_idx(const char *name)
{
	int i;

	for (i = 0; i < sizeof(_mi_str)/sizeof(_mi_str[0]); ++i) {
		if (name && _mi_str[i] && !strcmp(name, _mi_str[i]))
			return i;
	}

	return -1;
}

static int _get_meminfo(struct meminfo *mi)
{
	FILE *fp;
	char buf[4096];
	char *s, *name, *num;
	int idx;

	assert(mi);

	fp = fopen(PROC_MEMORY, "r");
	if (!fp)
		return -1;

	memset(mi, 0, sizeof(*mi));

	while (!feof(fp)) {
		s = fgets(buf, sizeof(buf), fp);
		if (!s)
			break;

		s = strchr(buf, ':');
		if (!s)
			continue;

		*s = '\0';
		name = buf;

		s++;
		while (*s == ' ')
			s++;
		num = s;

		idx = _get_idx(name);
		if (idx < 0)
			continue;

		mi->l[idx] = strtol(num, NULL, 10);
	}

	fclose(fp);
	return 0;
}

int device_memory_get_total(unsigned int *total_mem)
{
	struct meminfo mi;
	int ret;

	if (total_mem == NULL)
		return DEVICE_ERROR_INVALID_PARAMETER;

	ret = _get_meminfo(&mi);
	if (ret < 0)
		return DEVICE_ERROR_OPERATION_FAILED;

	*total_mem = mi.l[_MI_TOTAL];
	return DEVICE_ERROR_NONE;
}

int device_memory_get_available(unsigned int *avail_mem)
{
	struct meminfo mi;
	int ret;

	if (avail_mem == NULL)
		return DEVICE_ERROR_INVALID_PARAMETER;

	ret = _get_meminfo(&mi);
	if (ret < 0)
		return DEVICE_ERROR_OPERATION_FAILED;

	*avail_mem = mi.l[_MI_FREE] + mi.l[_MI_BUF] + mi.l[_MI_CACHED] + mi.l[_MI_SWAP] - mi.l[_MI_SHMEM];
	return DEVICE_ERROR_NONE;
}
