/*
 * UserInterface.h
 *
 *  Created on: Jul 12, 2017
 *      Author: buildrooteclipse
 */

#ifndef PEDALUIINT_H_
#define PEDALUIINT_H_

#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <signal.h>


#include <string>
#include <cstring>
#include <iostream>
#include "config.h"

#include "ComputeModulePinOuts2.h"
#include "GPIOClass.h"
#include "Utilities.h"
#include "SPI.h"

#define LCD_LINE1_ADDR 0x00
#define LCD_LINE2_ADDR 0x40
#define LCD_LINE3_ADDR 0x14
#define LCD_LINE4_ADDR 0x54
#define SET_DDRAM_ADDRESS 0x80

using namespace std;



class PedalUiInt {
private:
	GPIOClass initializePin(GPIOClass pin, int pinNumber, string direction);
	int rotEncReadCount;
	void printPinData(GPIOClass pin);


public:
	PedalUiInt();
	~PedalUiInt();

	GPIOClass menuButtons[7];
	GPIOClass rotEncOut[2];
	GPIOClass rotEncRst;
	GPIOClass footswitchIn[2];
	GPIOClass footswitchOut[2];
	GPIOClass powerButton;
	GPIOClass inputCoupling[3];
	SPI spi;

	int readButtons();
	bool isPowerButtonPushed();
	bool isUpdateRequested();
	bool isSFTPAccessRequested();
	void resetEncoder();
	int readEncoder();

	void writeLcdLine(int lineNumber, string lineString);
	void writeLcdLine(int lineNumber, char *lineString);
	void powerOff();
	void spiBitBangTest();
};

#endif /* PEDALUIINT_H_ */
