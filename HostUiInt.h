/*
 * HostUiInt.h
 *
 *  Created on: Sep 27, 2016
 *      Author: buildrooteclipse
 */

#ifndef HOSTUIINT_H_
#define HOSTUIINT_H_

#include "UsbInt.h"
#include <vector>




#define FILE_SIZE 32000
class HostUiInt : public UsbInt
{

public:

	HostUiInt();
	 ~HostUiInt();

	// Files are retrieved from the directory and sent straight to the host
	string getUserRequest(void);

	void requestUpdateFromEditor(void);
	int sendCurrentStatus(string currentStatus);
};

#endif /* HOSTUIINT_H_ */
