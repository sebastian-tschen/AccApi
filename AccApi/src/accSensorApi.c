/*
 * accSensorApi.c
 *
 *
 * Interface for the acceleration chip. provides basic
 * functions to read acceleration from all axis and temperature.
 * It will set most chip preferences by itself, but some are exposed to be set later.
 *
 * Created on: Jun 13, 2012
 *      Author: Sebastian Behrens
 *
 *
 *      Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in
 *      compliance with the License and the following stipulations. The Apache License , Version 2.0 is applicable unless
 *      otherwise stated by the stipulations of the disclaimer below.
 *
 *      You may obtain a copy of the License at
 *      http://www.apache.org/licenses/LICENSE-2.0
 */

#include "../header/smb380.h"
#include "../header/spiInterface.h"
#include <stdint.h>
#include <stdio.h>
#include <math.h>

short X_AXIS_OFFSET=0;
short Y_AXIS_OFFSET=0;
short Z_AXIS_OFFSET=0;
//init the whole thing
int acc_sensor_init() {

//setup the spi-bus
	set_device("/dev/spidev0.0");
	set_speed(50000);
	set_spi_mode(3);
	init_bus();

//init the api for the acc-sensor
	smb380_t myStruct;
	smb380_init(&myStruct);

//test connection by reading the chips serial number
	unsigned char chip_id_address = 0x00;
	unsigned char chip_id_value = 0x02;
	unsigned char size = 1;
	unsigned char r[size];
	smb380_read_reg(chip_id_address, r, size);
	if (r[0] != chip_id_value) {
		printf(
				"wrong chip id. expected %.2X but got %.2X. spi-connection brocken, wrong chip or chip not ready.",
				chip_id_value, r[0]);
		return 0;
	}
//set some sensitive default values for chip
	smb380_set_bandwidth(SMB380_BW_25HZ);
	smb380_set_range(SMB380_RANGE_2G);

	return 0;
}

//void main(int argc, char *argv[]){
void old_main() {
	set_device("/dev/spidev0.0");
	set_speed(30000);
	set_spi_mode(3);

	init_bus();

	smb380_t myStruct;
	smb380_init(&myStruct);

	unsigned char size = 32;

	unsigned char r[size];
	smb380_read_reg(0x00, r, size);
	int i;
	for (i = 0; i < size; i++) {
		if (!(i % 8)) {
			puts("");
		}
		printf("%.2X ", r[i]);
	}
	puts("");
	puts("setting bandwidth and range");
	smb380_set_bandwidth(SMB380_BW_25HZ);
	smb380_set_range(SMB380_RANGE_2G);
	puts("done");
	short x;
	short y;
	short z;
	unsigned char temp;

	while (1) {

		smb380_read_accel_x(&x);
		smb380_read_accel_y(&y);
		smb380_read_accel_z(&z);
		smb380_read_temperature(&temp);

		float t = -30 + (0.5 * temp);

		printf("\r");
		printf("%+.4d   %+.4d   %+.4d   %fC ", x, y, z, t);

	}


//smb380_write_reg(0x00,x,size);

	/*	unsigned char tx[] = {
	 0x80, 0xFF, 0x12, 0x91, 0x70, 0x60, 0xfd, 0x5b,
	 0x70, 0x9d, 0x10, 0x3e, 0x8f, 0x01, 0x5f, 0x4d,
	 0x7a, 0x4d
	 };
	 unsigned char length = sizeof(tx) / sizeof(tx[0]);
	 unsigned char dev='a';
	 unsigned char addr='b';
	 bus_read(dev,addr,tx,length);
	 */
}

smb380acc_t xyz;
smb380acc_t * get_all_axis() {
	smb380_read_accel_xyz(&xyz);
	xyz.x -= X_AXIS_OFFSET;
	xyz.y -= Y_AXIS_OFFSET;
	xyz.z -= Z_AXIS_OFFSET;
	return &xyz;
}
short get_z_axis(){
	short z;
	smb380_read_accel_z(&z);
	z -= Z_AXIS_OFFSET;
	return z;
}
short get_x_axis(){
	short x;
	smb380_read_accel_x(&x);
	x -= X_AXIS_OFFSET;
	return x;
}
short get_y_axis(){
	short y;
	smb380_read_accel_y(&y);
	y -= Y_AXIS_OFFSET;
	return y;
}
float get_raw_temp(){
	unsigned char temp_chip_value;
	smb380_read_temperature(&temp_chip_value);
	float t = -30 + (0.5 * temp_chip_value);
	return t;
}
float TEMP_OFFSET=0;
float get_temp(){
	return get_raw_temp()-TEMP_OFFSET;
}


void set_temp(float t){
	TEMP_OFFSET = get_raw_temp() - t;
}


void set_x_axis_zero(){
	X_AXIS_OFFSET=get_x_axis();
}
void set_y_axis_zero(){
	Y_AXIS_OFFSET=get_y_axis();
}
void set_z_axis_zero(){
	Z_AXIS_OFFSET=get_z_axis();
}

void set_x_axis_zero_to(short i){
        X_AXIS_OFFSET=get_x_axis()-i;
}
void set_y_axis_zero_to(short i){
        Y_AXIS_OFFSET=get_y_axis()-i;
}
void set_z_axis_zero_to(short i){
        Z_AXIS_OFFSET=get_z_axis()-i;
}




