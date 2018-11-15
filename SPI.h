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

namespace std
{




class SPI
{
private:
	int status;
	const char *device0 = "/dev/spidev0.0";
	int mode = 0;
	int bits = 8;
	int speed = 100000;

	int spiFD;
public:
	SPI();
	~SPI();


	int sendData(char *data, int length);
	int getData(char *data, int length);

};

}

#endif /* SHAREDMEMORYINT_H_ */
