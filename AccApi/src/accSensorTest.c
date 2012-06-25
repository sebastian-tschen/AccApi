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

main() {

	acc_sensor_init();

	smb380acc_t* xzy;

	float temp;

	set_temp(20.0);
	set_x_axis_zero();
	while (1) {

		xzy = get_all_axis();
		temp = get_temp();
		float t = -30 + (0.5 * temp);

		printf("\r");
		printf("%+.4d   %+.4d   %+.4d   %fC ", xzy->x, xzy->y, xzy->z, t);

	}

}
