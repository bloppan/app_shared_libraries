/*
 * PCA9532_export.h
 *
 *  Created on: Aug 2, 2021
 *      Author: bernardo
 */

#ifndef PCA9532_EXPORT_H_
#define PCA9532_EXPORT_H_

#include "PCA9532.h"

error_type (*setLED_Value)(uint8_t LEDn, uint8_t color);
error_type (*PCA9532_Initialize)(void);

#endif /* PCA9532_EXPORT_H_ */

