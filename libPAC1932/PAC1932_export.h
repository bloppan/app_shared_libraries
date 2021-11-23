/*
 * PAC1932_export.h
 *
 *  Created on: Jul 6, 2021
 *      Author: bernardo
 */

#ifndef PAC1932_EXPORT_H_
#define PAC1932_EXPORT_H_

#include "PAC1932.h"

error_type (*PAC1932_Initialize)(void);
error_type (*PAC1932_GetAllValues)(PAC1932_struct* PAC1932_Value);

#endif /* PAC1932_EXPORT_H_ */
