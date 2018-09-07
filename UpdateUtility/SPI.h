/*
 * SharedMemoryInt.h
 *
 *  Created on: Mar 20, 2016
 *      Author: mike
 */

#ifndef SHAREDMEMORYINT_H_
#define SHAREDMEMORYINT_H_


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
#include "GPIOClass.h"


class SPI
{
public:
	SPI();
	~SPI();


private:
	char txBuffer[TX_BUFFER_SIZE];
	char rxBuffer[RX_BUFFER_SIZE];
protected:

public:

	uint8_t status;
	uint8_t sendData(char *data, unsigned int length);
	uint8_t getData(char *data, uint16_t length);

};



#endif /* SHAREDMEMORYINT_H_ */
