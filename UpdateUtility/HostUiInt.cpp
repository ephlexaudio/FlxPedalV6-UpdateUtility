/*
 * HostUiInt.cpp
 *
 *  Created on: Sep 27, 2016
 *      Author: buildrooteclipse
 */

#include "config.h"
#include "HostUiInt.h"


#define FILE_SIZE 32000
#define SHARED_MEMORY_FILE_ADDRESS 32768
extern vector<string> componentVector;
extern vector<string> controlTypeVector;
extern bool debugOutput;

using namespace std;

HostUiInt::HostUiInt()
{
	// TODO Auto-generated constructor stub
	UsbInt usb = UsbInt();
	clearBuffer(this->hostUiRawRequestCharArray,FILE_SIZE);
	clearBuffer(this->hostUiRequestCharArray,FILE_SIZE);
	clearBuffer(this->hostUiResponseCharArray,FILE_SIZE);

}

HostUiInt::~HostUiInt() {
	// TODO Auto-generated destructor stub
}
#define dbg 1

int HostUiInt::connect()
{
	int status = 0;
#if(dbg >= 1)
	if(debugOutput) cout << "***** ENTERING: HostUiInt::connect" << endl;
#endif

	status = usb.connect();

#if(dbg >= 1)
	if(debugOutput) cout << "***** EXITING: HostUiInt::connect: " << status << endl;
#endif

	return status;
}

#define dbg 1
int HostUiInt::disconnect()
{
	int status = 0;
#if(dbg >= 1)
	if(debugOutput) cout << "***** ENTERING: HostUiInt::disconnect" << endl;
#endif

	status = usb.disconnect();

#if(dbg >= 1)
	if(debugOutput) cout << "***** EXITING: HostUiInt::disconnect:" << status << endl;
#endif

	return status;
}

#define dbg 0
int HostUiInt::open()
{
	int status = 0;
#if(dbg >= 1)
	if(debugOutput) cout << "***** ENTERING: HostUiInt::open" << endl;
#endif

	status = usb.isPortOpen();

#if(dbg >= 1)
	if(debugOutput) cout << "***** EXITING: HostUiInt::open:" << status << endl;
#endif

	return status;

}

#define dbg 0
bool HostUiInt::isPortOpen()
{
	bool status;
#if(dbg >= 1)
	if(debugOutput) cout << "***** ENTERING: HostUiInt::isPortOpen" << endl;
#endif

	status = usb.isPortOpen();

#if(dbg >= 1)
	if(debugOutput) cout << "***** EXITING: HostUiInt::isPortOpen: " << status << endl;
#endif

	return status;
}

#define dbg 0
int HostUiInt::close()
{
	int status = 0;
#if(dbg >= 1)
	if(debugOutput) cout << "***** ENTERING: HostUiInt::close" << endl;
#endif

	status = usb.closePort();

#if(dbg >= 1)
	if(debugOutput) cout << "***** EXITING: HostUiInt::close:" << status << endl;
#endif

	return status;
}

#define dbg 0
bool HostUiInt::isConnected()
{
	bool status = false;
#if(dbg >= 1)
	if(debugOutput) cout << "***** ENTERING: HostUiInt::isConnected" << endl;
#endif

	if(usb.isConnected()) status = true;

#if(dbg >= 1)
	if(debugOutput) cout << "***** EXITING: HostUiInt::isConnected:" << status << endl;
#endif

	return status;

}



#define dbg 0
int HostUiInt::checkForNewHostData(void)
{
#if(dbg >= 1)
	if(debugOutput) cout << "***** ENTERING: HostUiInt::checkForNewHostData" << endl;
#endif

	int status = usb.newData();

#if(dbg >= 1)
	if(debugOutput) cout << "***** EXITING: HostUiInt::checkForNewHostData: " << status << endl;
#endif
	return status;
}

#define dbg 0
string HostUiInt::getUserRequest(void)
{
#if(dbg >= 1)
	if(debugOutput) cout << "***** ENTERING: HostUiInt::getUserRequest" << endl;
#endif

#if(dbg >= 1)
	if(debugOutput) cout << "***** EXITING: HostUiInt::getUserRequest" << usb.readData() << endl;
#endif
	return string(usb.readData());
}


#define dbg 0
int HostUiInt::sendCurrentStatus(char *currentStatus)
{
	int status = 0;

#if(dbg >= 1)
	if(debugOutput) cout << "***** ENTERING: HostUiInt::sendCurrentStatus" << endl;
#endif

	clearBuffer(this->hostUiResponseCharArray,FILE_SIZE);
	sprintf(this->hostUiResponseCharArray,"<CurrentStatus>%s</CurrentStatus>",currentStatus);
	if(usb.writeData(this->hostUiResponseCharArray) >= 0)
	{
		status = 0;
	}
	else
	{
		status = -1;
	}


#if(dbg >= 1)
	if(debugOutput) cout << "***** EXITING: HostUiInt::sendCurrentStatus: " << status << endl;
#endif

	return status;
}


#define dbg 0
int HostUiInt::sendSimpleResponse(char *response)
{
	int status = 0;

#if(dbg >= 1)
	if(debugOutput) cout << "***** ENTERING: HostUiInt::sendSimpleResponse" << endl;
	if(debugOutput) cout << "response: " << response << endl;
#endif
	sprintf(this->hostUiResponseCharArray,"<SimpleResponse>%s</SimpleResponse>",response);
	if(usb.writeData(this->hostUiResponseCharArray) >= 0)
	{
		status = 0;
	}
	else
	{
		status = -1;
	}
	clearBuffer(this->hostUiResponseCharArray,FILE_SIZE);
#if(dbg >= 1)
	if(debugOutput) cout << "***** EXITING: HostUiInt::sendSimpleResponse: " << status << endl;
#endif


	return status;
}

void HostUiInt::clearAllInternalCharArrays(void)
{
	for(int i = 0; i < FILE_SIZE; i++)
	{
		this->hostUiRawRequestCharArray[i] = 0;
		this->hostUiRequestCharArray[i] = 0;
		this->hostUiResponseCharArray[i] = 0;
	}
}
