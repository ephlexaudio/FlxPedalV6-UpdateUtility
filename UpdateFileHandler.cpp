/*
 * UpdateFileHandler.cpp
 *
 *  Created on: Oct 23, 2018
 *      Author: buildrooteclipse
 */

#include "UpdateFileHandler.h"

namespace std
{

	UpdateFileHandler::UpdateFileHandler ()
	{
		this->size = 0;
		clearBuffer(this->version,7);
		clearBuffer(updateZipFileString,20);
	}

	UpdateFileHandler::~UpdateFileHandler ()
	{
		// TODO Auto-generated destructor stub
	}

	bool UpdateFileHandler::areFlxBinariesInHomeFolder()
	{
		char homeFolderFiles[200];
		vector<string> homeFolderFileVector;

		FILE *binaryCheckerFd = popen("ls /home/","r");

		while(fgets(homeFolderFiles,200,binaryCheckerFd ) != NULL)
		{
			char *fileName;

			fileName = strtok(homeFolderFiles,"\n");
			while(fileName != 0)
			{
				homeFolderFileVector.push_back(string(fileName));
				fileName = strtok(NULL,"\n");
			}
		}

		bool flxMainPresent = false;
		bool flxPedalUiPresent = false;
		for(auto & homeFileName : homeFolderFileVector)
		{
			if(homeFileName.compare("FlxMain") == 0) flxMainPresent = true;
			if(homeFileName.compare("FlxPedalUi") == 0) flxPedalUiPresent = true;
		}

		return flxMainPresent && flxPedalUiPresent;
	}

	int UpdateFileHandler::waitForFolderToLoad()
	{
		bool zipFileRecieved = false;
		int status = 0;
		char parseUpdateZipFileString[20] = "";
		int previousSize = 0;
		char *zipFileSizeStringPtr = NULL;
		char *zipFilePathNameStringPtr = NULL;


		while(zipFileRecieved == false)
		{
			sleep(1);
			FILE *getZipFileFD = popen("ls -s /home/Updates/","r");
			while(fgets(updateZipFileString, 100, getZipFileFD) != NULL)
			{
				if(strncmp(updateZipFileString,"total",5) != 0)
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
						char *prefixNamePtr = NULL;
						char *suffixNamePtr = NULL;
						prefixNamePtr = strstr(zipFilePathNameStringPtr,"flx_");
						suffixNamePtr = strstr(zipFilePathNameStringPtr,".zip");
						if(prefixNamePtr == NULL || suffixNamePtr == NULL)
						{
							system("rm -rf /home/Updates/*"); //wait for another file
							status = -1;
						}
						else
						{
							zipFileRecieved = true;
							char *dummyPtr = NULL;
							char *versionPtr1 = NULL;
							char *versionPtr2 = NULL;
							dummyPtr = strtok(updateZipFileString, "_");
							versionPtr1 = strtok(NULL,".");
							versionPtr2 = strtok(NULL,".");
							snprintf(this->version,5,"%s.%s",versionPtr1, versionPtr2);
							cout << "version: " << this->version << endl;
						}
						break;
					}
					previousSize = size;
					usleep(100000);
				}
			}
			pclose(getZipFileFD);

		}
		return status;
	}

	bool UpdateFileHandler::processUpdateFile()
	{

		char tempString[20] = "RXed: ";
		char versionStringCommand[30];
		clearBuffer(versionStringCommand,30);

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

			cout << "version: " << this->version << endl;
			snprintf(versionStringCommand, 27,"echo %s > /home/version", this->version);
			system(versionStringCommand);

			return true;
		}
		else
		{
			return false;
		}
	}



} /* namespace std */
