/*
 * i2c.c
 *
 *  Created on: Jun 30, 2021
 *      Author: bernardo
 */

#include "I2C.h"

#define DEBUG

static pthread_mutex_t i2c_mutex = PTHREAD_MUTEX_INITIALIZER;

/*
 * @brief	Funcion que lee datos a traves del puerto I2C
 * @param	i2c_dev		Puntero a la estructura de datos I2C
 * @return	errorCode	Codigo de error al ejecutar la funcion
 */
error_type read_i2c_data(i2c_device_struct *i2c_dev)
{
	struct i2c_rdwr_ioctl_data i2c_packet;
	struct i2c_msg message[2];			// Una estructura para el mensaje de escritura y otra para el mensaje de lectura
	uint32_t i2c_file;					// File descriptor al puerto I2C
	uint8_t outbuf[1];
	error_type errorCode = NO_ERROR;

	// Bloqueo mutex
	pthread_mutex_lock (&i2c_mutex);

	i2c_file = open(i2c_dev->i2c_port, O_RDWR);

	if(i2c_file > 0){

		/*
		 * Antes de leer un registro, se debe indicar mediante una operacion de escritura
		 * el registro que se quiere leer:
		 */

		outbuf[0] = i2c_dev->reg_addr;

		message[0].addr 	= i2c_dev->dev_addr;
		message[0].flags 	= 0; 					// 0 -> Write
		message[0].len 		= 1;					// 1 byte
		message[0].buf		= outbuf;

		/* Ahora se configura la estructura de lectura: */

		message[1].addr		= i2c_dev->dev_addr;
		message[1].flags	= 1;					// 1 -> Read
		message[1].len		= i2c_dev->length;		// N bytes que se van a leer a partir de la direccion reg_addr
		message[1].buf		= i2c_dev->data; 		// Buffer de recepcion

		/* Send the request to the kernel and get the result back */

		i2c_packet.msgs		= message;
		i2c_packet.nmsgs	= 2;

		if(ioctl(i2c_file, I2C_RDWR, &i2c_packet) < 0){

#ifdef DEBUG
			printf("[ERROR]\t[I2C]\t\tioctl: %s\n", strerror(errno));
#endif
			errorCode = APP_REPORT(I2C, WRITING_IOCTL_FILE);
		}

	}else{

#ifdef DEBUG
		printf("[ERROR]\t[I2C]\t\tOpen I2C port file: %s \n", strerror(errno));
#endif
		errorCode = APP_REPORT(I2C, OPENING_I2C_PORT);
	}

	close(i2c_file);

	// Desbloqueo mutex
	pthread_mutex_unlock (&i2c_mutex);

	return errorCode;
}
/*
 * @brief	Funcion que escribe datos a traves del puerto I2C
 * @param	i2c_dev		Puntero a la estructura de datos I2C
 * @return	errorCode	Codigo de error al ejecutar la funcion
 */
error_type write_i2c_data(i2c_device_struct *i2c_dev)
{
	struct i2c_rdwr_ioctl_data i2c_packet;
	struct i2c_msg message[1];			// Una estructura para el mensaje de escritura
	uint32_t i2c_file;					// File descriptor al puerto I2C
	uint8_t outbuf[128] = {0};
	error_type errorCode = NO_ERROR;

	// Bloqueo mutex
	pthread_mutex_lock (&i2c_mutex);

	i2c_file = open(i2c_dev->i2c_port, O_RDWR);

	if(i2c_file > 0){

		/*
		 * Antes de leer un registro, se debe indicar mediante una operacion de escritura
		 * el registro que se quiere leer:
		 */
		memset(outbuf, 0, sizeof(outbuf));

		outbuf[0] = i2c_dev->reg_addr;

		for(int i = 0; i < i2c_dev->length; i++){

			outbuf[i + 1] = i2c_dev->data[i];
		}

		message[0].addr 	= i2c_dev->dev_addr;
		message[0].flags 	= 0; 			// 0 -> Write
		message[0].len 		= i2c_dev->length + 1; 	// N bytes de escritura
		message[0].buf		= outbuf;		// Registro a partir del cual se va a escribir

		/*
		 * Send the request to the kernel and get the result back
		 */
		i2c_packet.msgs		= message;
		i2c_packet.nmsgs	= 1;

		if(ioctl(i2c_file, I2C_RDWR, &i2c_packet) < 0){

#ifdef DEBUG
			printf("[ERROR]\t[I2C]\t\tioctl: %s\n", strerror(errno));
#endif
			errorCode = APP_REPORT(I2C, WRITING_IOCTL_FILE);
		}

	}else{

#ifdef DEBUG
		printf("[ERROR]\t[I2C]\t\tOpen I2C port file: %s \n", strerror(errno));
#endif
		errorCode = APP_REPORT(I2C, OPENING_I2C_PORT);
	}

	close(i2c_file);

	// Desbloqueo mutex
	pthread_mutex_unlock (&i2c_mutex);

	return errorCode;
}




