/*
 * HostUiInt.cpp
 *
 *  Created on: Sep 27, 2016
 *      Author: buildrooteclipse
 */

#include "config.h"
#include "HostUiInt.h"


#define FILE_SIZE 32000




namespace std
{



HostUiInt::HostUiInt()
{


}

HostUiInt::~HostUiInt() {
	// TODO Auto-generated destructor stub
}

#define dbg 0
string HostUiInt::getUserRequest(void)
{
	string hostData;
#if(dbg >= 1)
	cout << "***** ENTERING: HostUiInt::getUserRequest" << endl;
#endif
	hostData = UsbInt::readNewData();
#if(dbg >= 1)
	cout << "***** EXITING: HostUiInt::getUserRequest: " << hostData << endl;
#endif
	return hostData;
}

void HostUiInt::requestUpdateFromEditor(void)
{
	bool hostUiRequestIsValid = false;
	string  hostUiRequest;
	while(hostUiRequestIsValid == false)
	{
        hostUiRequest = this->getUserRequest();
        if(hostUiRequest.empty() == false)
        {
			 cout << "HOST UI REQUEST: " << hostUiRequest << endl;
			 cout << "hostUiRequest size: " << hostUiRequest.size() << endl;

			/************ Validate and clean-up user request first ******************/
			hostUiRequestIsValid = validateString(hostUiRequest);
        }
	}
    cout << " clean hostUiRequest: " << hostUiRequest << endl;
    if(hostUiRequest.compare("getCurrentStatus") == 0)
    {
    	this->sendCurrentStatus("ready for update");
    }
}

#define dbg 0
int HostUiInt::sendCurrentStatus(string currentStatus)
{
	int status = 0;

#if(dbg >= 1)
	cout << "***** ENTERING: HostUiInt::sendCurrentStatus" << endl;
#endif
	string hostUiResponseString;
	hostUiResponseString = "<CurrentStatus>" + currentStatus + "</CurrentStatus>";
	if(UsbInt::writeData(hostUiResponseString.c_str()) >= 0)
	{
		status = 0;
	}
	else
	{
		status = -1;
	}

#if(dbg >= 1)
	cout << "***** EXITING: HostUiInt::sendCurrentStatus: " << status << endl;
#endif

	return status;
}
}
