/*
 * spiInterface.c
 *
 *  Created on: Jun 24, 2012
 *      Author: Sebastian Behrens
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

#ifdef __WIN32__
#include "ext_header/somedefs.h"
#include "ext_header/linux/getopt.h"
#include "ext_header/linux/types.h"
#include "ext_header/linux/spi/spidev.h"
#endif

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static const char *device = "/dev/spidev0.0";
static uint8_t mode;
static uint8_t bits = 8;
static uint32_t speed = 50000;
static uint16_t delay;

static void pabort(const char *s) {
	perror(s);
	abort();
}

static int transfer(int fd, unsigned char * tx_data, unsigned char * rx_data,
		unsigned char length) {
	int ret;

	struct spi_ioc_transfer tr = { .tx_buf = (unsigned int) tx_data, .rx_buf =
			(unsigned int) rx_data, .len = length, .delay_usecs = delay,
			.speed_hz = speed, .bits_per_word = bits, };

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
		pabort("can't send spi message");

	/*	for (ret = 0; ret < length; ret++) {
	 if (!(ret % 6))
	 puts("");
	 printf("%.2X ", rx_data[ret]);
	 }
	 puts("");
	 */
	return ret;

}

static void print_usage(const char *prog) {
	printf("Usage: %s [-DsbdlHOLC3]\n", prog);
	puts("  -D --device   device to use (default /dev/spidev0.0)\n"
			"  -s --speed    max speed (Hz)\n"
			"  -d --delay    delay (usec)\n"
			"  -b --bpw      bits per word \n"
			"  -l --loop     loopback\n"
			"  -H --cpha     clock phase\n"
			"  -O --cpol     clock polarity\n"
			"  -L --lsb      least significant bit first\n"
			"  -C --cs-high  chip select active high\n"
			"  -3 --3wire    SI/SO signals shared\n");
	exit(1);
}

// fucntions to set the various spi options and modes:

void set_device(char *dev) {
	device = dev;
	set_mode();
}

void set_speed(uint32_t s) {
	speed = s;
	set_mode();
}

void set_delay(uint16_t d) {
	delay = d;
	set_mode();
}

void set_bits_per_word(uint8_t bpw) {
	bits = bpw;
	set_mode();
}

void set_loop(int l) {
	if (l) {
		mode |= SPI_LOOP;
	} else {
		mode &= ~SPI_LOOP;
	}
	set_mode();
}

void set_clock_phase(int h) {
	if (h) {
		mode |= SPI_CPHA;
	} else {
		mode &= ~SPI_CPHA;
	}
	set_mode();

}
void set_clock_polarity(int o) {
	if (o) {
		mode |= SPI_CPOL;
	} else {
		mode &= ~SPI_CPOL;
	}
	set_mode();

}

void set_spi_mode(int spi_mode) {

	if (spi_mode == 0) {
		set_clock_phase(0);
		set_clock_polarity(0);
	} else if (spi_mode == 1) {
		set_clock_phase(0);
		set_clock_polarity(1);
	} else if (spi_mode == 2) {
		set_clock_phase(1);
		set_clock_polarity(0);
	} else if (spi_mode == 3) {
		set_clock_phase(1);
		set_clock_polarity(1);
	} else {
		pabort("spi mode must may be only one of 0,1,2 and 3");
	}
}
void set_lsb_first(int l) {
	if (l) {
		mode |= SPI_LSB_FIRST;
	} else {
		mode &= ~SPI_LSB_FIRST;
	}
	set_mode();
}

void set_cs_high(int c) {
	if (c) {
		mode |= SPI_CS_HIGH;
	} else {
		mode &= ~SPI_CS_HIGH;
	}
	set_mode();

}
void set_3wire(int i) {
	if (i) {
		mode |= SPI_3WIRE;
	} else {
		mode &= ~SPI_3WIRE;
	}
	set_mode();

}

//                case 'l':
//                        mode |= SPI_LOOP;
//                        break;
//                case 'H':
//                        mode |= SPI_CPHA;
//                        break;
//                case 'O':
//                        mode |= SPI_CPOL;
//                        break;
//                case 'L':
//                        mode |= SPI_LSB_FIRST;
//                        break;
//                case 'C':
//                        mode |= SPI_CS_HIGH;
//                        break;
//                case '3':
//                        mode |= SPI_3WIRE;
//                        break;
//                case 'N':
//                        mode |= SPI_NO_CS;
//                        break;
//                case 'R':
//                        mode |= SPI_READY;
//                        break;

//static uint8_t mode;
//static uint8_t bits = 8;
//static uint32_t speed = 5000;
//static uint16_t delay;

