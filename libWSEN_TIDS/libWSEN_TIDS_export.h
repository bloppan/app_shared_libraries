/*
 * libWSEN_TIDS_export.h
 *
 *  Created on: Jan 26, 2022
 *      Author: bernar
 */

#ifndef LIBWSEN_TIDS_EXPORT_H_
#define LIBWSEN_TIDS_EXPORT_H_

#include "libWSEN_TIDS.h"

error_type (*WSEN_TIDS_Initialize)(void);
error_type (*WSEN_TIDS_getTemperature)(float*);

#endif /* LIBWSEN_TIDS_EXPORT_H_ */

