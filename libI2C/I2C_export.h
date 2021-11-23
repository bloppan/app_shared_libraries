/*
 * i2c_export.h
 *
 *  Created on: Jun 30, 2021
 *      Author: bernardo
 */

#ifndef I2C_EXPORT_H_
#define I2C_EXPORT_H_

#include "I2C.h"

error_type (*read_i2c_data)(i2c_device_struct *i2c_dev);
error_type (*write_i2c_data)(i2c_device_struct *i2c_dev);

#endif /* I2C_EXPORT_H_ */
