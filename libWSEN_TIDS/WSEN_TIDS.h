/*
 * libWSEN_TIDS.h
 *
 *  Created on: Jan 26, 2022
 *      Author: bernar
 */

#ifndef LIBWSEN_TIDS_H_
#define LIBWSEN_TIDS_H_


#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>

#include "../libI2C/I2C_export.h"

#include "../../app_includes/app_errors.h"
#include "../../app_includes/app_typedef.h"


#define	WSEN_ADDR		0x3F		// Sensor I2C address

#define	DEVICE_ID		0x01		// Device ID Register
#define	T_H_LIMIT		0x02		// Temperature limit register H
#define	T_L_LIMIT		0x03		// Temperature limit register L
#define WSEN_CTRL		0x04		// Control register
#define STATUS			0x05		// Status register
#define DATA_T_L		0x06		// Temperature output register L
#define	DATA_T_H		0x07		// Temperature output register H
#define	SOFT_RESET		0x0C		// Software reset register


#endif /* LIBWSEN_TIDS_H_ */

