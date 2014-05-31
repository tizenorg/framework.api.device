/*
 * 
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 * PROPRIETARY/CONFIDENTIAL
 * 
 * This software is the confidential and proprietary information of SAMSUNG 
 * ELECTRONICS ("Confidential Information"). You agree and acknowledge that 
 * this software is owned by Samsung and you shall not disclose such 
 * Confidential Information and shall use it only in accordance with the terms 
 * of the license agreement you entered into with SAMSUNG ELECTRONICS. SAMSUNG 
 * make no representations or warranties about the suitability of the software, 
 * either express or implied, including but not limited to the implied 
 * warranties of merchantability, fitness for a particular purpose, or 
 * non-infringement. SAMSUNG shall not be liable for any damages suffered by 
 * licensee arising out of or related to this software.
 * 
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <glib.h>
#include <device.h>

int main(int argc, char *argv[])
{
	int pct;
	bool is_full;
	int brt, max, cnt;

	if(device_battery_get_percent(&pct) == DEVICE_ERROR_NONE){
		printf("battery percent is %d %%\n", pct);
	}else{
		printf("device_battery_get_percent error\n");
	}

	if(device_battery_is_full(&is_full) == DEVICE_ERROR_NONE){
		printf("battery is %s\n", is_full ? "full" : "not full");
	}else{
		printf("device_battery_is_full error\n");
	}

	if(device_get_max_brightness(0, &max) == DEVICE_ERROR_NONE){
		printf("display brightness max is %d\n", max);
	}else{
		printf("device_get_max_brightness error\n");
	}

    cnt = device_get_display_count();
    printf("display count is %d\n", cnt);

	if(argc > 1){
		brt = atoi(argv[1]);
	}else{
		brt = max / 2;
	}
	if(device_set_brightness(0, brt) == DEVICE_ERROR_NONE){
		printf("set brightness %d\n", brt);
	}else{
		printf("device_set_brightness error\n");
	}

	if(device_get_brightness(0, &brt) == DEVICE_ERROR_NONE){
		printf("display brightness is %d\n", brt);
	}else{
		printf("device_get_brightness error\n");
	}

	return 0;
}
