/*
 * CAN.h
 *
 *  Created on: Dec 16, 2021
 *      Author: bernardo
 */

#ifndef CAN_H_
#define CAN_H_

#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include "../../app_includes/app_typedef.h"
#include "../../app_includes/app_errors.h"


error_type CAN_Initialize(char* interface, char* speed);
error_type CAN_Send(struct can_frame* frame);
error_type CAN_Receive(struct can_frame* frame);

#endif /* CAN_H_ */
