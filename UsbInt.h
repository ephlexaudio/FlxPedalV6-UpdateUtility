/*
 * UsbInt.h
 *
 *  Created on: Nov 16, 2016
 *      Author: buildrooteclipse
 */

#ifndef USBINT_H_
#define USBINT_H_

#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <string>
#include <cstring>
#include <iostream>
#include <errno.h>
#include <signal.h>
#include "config.h"
#include "Utilities.h"
#include "ComputeModulePinOuts2.h"
#include "GPIOClass.h"
using namespace std;

class UsbInt
{
private:

	int osSelected = 3;
	char gadget[10] = "";
	int usbFD;
	bool connectionStatus;
	bool portOpenStatus;
	bool usbConfigStatus;
	GPIOClass usbDetect;
	void acmEcmUsbInit();
	void acmRndisUsbInit();
public:
	UsbInt();
	virtual ~UsbInt();
	bool isUsbCableConnected();
	int configureUsbGadgetForOs(int selectedOs);
	int openPort();
	bool isUsbConfigured();
	bool isPortOpen();
	int closePort();
	string readNewData(void);
	int writeData(string input);
};

#endif /* USBINT_H_ */
