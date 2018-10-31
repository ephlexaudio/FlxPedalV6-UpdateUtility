/*
 * Utilities.cpp
 *
 *  Created on: Jul 17, 2017
 *      Author: buildrooteclipse
 */

#include "Utilities.h"

using namespace std;

void clearBuffer(char *buffer, int bufferSize)
{
	for(int i = 0; i < bufferSize; i++)
	{
		buffer[i] = 0;
	}
}

void pabort(const char *s)
{
	perror(s);
}

bool validateString(string &data)
{
	bool dataIsValid;
	if(data.size() >  15)
	{
		dataIsValid = true;
		data.erase(remove(data.begin(), data.end(), '\n'), data.end());
		(data).erase(remove(data.begin(), data.end(), '\r'), data.end());
		for(auto & c : data)
		{
			if((c < ' ' || '~' < c))
			{
				dataIsValid = false;
				break;
			}
		}
	}

	return dataIsValid;
}

