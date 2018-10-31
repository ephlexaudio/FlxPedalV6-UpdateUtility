/*
 * main.cpp
 *
 *  Created on: Feb 7, 2018
 *      Author: buildrooteclipse
 */

#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string>
#include <time.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <signal.h>
#include <algorithm>

#include "HostUiInt.h"
#include "PedalUi.h"
#include "UpdateFileHandler.h"


bool debugOutput = true;

int main(void)
{
	PedalUi pedalUi;
	HostUiInt hostUi;
	UpdateFileHandler updateFileHandler;
	bool _exit = false;
	int osSelected = 0;
	bool usbConnected = false;
	bool userRequestingUpdate = false;

	bool flxBinariesPresent = updateFileHandler.areFlxBinariesInHomeFolder();
	if(flxBinariesPresent == false)
	{
		pedalUi.writeLcd("FLX files missing:","Re-install using ","FLX Editor       ","software update  ");
		sleep(3);
	}
	// Holding down the power button indicates the user WANTS to update the files
	if(pedalUi.isPowerButtonPushed() == true) userRequestingUpdate = true;
	while(userRequestingUpdate == true || flxBinariesPresent == false)
	{
		usleep(100000);

		//**************** Configure USB and open port ************************
		if(hostUi.isUsbCableConnected() == true && usbConnected == false)
		{
			if(hostUi.isUsbConfigured() == true)
			{
				if(hostUi.openPort() >= 0)
				{
					usbConnected = true;
					cout << "usbConnected: " << usbConnected << endl;
				}
				else
				{
					cout << "port open failed" << endl;
				}
			}
			else
			{
				if(osSelected == 0)
				{
					osSelected = pedalUi.selectHostOperatingSystem();
					hostUi.configureUsbGadgetForOs(osSelected);
				}
			}
		}
		else if(hostUi.isUsbCableConnected() == false)
		{
			_exit = false;  //reset for next time USB is connected
			pedalUi.writeLcd("Please connect","USB Cable","","");
			if(flxBinariesPresent == false)
			{
				sleep(3);
				pedalUi.writeLcd("FLX files missing:","Re-install using ","FLX Editor       ","software update  ");
				sleep(3);
			}
		}
		//********************** UPDATING SECTION ********************************

		if(usbConnected == true)
		{

			while(_exit == false)
			{
				pedalUi.writeLcd("FLX Update Menu","<: Update pedal","","");

				while(pedalUi.isUpdateRequested() == false);
				if(pedalUi.isUpdateRequested() == true)
				{
					pedalUi.writeLcd("Ready for Update","","","");

					hostUi.requestUpdateFromEditor();
					pedalUi.writeLcd("Recieving ","update file","","");
					updateFileHandler.waitForFolderToLoad();
					pedalUi.writeLcd("Processing ","update file","","");
					_exit = updateFileHandler.processUpdateFile();
					pedalUi.writeLcd("Files updated","","","");
					flxBinariesPresent = true;
					userRequestingUpdate = false;
				}
			}
		}
	}
	hostUi.closePort();
	system("/root/flxl");  // start updated software
	exit(0);
	return 0;
}
