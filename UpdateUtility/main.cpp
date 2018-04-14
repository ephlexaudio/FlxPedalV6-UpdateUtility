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
#include "PedalUiInt.h"

GPIOClass usbDetect = GPIOClass(USB_CONNECTED);

bool isUsbConnected();
PedalUiInt pedalUi;
HostUiInt hostUi;
bool debugOutput = true;

int main(void)
{
	int osSelected = 0;
	//string osSelectedName = "test";
	usbDetect.export_gpio();
	usbDetect.setdir_gpio("in");
	int buttonPushed = -1;
	bool _exit = false;
	char tempString[20] = "RXed: ";
	vector<string> updateFileList;
	bool backdoorAccessRequested = false;
	char gadget[10] = "";
	long exitLoopCount = 0;
	bool sftpAccess = false;
	//bool update = false;
	if(pedalUi.isPowerButtonPushed() == true)
	//while(_exit == false)
	{
		usleep(100000);

		if((isUsbConnected() == true))
		{
			while(_exit == false)
			{

				FILE *gadgetDetect = popen("ls /sys/kernel/config/usb_gadget/","r");
				while((fgets(gadget,5,gadgetDetect)) != NULL)
				{
					//cout << gadget << endl;
				}
				pclose(gadgetDetect);

				if(gadget[0] != 'g')
				{
					while(osSelected == 0)
					{
						pedalUi.writeLcdLine(1,"Host PC Detected:");
						pedalUi.writeLcdLine(2," Select OS:     ");
						pedalUi.writeLcdLine(3,"                ");
						pedalUi.writeLcdLine(4," Win  Mac  Lin  ");

						while(buttonPushed == -1)  // keeps LCD from flickering
						{
							buttonPushed = pedalUi.readButtons();
						}
						if(buttonPushed >= 0)
						{
							usleep(10000); // debounce
							if(pedalUi.readButtons() == buttonPushed)
							{
								if(buttonPushed > 3) buttonPushed -= 2;
								else buttonPushed--;

								switch(buttonPushed)
								{
								case 0:		// Windows selected
									osSelected = 1;
									//osSelectedName = "Windows";
									system("/root/AcmRndisUsbInit");
									break;
								case 1:		// Mac OS X selected
									osSelected = 2;
									//osSelectedName = "Mac OS X";
									system("/root/AcmEcmUsbInit");
									break;
								case 2:		// Linux selected
									osSelected = 3;
									//osSelectedName = "Linux";
									system("/root/AcmEcmUsbInit");
									break;
								default:;
								}
							}
						}
					}

				}
				if(exitLoopCount++ == 0)
				{
					pedalUi.writeLcdLine(1,"Press left arrow " );
					pedalUi.writeLcdLine(2,"to update pedal");
					pedalUi.writeLcdLine(3,"Press right arrow " );
					pedalUi.writeLcdLine(4,"for SFTP access" );
				}


				while(pedalUi.isWindowsBackdoorAccessRequested() == false &&
						pedalUi.isMacLinuxBackdoorAccessRequested() == false);
				/********************** UPDATING SECTION ********************************/
				if(pedalUi.isWindowsBackdoorAccessRequested() == true)//if(pedalUi.isPowerButtonPushed() == true)
				//if((isUsbConnected() == true))
				{

					//while(pedalUi.isPowerButtonPushed() == true); // power button also shuts off pedal
																	//, so this should be a quick push/release

					pedalUi.writeLcdLine(1,"Ready for Update");
					pedalUi.writeLcdLine(2,"press left arrow");
					pedalUi.writeLcdLine(3,"to cancel update");
					/**************** Wait for folder to load *************/
					bool zipFileRecieved = false;
					char updateZipFileString[20] = "";
					char parseUpdateZipFileString[20] = "";
					int size = 0;
					int previousSize = 0;
					char *zipFileSizeStringPtr = NULL;
					char *zipFilePathNameStringPtr = NULL;

					char version[7] = "      ";
					while(pedalUi.isWindowsBackdoorAccessRequested() == true);
					while(zipFileRecieved == false)
					{
						sleep(1);
						FILE *getZipFileFD = popen("ls -s /home/Updates/","r");
						int passCount = 0;
						while(fgets(updateZipFileString, 100, getZipFileFD) != NULL)
						{
							if(pedalUi.isWindowsBackdoorAccessRequested() == true)
							/************** Cancel out of Update mode **************/
							{
								zipFileRecieved = true;
								size = 0;

								break;
							}
							else if(strncmp(updateZipFileString,"total",5) != 0)
							{
								strcpy(parseUpdateZipFileString, updateZipFileString);
								zipFileSizeStringPtr = strtok(parseUpdateZipFileString, " ");
								cout << zipFileSizeStringPtr << endl;
								zipFilePathNameStringPtr = strtok(NULL, "\0");
								cout << zipFilePathNameStringPtr << endl;
								strncpy(updateZipFileString, zipFilePathNameStringPtr, 14);
								size = atoi(zipFileSizeStringPtr);
								if(size == previousSize)
								{
									//char *tempChar = NULL;
									char *prefixNamePtr = NULL;
									char *suffixNamePtr = NULL;
									prefixNamePtr = strstr(zipFilePathNameStringPtr,"flx_");
									suffixNamePtr = strstr(zipFilePathNameStringPtr,".zip");
									if(prefixNamePtr == NULL || suffixNamePtr == NULL)
									{
										pedalUi.writeLcdLine(4,"invalid file.");
										system("rm -rf /home/Updates/*"); //wait for another file
									}
									else
									{
										pedalUi.writeLcdLine(1,"Getting Update");
										zipFileRecieved = true;
										char *dummyPtr = NULL;
										char *versionPtr1 = NULL;
										char *versionPtr2 = NULL;
										dummyPtr = strtok(updateZipFileString, "_");
										versionPtr1 = strtok(NULL,".");
										versionPtr2 = strtok(NULL,".");
										snprintf(version,5,"%s.%s",versionPtr1, versionPtr2);
										cout << "version: " << version << endl;
									}
									break;
								}
								previousSize = size;
								usleep(100000);
							}
						}
						pclose(getZipFileFD);

					}
					if(size > 0)
					{
						strncat(tempString,updateZipFileString,20);

						sleep(1);
						system("unzip /home/Updates/flx*.zip -d /home/Updates");

						system("touch /home/version");
						system("rm -rf /home/Updates/flx");
						system("rm /home/Updates/flx*.zip");
						system("chmod 751 /home/Updates/*");
						system("mv /home/Updates/* /home");
						char versionStringCommand[25] = "";

						sprintf(versionStringCommand,"echo %s > /home/version", version);
						system(versionStringCommand);
						_exit = true;
						pedalUi.writeLcdLine(1,"Update Done ");
						pedalUi.writeLcdLine(2,tempString);
						/*pedalUi.writeLcdLine(1,"            " );
						pedalUi.writeLcdLine(2,(char *)"Update Done" );
						pedalUi.writeLcdLine(3,"            " );
						pedalUi.writeLcdLine(4,"            " );*/
					}
					else
					{
						pedalUi.writeLcdLine(1, "Update canceled.");
						_exit = true;
					}


				}
				/*************************** PLAIN SFTP ACCESS SECTION **************************/
				else if((pedalUi.isMacLinuxBackdoorAccessRequested() == true) && sftpAccess == false)
				{
					sftpAccess = true;
					pedalUi.writeLcdLine(1,"SFTP Access Granted");
					pedalUi.writeLcdLine(2,"press right arrow");
					pedalUi.writeLcdLine(3,"to release access");

					_exit = false;
					while(pedalUi.isMacLinuxBackdoorAccessRequested() == true);
				}
				else if((pedalUi.isMacLinuxBackdoorAccessRequested() == true) && sftpAccess == true)
				{
					sftpAccess = false;
					pedalUi.writeLcdLine(1,"SFTP Access Released");

					_exit = true;
					while(pedalUi.isMacLinuxBackdoorAccessRequested() == true);
				}

			}
		}
		else
		{

			_exit = false;  //reset for next time USB is connected
			pedalUi.writeLcdLine(1,(char *)"Please connect");
			pedalUi.writeLcdLine(2,(char *)"USB Cable");
		}
	}

	//*if(backdoorAccessRequested == true)
	{
		while(1)
		{
			if(pedalUi.isPowerButtonPushed() == true)
			{
				usleep(10000);
				if(pedalUi.isPowerButtonPushed() == true)
				{
					break;
				}
			}
		}
		system("poweroff");
	}
	exit(0);
	return 0;
}

#define dbg 0
bool isUsbConnected()
{

	bool status = false;
	int usbLine = 0;
#if(dbg >= 1)
	cout << "********** ENTERING FlxPedalUi:main:isUsbConnected: " <<  endl;
#endif

	usbDetect.getval_gpio(usbLine);
	if(usbLine == 1) status = true;
	//if(pedalUi.readButtons() == 5) status = true;

#if(dbg >= 1)
	cout << "********** EXITING FlxPedalUi:main::isUsbConnected: " << status << endl;
#endif

	return status;
}
