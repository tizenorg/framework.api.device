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

#define PROC_STAT		"/proc/stat"

#define PROC_CPU_PRESENT				"/sys/devices/system/cpu/present"
#define PROC_SCALING_CUR_FREQ			"/sys/devices/system/cpu/cpu%d/cpufreq/scaling_cur_freq"
#define PROC_SCALING_MAX_FREQ			"/sys/devices/system/cpu/cpu%d/cpufreq/scaling_max_freq"


int device_cpu_get_count(int *cpu_cnt)
{
	FILE *fp;
	int ret;
	int st, ed;

	if (cpu_cnt == NULL)
		return DEVICE_ERROR_INVALID_PARAMETER;

	fp = fopen(PROC_CPU_PRESENT, "r");
	if (!fp)
		return DEVICE_ERROR_OPERATION_FAILED;

	ret = fscanf(fp, "%u-%u", &st, &ed);
	fclose(fp);
	if (ret != 2)
		return DEVICE_ERROR_OPERATION_FAILED;

	*cpu_cnt = ed+1;
	return DEVICE_ERROR_NONE;
}

static int _get_systime(device_system_time_s *st)
{
	FILE *fp;
	char buf[4096];
	char *s;

	assert(st);

	fp = fopen(PROC_STAT, "r");
	if (!fp)
		return -1;

	s = fgets(buf, sizeof(buf), fp);
	fclose(fp);
	if (!s)
		return -1;

	s = strchr(buf, ' ');
	if (!s)
		return -1;

	s++;
	st->user = strtol(s, &s, 10);
	st->nice = strtol(s, &s, 10);
	st->system = strtol(s, &s, 10);
	st->idle = strtol(s, &s, 10);
	st->iowait = strtol(s, &s, 10);
	st->irq = strtol(s, &s, 10);
	st->softirq = strtol(s, &s, 10);

	return 0;
}

int device_cpu_get_system_time(device_system_time_s *time)
{
	int ret;
	device_system_time_s st;

	if (time == NULL)
		return DEVICE_ERROR_INVALID_PARAMETER;

	ret = _get_systime(&st);
	if (ret < 0)
		return DEVICE_ERROR_OPERATION_FAILED;

	st.total = st.user+st.nice+st.system+st.idle+st.iowait+st.irq+st.softirq;
	*time = st;
	return DEVICE_ERROR_NONE;
}

static int _get_uint(const char *path, unsigned int *val)
{
	FILE *fp;
	unsigned int num;
	int ret;

	assert(path);
	assert(val);

	fp = fopen(path, "r");
	if (!fp)
		return -1;

	ret = fscanf(fp, "%u", &num);
	fclose(fp);
	if (ret != 1)
		return -1;

	*val = num;
	return 0;
}

int device_cpu_get_current_freq(int cpu, unsigned int *cur_freq)
{
	char path[FILENAME_MAX];
	int ret;
	int count;
	unsigned int cur;

	if (cur_freq == NULL)
		return DEVICE_ERROR_INVALID_PARAMETER;

	ret = device_cpu_get_count(&count);
	if (ret != DEVICE_ERROR_NONE || cpu < 0 || cpu >= count)
		return DEVICE_ERROR_INVALID_PARAMETER;

	snprintf(path, sizeof(path), PROC_SCALING_CUR_FREQ, cpu);
	ret = _get_uint(path, &cur);
	if (ret < 0)
		cur = 0;

	*cur_freq = cur;
	return DEVICE_ERROR_NONE;
}

int device_cpu_get_max_freq(int cpu, unsigned int *max_freq)
{
	char path[FILENAME_MAX];
	int ret;
	int count;
	unsigned int max;

	if (max_freq == NULL)
		return DEVICE_ERROR_INVALID_PARAMETER;

	ret = device_cpu_get_count(&count);
	if (ret != DEVICE_ERROR_NONE || cpu < 0 || cpu >= count)
		return DEVICE_ERROR_INVALID_PARAMETER;

	snprintf(path, sizeof(path), PROC_SCALING_MAX_FREQ, cpu);
	ret = _get_uint(path, &max);
	if (ret < 0)
		max = 0;

	*max_freq = max;
	return DEVICE_ERROR_NONE;
}