static void parse_opts(int argc, char *argv[]) {
	while (1) {
		static const struct option lopts[] = { { "device", 1, 0, 'D' }, {
				"speed", 1, 0, 's' }, { "delay", 1, 0, 'd' },
				{ "bpw", 1, 0, 'b' }, { "loop", 0, 0, 'l' },
				{ "cpha", 0, 0, 'H' }, { "cpol", 0, 0, 'O' },
				{ "lsb", 0, 0, 'L' }, { "cs-high", 0, 0, 'C' }, { "3wire", 0, 0,
						'3' }, { "no-cs", 0, 0, 'N' }, { "ready", 0, 0, 'R' }, {
						NULL, 0, 0, 0 }, };
		int c;

		c = getopt_long(argc, argv, "D:s:d:b:lHOLC3NR", lopts, NULL);

		if (c == -1)
			break;

		switch (c) {
		case 'D':
			device = optarg;
			break;
		case 's':
			speed = atoi(optarg);
			break;
		case 'd':
			delay = atoi(optarg);
			break;
		case 'b':
			bits = atoi(optarg);
			break;
		case 'l':
			mode |= SPI_LOOP;
			break;
		case 'H':
			mode |= SPI_CPHA;
			break;
		case 'O':
			mode |= SPI_CPOL;
			break;
		case 'L':
			mode |= SPI_LSB_FIRST;
			break;
		case 'C':
			mode |= SPI_CS_HIGH;
			break;
		case '3':
			mode |= SPI_3WIRE;
			break;
		case 'N':
			mode |= SPI_NO_CS;
			break;
		case 'R':
			mode |= SPI_READY;
			break;
		default:
			print_usage(argv[0]);
			break;
		}
	}
}

int set_mode() {

	int ret = 0;
	int fd;
	fd = open(device, O_RDWR);
	if (fd < 0)
		pabort("can't open device");

	/*
	 * spi mode
	 */
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		pabort("can't set spi mode");

	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
		pabort("can't get spi mode");

	/*
	 * bits per word
	 */
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't set bits per word");

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't get bits per word");

	/*
	 * max speed hz
	 */
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't set max speed hz");

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't get max speed hz");

	close(fd);
}

int tx_rx(unsigned char * tx_data, unsigned char * rx_data,
		unsigned char length) {
	int ret = 0;
	int fd;

	fd = open(device, O_RDWR);
	ret = transfer(fd, tx_data, rx_data, length);

	close(fd);

	return ret;
}

char bus_write(unsigned char dev_addr, unsigned char reg_addr,
		unsigned char * data, unsigned char data_length) {

	int i;
	/*	printf("Dev:     %d\n",dev_addr);
	 printf("Address: %.2X\n",reg_addr);
	 printf("Data:");
	 for (i=0;i<data_length;i++){
	 if(!(i % 8)){
	 printf("\n        ");
	 }
	 printf("%.2X ", data[i]);
	 }
	 puts("");
	 printf("Length:  %d\n",data_length);
	 */
	unsigned char tx_length = data_length * 2;
	//printf("%d\n",tx_length);
	unsigned char tx_data[tx_length];
	unsigned char rx_data[tx_length];

	int j = 0;
	for (i = 0; i < tx_length; i++) {

		tx_data[i] = reg_addr;
		reg_addr++;
		tx_data[i + 1] = data[j];
		j++;
		i++;

	}
	/*
	 printf("tx_Data:");
	 for (i=0;i<tx_length;i++){
	 if(!(i % 8)){
	 printf("\n        ");
	 }
	 printf("%.2X ", tx_data[i]);
	 }
	 puts("");

	 */
	tx_rx(tx_data, rx_data, tx_length);
	return 0x00;
}

char bus_read(unsigned char dev_addr, unsigned char reg_addr,
		unsigned char * data, unsigned char data_length) {

	int i;
	/*	printf("Dev:     %d\n",dev_addr);
	 printf("Address: %.2X\n",reg_addr);
	 printf("Data:");
	 for (i=0;i<data_length;i++){
	 if(!(i % 8)){
	 printf("\n        ");
	 }
	 printf("%.2X ", data[i]);
	 }
	 puts("");
	 printf("Length:  %d\n",data_length);
	 */
	unsigned char read_addr = (0x80 | reg_addr);
//	printf("%d\n",read_addr);
	unsigned char tx_data[data_length + 1];
	tx_data[0] = read_addr;
	for (i = 1; i <= data_length; i++) {
		tx_data[i] = 0x00;
	}
	unsigned char rx_data[data_length + 1];
	tx_rx(tx_data, rx_data, data_length + 1);
	for (i = 0; i < data_length; i++) {
		data[i] = rx_data[i + 1];
	}
	return 0x00;
}

char init_bus() {

//parse_opts(argc, argv);

	printf("spi mode: %d\n", mode);
	printf("bits per word: %d\n", bits);
	printf("max speed: %d Hz (%d KHz)\n", speed, speed / 1000);

	set_mode();
}

