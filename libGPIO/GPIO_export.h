/*
 * GPIO_export.h
 *
 *  Created on: Jul 23, 2021
 *      Author: bernardo
 */

#ifndef GPIO_EXPORT_H_
#define GPIO_EXPORT_H_

#include "GPIO.h"

error_type (*configGPIO)(uint8_t gpio, char* direction);
error_type (*freeGPIO)(uint8_t gpio);
error_type (*getGPIO_Value)(uint8_t gpio, uint8_t* value);
error_type (*getGPIO_Direction)(uint8_t gpio, char* direction);
error_type (*setGPIO_Value)(uint8_t gpio, uint8_t value);


#endif /* GPIO_EXPORT_H_ */
