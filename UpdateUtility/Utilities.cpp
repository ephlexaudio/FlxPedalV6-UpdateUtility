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
	//abort();
	//goto exit;
}

