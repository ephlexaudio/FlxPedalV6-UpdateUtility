/*
 * SharedMemoryInt.h
 *
 *  Created on: Mar 20, 2016
 *      Author: mike
 */

#ifndef SPI_H_
#define SPI_H_


#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <errno.h>
#include <iostream>
#include <fcntl.h>
#include <json/json.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <sys/stat.h>

#include "config.h"



class SPI
{
private:
	uint8_t status;
	const char *device0 = "/dev/spidev0.0";
	uint8_t mode = 0;
	uint8_t bits = 8;
	uint32_t speed = 100000;

	int spiFD;
public:
	SPI();
	~SPI();


	uint8_t sendData(char *data, int length);
	uint8_t getData(char *data, uint16_t length);

};



#endif /* SHAREDMEMORYINT_H_ */
