/*
 * accSensorApi.h
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
 *
 */

#ifndef ACCSENSORAPI_H_
#define ACCSENSORAPI_H_

int acc_sensor_init();

//get the values. note: its more efficient to get all axis at once than to get each one seperately
smb380acc_t * get_all_axis();
short get_x_axis();
short get_y_axis();
short get_z_axis();

float get_temp();

//offset some values;

void set_temp(float t);

void set_x_axis_zero();
void set_y_axis_zero();
void set_z_axis_zero();

#endif /* ACCSENSORAPI_H_ */
