/*
 * Utilities.h
 *
 *  Created on: Jul 17, 2017
 *      Author: buildrooteclipse
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

#include <linux/types.h>
#include <signal.h>
#include <string>
#include <cstring>
#include <iostream>
#include <json/json.h>
#include <algorithm>
using namespace std;

void clearBuffer(char *buffer, int bufferSize);
void pabort(const char *s);
bool validateString(string &data);
#endif /* UTILITIES_H_ */
