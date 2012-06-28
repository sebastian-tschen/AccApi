/*
 * spiInterface.h
 *
 *  Created on: Jun 24, 2012
 *      Author: Sebastian Behrens
 *
 *
 *
 *      Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in
 *      compliance with the License and the following stipulations. The Apache License , Version 2.0 is applicable unless
 *      otherwise stated by the stipulations of the disclaimer below.
 *
 *      You may obtain a copy of the License at
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 */


char bus_write(unsigned char, unsigned char, unsigned char *, unsigned char );


char bus_read(unsigned char, unsigned char, unsigned char *, unsigned char);

void set_device(char *);
void set_speed(uint32_t);
void set_delay(uint16_t);
void set_bits_per_word(uint8_t);
void set_loop(int);
void set_spi_mode(int);
void set_clock_phase(int);
void set_clock_polarity(int);
void set_lsb_first(int);
void set_cs_high(int);
void set_3wire(int);


char init_bus();

