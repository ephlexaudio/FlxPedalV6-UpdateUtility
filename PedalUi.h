/*
 * UserInterface.h
 *
 *  Created on: Jul 12, 2017
 *      Author: buildrooteclipse
 */

#ifndef PEDALUI_H_
#define PEDALUI_H_

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

namespace std
{





class PedalUi {
private:
	GPIOClass initializePin(GPIOClass pin, int pinNumber, string direction);
	int rotEncReadCount;
	void printPinData(GPIOClass pin);
	int buttonPushed = -1;

	GPIOClass menuButtons[7];
	GPIOClass rotEncOut[2];
	GPIOClass rotEncRst;
	GPIOClass powerButton;
	SPI spi;


public:
	PedalUi();
	~PedalUi();

	int readButtons();
	bool isPowerButtonPushed();
	bool isUpdateRequested();
	void resetEncoder();
	int readEncoder();

	void writeLcdLine(int lineNumber, string lineString);
	void writeLcd(string line1, string line2, string line3, string line4);
	int selectHostOperatingSystem();
	void powerOff();

};
}
#endif /* PEDALUI_H_ */
