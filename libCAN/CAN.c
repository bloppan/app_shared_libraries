/*
 * CAN.c
 *
 *  Created on: Dec 16, 2021
 *      Author: bernardo
 */

#include "CAN.h"

static int s;						// Socket
static struct sockaddr_can addr;	// Socket address
static struct ifreq ifr;			// Interface name

/**
 * @brief	Inicializa la interfaz CAN y configura el socket con el driver
 * @param	interface	Interfaz a configurar
 * @param	speed		Velocidad de transmision a configurar (en b/s)
 * @retval	Codigo de error
 */
error_type CAN_Initialize(char* interface, char* speed)
{

    FILE *fp;
    char command[100];

    /* command contains the command string (a character array) */
    memset(command, 0, sizeof(command));
    sprintf(command, "/sbin/ifconfig %s down", interface);
    printf("command = %s \n", command);
    fp = popen(command,"r");

    sleep(1);

    memset(command, 0, sizeof(command));
    sprintf(command, "ip link set can0 type can bitrate %s", speed);
    printf("command = %s \n", command);
    fp = popen(command,"r");

    sleep(1);

    memset(command, 0, sizeof(command));
    sprintf(command, "/sbin/ifconfig %s up", interface);
    printf("command = %s \n", command);
    fp = popen(command,"r");

    fclose(fp);

    sleep(1);

    // Abre un socket a la CAN Network
	if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {

		printf("[ERROR]\t[CAN]\t\tOpening socket: %s", strerror(errno));

		return APP_REPORT(CAN, OPENING_SOCKET);
	}

	strcpy(ifr.ifr_name, interface);

	// Se pasa al socket el nombre de la interfaz CAN
	if(ioctl(s, SIOCGIFINDEX, &ifr) < 0){

		printf("[ERROR]\t[CAN]\t\tioctl: %s \n", strerror(errno));

		return APP_REPORT(CAN, WRITING_IOCTL_FILE);
	}

	memset(&addr, 0, sizeof(addr));

	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	// Intenta conectarse a la interfaz CAN
	if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {

		printf("[ERROR]\t[CAN]\t\tbind: %s \n", strerror(errno));

		return APP_REPORT(CAN, BIND_CALL);
	}

	return NO_ERROR;
}

/**
 * @brief	Inicializa la interfaz CAN y configura el socket con el driver
 * @param	frame	Estructura que contiene la informacion de la trama CAN a enviar
 * @retval	Codigo de error
 */
error_type CAN_Send(struct can_frame* frame)
{

	// Envia a traves del socket la informacion de la trama CAN
	if (write(s, frame, sizeof(struct can_frame)) != sizeof(struct can_frame)) {

		printf("[ERROR]\t[CAN]\t\tSend frame: %s \n", strerror(errno));

		return APP_REPORT(CAN, SEND_FRAME);
	}

	return NO_ERROR;
}

/**
 * @brief	Inicializa la interfaz CAN y configura el socket con el driver
 * @param	frame	Estructura que contiene la informacion de la trama CAN a recibir
 * @retval	Codigo de error
 */
error_type CAN_Receive(struct can_frame* frame)
{
	int nbytes;

	// Lee a traves del socket y almacena la informacion CAN en la estructura frame
	nbytes = read(s, frame, sizeof(struct can_frame));

 	if (nbytes < 0) {

		printf("[ERROR]\t[CAN]\t\tReceive frame: %s \n", strerror(errno));

		return APP_REPORT(CAN, RECEIVE_FRAME);
	}

	return NO_ERROR;
}
