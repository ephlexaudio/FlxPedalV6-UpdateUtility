/*
 * PedalUi.cpp
 *
 *  Created on: Jul 12, 2017
 *      Author: buildrooteclipse
 */

#include "PedalUi.h"

#include "config.h"


using namespace std;



PedalUi::PedalUi() {
	// TODO Auto-generated constructor stub


	this->menuButtons[0] = this->initializePin(this->menuButtons[0],BUTTON0,"in");
	this->menuButtons[1] = this->initializePin(this->menuButtons[1],BUTTON1,"in");
	this->menuButtons[2] = this->initializePin(this->menuButtons[2],BUTTON2,"in");
	this->menuButtons[3] = this->initializePin(this->menuButtons[3],BUTTON3,"in");
	this->menuButtons[4] = this->initializePin(this->menuButtons[4],BUTTON4,"in");
	this->menuButtons[5] = this->initializePin(this->menuButtons[5],BUTTON5,"in");
	this->menuButtons[6] = this->initializePin(this->menuButtons[6],BUTTON6,"in");
	this->rotEncOut[0] = this->initializePin(this->rotEncOut[0],ROTENC_OUTA,"in");
	this->rotEncOut[1] = this->initializePin(this->rotEncOut[1],ROTENC_OUTB,"in");
	this->rotEncRst = this->initializePin(this->rotEncRst,RST_ROTENC_FFS,"out");
	this->rotEncReadCount = 0;
	this->powerButton = this->initializePin(this->powerButton,POWER_BUTTON,"in");

}

PedalUi::~PedalUi()
{
	// TODO Auto-generated destructor stub
}


#define dbg 0
GPIOClass PedalUi::initializePin(GPIOClass pin, int pinNumber, string direction)
{
#if(dbg >= 1)
	cout << "********** ENTERING PedalUi::initializePin: " << pinNumber << ": " << direction << endl;
#endif
	int status = 0;

	pin = GPIOClass(pinNumber);

	char dirCharArray[4];
	strncpy(dirCharArray, direction.c_str(),4);
	if(pin.export_gpio() == -1) status = -1;
	if(pin.setdir_gpio(dirCharArray) == -1) status = -1;

#if(dbg >= 1)
	cout << "********** EXITING PedalUi::initializePin: " << status << endl;
#endif
	if(status == -1) pin = 0;
	return pin;
}



void PedalUi::printPinData(GPIOClass pin)
{
#if(dbg >= 2)
	cout << "Pin number: " << pin.get_gpionum() << endl;
#endif
}

#define dbg 0
int PedalUi::readButtons()
{
#if(dbg >= 1)
	cout << "********** ENTERING PedalUi::readButtons: " << endl;
#endif
	int buttonPushed = -1;

	for(int buttonIndex = 0; buttonIndex < 7; buttonIndex++)
	{
		int buttonValue;
		if(this->menuButtons[buttonIndex].getval_gpio(buttonValue) >= 0)
		{
			if(buttonValue == 1)
			{
				buttonPushed =  6 - buttonIndex; // buttons are backwards
			}
		}
		else
		{
#if(dbg >= 2)
			cout << "error reading button " << buttonIndex << endl;
#endif
		}
	}

#if(dbg >= 1)
	cout << "********** EXITING PedalUi::readButtons: " << endl;
#endif

	return buttonPushed;
}


bool PedalUi::isPowerButtonPushed()
{
	bool status = false;
	int buttonValue = 0;

	this->powerButton.getval_gpio(buttonValue);
	if(buttonValue == 0) status = true;

	return status;
}

bool PedalUi::isUpdateRequested()
{
	bool status = false;
	int buttonValue = 0;

	this->menuButtons[6].getval_gpio(buttonValue);
	if(buttonValue == 1) status = true;

	return status;
}


void PedalUi::resetEncoder()
{
	this->rotEncRst.setval_gpio(0);
	this->rotEncRst.setval_gpio(1);
}

#define dbg 0
int PedalUi::readEncoder()
{
#if(dbg >= 1)
	cout << "********** ENTERING PedalUi::readEncoder: " << endl;
#endif


	int result = 0;
	int rotEncA = 0;
	int rotEncB = 0;
	this->rotEncOut[0].getval_gpio(rotEncA);
	this->rotEncOut[1].getval_gpio(rotEncB);
	result = -1*rotEncA + rotEncB;
#if(dbg >= 1)
	cout << "********** EXITING PedalUi::readEncoder: " << rotEncA << ":" << rotEncB << "->" << result << endl;
#endif

	return result;
}

#define dbg 0
void PedalUi::writeLcdLine(int lineNumber, string lineString)
{
#if(dbg >= 1)
	cout << "********** ENTERING PedalUi::writeLcdLine: " << endl;
#endif

	char sendCharArray[50];
	char lineCharArray[50];

	clearBuffer(sendCharArray,50);
	clearBuffer(lineCharArray,50);

	strncpy(lineCharArray,lineString.c_str(),20);
	sendCharArray[0] = (char)lineNumber;
	strncat(sendCharArray, lineCharArray, 20);
	for(int i = strlen(sendCharArray); i < 21; i++)
	{
		strcat(sendCharArray," ");
	}
	this->spi.sendData(sendCharArray,22);
	usleep(LCD_LINE_WRITE_SPACING);

#if(dbg >= 1)
	cout << "********** EXITING PedalUi::writeLcdLine: " << strlen(sendCharArray) << endl;
#endif
#if(dbg >= 2)
	cout << "********** " << sendCharArray << endl;
#endif
}

void PedalUi::writeLcd(string line1, string line2, string line3, string line4)
{
	this->writeLcdLine(1,line1);
	this->writeLcdLine(2,line2);
	this->writeLcdLine(3,line3);
	this->writeLcdLine(4,line4);
}

#define dbg 1
int PedalUi::selectHostOperatingSystem()
{
	int osSelected = 0;
#if(dbg >= 1)
	cout << "********** ENTERING PedalUi::selectHostOperatingSystem: " << endl;
#endif



	while(osSelected == 0)
	{
		this->writeLcdLine(1,"Host PC Detected:");
		this->writeLcdLine(2," Select OS:     ");
		this->writeLcdLine(3,"                ");
		this->writeLcdLine(4," Win  Mac  Lin  ");

		while(this->buttonPushed == -1)  // keeps LCD from flickering
		{
			this->buttonPushed = this->readButtons();
		}

		usleep(10000); // debounce
		if(this->readButtons() == this->buttonPushed)
		{
			if(this->buttonPushed > 3) this->buttonPushed -= 2;
			else this->buttonPushed--;

			switch(this->buttonPushed)
			{
			case 0:		// Windows selected
				osSelected = 1;
				break;
			case 1:		// Mac OS X selected
				osSelected = 2;
				break;
			case 2:		// Linux selected
				osSelected = 3;
				break;
			default:;
			}
		}
	}
#if(dbg >= 1)
	cout << "********** EXITING PedalUi::selectHostOperatingSystem: " << endl;
#endif

	return osSelected;
}

#define dbg 0
void PedalUi::powerOff()
{
#if(dbg >= 1)
	cout << "********** ENTERING PedalUi::powerOff: " << endl;
#endif

	system("poweroff");
#if(dbg >= 1)
	cout << "********** EXITING PedalUi::powerOff: " << endl;
#endif

}

