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
#include "../header/accSensorErrors.h"
#include <stdint.h>
#include <stdio.h>
#include <math.h>

int status=0;
short X_AXIS_OFFSET=0;
short Y_AXIS_OFFSET=0;
short Z_AXIS_OFFSET=0;
//init the whole thing
int acc_sensor_init(int speed, char * device) {

//setup the spi-bus
	set_device(device);
	set_speed(speed);
//	set_device("/dev/spidev0.0");
//	set_speed(500000);
	set_spi_mode(3);
	init_bus();

//init the api for the acc-sensor
	smb380_t myStruct;
	status = smb380_init(&myStruct);
	if (status){
		return SPI_BUS_READ_ERROR;
	}

//test connection by reading the chips serial number
	unsigned char chip_id_address = 0x00;
	unsigned char chip_id_value = 0x02;
	unsigned char size = 1;
	unsigned char r[size];
	status = smb380_read_reg(chip_id_address, r, size);
	
        if (status){
                return SPI_BUS_READ_ERROR;
        }

	if (r[0] != chip_id_value) {
		return WRONG_SENSOR_ID_ERROR;
	}
//set some sensitive default values for chip
	status = smb380_set_bandwidth(SMB380_BW_25HZ);
        if (status){
                return SPI_BUS_READ_ERROR;
        }

	status = smb380_set_range(SMB380_RANGE_2G);
        if (status){
                return SPI_BUS_READ_ERROR;
        }


	return 0;
}

smb380acc_t xyz;
smb380acc_t * get_all_axis() {
	status = smb380_read_accel_xyz(&xyz);
	xyz.x -= X_AXIS_OFFSET;
	xyz.y -= Y_AXIS_OFFSET;
	xyz.z -= Z_AXIS_OFFSET;
	return &xyz;
}
short get_z_axis(){
	short z;
	status = smb380_read_accel_z(&z);
	z -= Z_AXIS_OFFSET;
	return z;
}
short get_x_axis(){
	short x;
	status = smb380_read_accel_x(&x);
	x -= X_AXIS_OFFSET;
	return x;
}
short get_y_axis(){
	short y;
	status = smb380_read_accel_y(&y);
	y -= Y_AXIS_OFFSET;
	return y;
}
float get_raw_temp(){
	unsigned char temp_chip_value;
	status = smb380_read_temperature(&temp_chip_value);
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



int acc_sensor_get_status(){
return status;
}

int acc_sensor_check_id(){
	int status;
 	unsigned char chip_id_address = 0x00;
        unsigned char chip_id_value = 0x02;
        unsigned char size = 1;
        unsigned char r[size];
        status = smb380_read_reg(chip_id_address, r, size);

        if (status){
                return SPI_BUS_READ_ERROR;
        }

        if (r[0] != chip_id_value) {
                return WRONG_SENSOR_ID_ERROR;
        }
	return 0;
}

