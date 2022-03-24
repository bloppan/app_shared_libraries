/*
 * CAN_export.h
 *
 *  Created on: Dec 16, 2021
 *      Author: bernardo
 */

#ifndef CAN_EXPORT_H_
#define CAN_EXPORT_H_

#include "CAN.h"

error_type (*CAN_Initialize)(struct can_message *);
error_type (*CAN_Configure)(struct can_message *);
error_type (*CAN_Send)(struct can_message *);
error_type (*CAN_SendFile)(struct can_message *, char *);
error_type (*CAN_Receive)(struct can_message *);


#endif /* CAN_EXPORT_H_ */
