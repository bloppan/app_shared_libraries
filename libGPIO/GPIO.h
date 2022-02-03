/*
 * GPIO.h
 *
 *  Created on: Jul 23, 2021
 *      Author: bernardo
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>
#include <pthread.h>

#include "../../app_includes/app_typedef.h"
#include "../../app_includes/app_errors.h"

#define		GPIO_LIBRARY_DIRECTORY		"/usr/lib/"
#define		GPIO_LIBRARY_NAME			"libGPIO.so.1"



#endif /* GPIO_H_ */
