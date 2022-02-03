/*
 * CAN_export.h
 *
 *  Created on: Dec 16, 2021
 *      Author: bernardo
 */

#ifndef CAN_EXPORT_H_
#define CAN_EXPORT_H_

#include "CAN.h"

error_type (*CAN_Initialize)(char* interface, char* speed);
error_type (*CAN_Send)(struct can_frame* frame);
error_type (*CAN_Receive)(struct can_frame* frame);


#endif /* CAN_EXPORT_H_ */
