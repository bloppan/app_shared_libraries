/*
 * HTS221_export.h
 *
 *  Created on: Jul 1, 2021
 *      Author: bernardo
 */

#ifndef HTS221_EXPORT_H_
#define HTS221_EXPORT_H_

#include "HTS221.h"

error_type 	(*HTS221_Initialize)(void);
error_type	(*HTS221_getHumidity)(float *Hum);
error_type	(*HTS221_getTemperature)(float *Temp);

#endif /* HTS221_EXPORT_H_ */

