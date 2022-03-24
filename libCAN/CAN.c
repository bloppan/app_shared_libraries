/*
 * CAN.c
 *
 *  Created on: Dec 16, 2021
 *      Author: bernardo
 */

#include "CAN.h"

/**
 * @brief	Inicializa la interfaz CAN
 * @param	interface	Interfaz a configurar
 * @retval	Codigo de error
 */
error_type CAN_Initialize(struct can_message * can_data)
{
	error_type codeError = NO_ERROR;
	int todo_broadcast = 1;
	int ret = 0;
    char command[100];

    memset(command, 0, sizeof(command));
    sprintf(command, "/sbin/ifconfig can0 down");
    printf("command = %s \n", command);
    popen(command,"r");

    sleep(1);

    memset(command, 0, sizeof(command));
    sprintf(command, "ip link set can0 type can bitrate 500000");
    printf("command = %s \n", command);
    popen(command,"r");

    sleep(1);

    memset(command, 0, sizeof(command));
    sprintf(command, "/sbin/ifconfig can0 up");
    printf("command = %s \n", command);
    popen(command,"r");

    sleep(1);

	can_data->socket = socket(PF_CAN, SOCK_DGRAM, CAN_J1939);

	if(setsockopt(can_data->socket, SOL_SOCKET, SO_BROADCAST, &todo_broadcast, sizeof(todo_broadcast))){

		printf("[ERROR]\t\t[CAN]\t Opening socket \n");
		return APP_REPORT(CAN, OPENING_SOCKET);
	}

	return NO_ERROR;
}
/**
 * @brief	Configura la interfaz CAN
 * @param	can_data	Puntero a estructura de datos CAN
 * @retval	Codigo de error
 */
error_type CAN_Configure(struct can_message * can_data)
{

	can_data->addr.can_family = AF_CAN;
	can_data->addr.can_addr.j1939.name = J1939_NO_NAME;
	can_data->addr.can_addr.j1939.addr = 0x20;
	can_data->addr.can_addr.j1939.pgn = J1939_NO_PGN;
	can_data->addr.can_ifindex = if_nametoindex("can0");

	if(bind(can_data->socket, (struct sockaddr *)&can_data->addr, sizeof(can_data->addr))){

		return APP_REPORT(CAN, BIND_CALL);
	}

	return NO_ERROR;
}
/**
 * @brief	Envia una trama CAN
 * @param	can_data	Puntero a estructura de datos CAN
 * @retval	Codigo de error
 */
error_type CAN_Send(struct can_message * can_data)
{

	if (write(can_data->socket, &can_data->frame, sizeof(struct can_frame)) != sizeof(struct can_frame)) {

		perror("CAN Send: ");
		return APP_REPORT(CAN, SEND_FRAME);
	}

	return NO_ERROR;
}
/**
 * @brief	Envia un fichero por el bus CAN
 * @param	can_data	Puntero a estructura de datos CAN
 * @param	path		Ruta donde se encuentra el fichero
 * @retval	Codigo de error
 */
error_type CAN_SendFile(struct can_message * can_data, char * path)
{
	char json_array[2000];
	uint32_t n_bytes = 0, j = 0;

	FILE *file = fopen(path, "r");

	memset(json_array, 0, sizeof(json_array));

	while(!feof(file)){

		fscanf(file, "%c", &json_array[n_bytes++]);
	}

	fclose(file);


	if (write(can_data->socket, json_array, n_bytes) != n_bytes) {

		perror("[ERROR]\t[CAN]\t\tSending message: ");
		return APP_REPORT(CAN, SEND_FRAME);
	}else{
		printf("[OK]\t[CAN]\t\tMessage sent: %s \n", json_array);
	}

	return NO_ERROR;
}
/**
 * @brief	Recepcion de tramas CAN
 * @param	frame	Estructura que contiene la informacion de la trama CAN a recibir
 * @retval	Codigo de error
 */
error_type CAN_Receive(struct can_message * can_data)
{
	int nbytes, i;

	nbytes = read(can_data->socket, &can_data->frame, sizeof(struct can_frame));

 	if (nbytes < 0) {

		perror("CAN Read: ");
		return APP_REPORT(CAN, RECEIVE_FRAME);
	}

 	// Imprime la trama recibida
	printf("0x%03X [%d] ",can_data->frame.can_id, can_data->frame.can_dlc);

	for (i = 0; i < can_data->frame.can_dlc; i++){

		printf("%02X ", can_data->frame.data[i]);
	}

	printf("\r\n");

	return NO_ERROR;
}
