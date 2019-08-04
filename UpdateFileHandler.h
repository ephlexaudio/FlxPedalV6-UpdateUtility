/*
 * UpdateFileHandler.h
 *
 *  Created on: Oct 23, 2018
 *      Author: buildrooteclipse
 */

#ifndef UPDATEFILEHANDLER_H_
#define UPDATEFILEHANDLER_H_


#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <pthread.h>
#include <string>
#include <vector>
#include <algorithm>
#include "Utilities.h"



	class UpdateFileHandler
	{
	private:
		int size;
		char version[7];
		char updateZipFileString[20];

	public:
		UpdateFileHandler ();
		~UpdateFileHandler ();
		int waitForFolderToLoad();
		bool processUpdateFile();

		bool areFlxBinariesInHomeFolder();
	};



#endif /* UPDATEFILEHANDLER_H_ */
