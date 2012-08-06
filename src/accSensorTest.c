/*
 * accSensorTest.c
 *
 *  Created on: Jun 24, 2012
 *      Author: Sebastian Behrens
 *
 *      Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in
 *      compliance with the License and the following stipulations. The Apache License , Version 2.0 is applicable unless
 *      otherwise stated by the stipulations of the disclaimer below.
 *
 *      You may obtain a copy of the License at
 *      http://www.apache.org/licenses/LICENSE-2.0
 */

#include "../header/smb380.h"
#include "../header/accSensorApi.h"
#include "../header/accSensorErrors.h"
#include <stdio.h>

int status;
int check_status(){

status= acc_sensor_get_status();
if (status){
	puts("error reading from sensor");

}
return status;
}

int main() {
	int status=0;
	char *device="/dev/spidev0.0";
	int speed = 500000;
	status=acc_sensor_init(speed,device);
	if(status){
		puts("error initiallizing sensor");
		return status;
	}
		

	smb380acc_t *xyz;
	float temp;
	set_temp(25.0);
	if(check_status()){
		return status;
	}
	set_x_axis_zero();
        if(check_status()){
                return status;
        }

	set_y_axis_zero();
        if(check_status()){
                return status;
        }

	set_z_axis_zero_to(255);
        if(check_status()){
                return status;
        }


	int counter=0;
	while (1) {
	counter++;
	if (counter % 1024==0){
		if(acc_sensor_check_id()){
			return 1;
		}
	}
//	puts("blubb1");
		xyz = get_all_axis();
        if(check_status()){
                return status;
        }

//		printf("%d",sizeof(xyz));
//	puts("blubb");
		temp = get_temp();
//		short x=get_x_axis();
//		short y=get_y_axis();
//		short z=get_z_axis();
//	printf("\r  %d    ",x);
//	puts("blubb");
//		float t = -30 + (0.5 * temp);
//	puts("blubb");

//		printf("\r");
		printf("\r%+.4d   %+.4d   %+.4d   %fC ", xyz->x,  xyz->y,  xyz->z, temp);
//		puts("bab");
//		printf("\r%fC ", temp);
//printf("n");
//		printf("%+.4d   %+.4d   %+.4d", x, y, z);
//	puts("blubb3");

	}

}
