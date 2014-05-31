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
#include <stdlib.h>
#include <glib.h>
#include <stdbool.h>
#include <device.h>

static GMainLoop *mainloop;

void battery_cb(int percent, void* ud)
{
    char* txt = (char*)ud;
    printf("battery capacity [%d] -- %s\n", percent, txt); 
}

static void sig_quit(int signo)
{
	if(mainloop)
	{
		g_main_loop_quit(mainloop);
	}
}

static void errp(device_error_e e){
    switch(e){
        case DEVICE_ERROR_INVALID_PARAMETER:
            printf("invalid parameter!\n");
            break;
        case DEVICE_ERROR_OPERATION_FAILED:
            printf("operation failed!\n");
            break;
        case DEVICE_ERROR_NONE:
            printf("success!\n");
            break;
        default:
            printf("unknown!\n");
    }
}

int main(int argc, char *argv[])
{
    int err;

	signal(SIGINT, sig_quit);
	signal(SIGTERM, sig_quit);
	signal(SIGQUIT, sig_quit);

	mainloop = g_main_loop_new(NULL, FALSE);

    bool charging;
    if( (err = device_battery_is_charging(&charging)) < 0){
        printf("is charging return ");
        errp(err);
    }
    printf("charging state -> %s\n", charging?"charging...":"not charging");

    if( (err=device_battery_set_cb(battery_cb, "PIUS!!")) < 0){
        printf("set cb return ");
        errp(err);
    }

	g_main_loop_run(mainloop);
	g_main_loop_unref(mainloop);

    if( (err=device_battery_unset_cb()) < 0){
        printf("unset cb return ");
        errp(err);
    }

	return 0;
}
