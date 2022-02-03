/*
 * libWSEN_TIDS.c
 *
 *  Created on: Jan 26, 2022
 *      Author: bernar
 */

#include "libWSEN_TIDS.h"

static i2c_device_struct WSEN_i2c_info = {0};	// Estructura de parametros I2C del sensor WSEN-TIDS

/*
 * @brief	Funcion que inicializa el sensor de temperatura WSEN_TIDS
 * @retval	Codigo de error
 */
error_type WSEN_TIDS_Initialize(void)
{

	void *libHandlerI2C = NULL;
	char route[256] = {0};

	// Carga la libreria I2C
	strcat(route, I2C_LIBRARY_DIRECTORY);
	strcat(route, I2C_LIBRARY_NAME);

	libHandlerI2C = dlopen(route, RTLD_LAZY);

	if(libHandlerI2C == NULL) {

#ifdef DEBUG
		printf("[ERROR]\t[WSEN-TIDS]\tLoading I2C library: %s \n", strerror(errno));
#endif

		return APP_REPORT(WSEN_TIDS, LOADING_I2C_LIBRARY);

	}else {

#ifdef DEBUG
		printf("[OK]\t[WSEN-TIDS]\tI2C library loaded successfully \n");
#endif
		// Carga de funciones de la libreria I2C:
		read_i2c_data = (error_type ( *)(i2c_device_struct *)) dlsym(libHandlerI2C, "read_i2c_data");
		write_i2c_data = (error_type ( *)(i2c_device_struct *)) dlsym(libHandlerI2C, "write_i2c_data");

		/*	Inicializacion de la estructura de datos I2C del sensor HTS221	*/

		uint8_t buffer[1];

		WSEN_i2c_info.i2c_port 		= I2C_0;
		WSEN_i2c_info.dev_addr		= WSEN_ADDR;
		WSEN_i2c_info.data			= buffer;
		WSEN_i2c_info.length		= 1;
		WSEN_i2c_info.reg_addr 		= SOFT_RESET;

		buffer[0] = 0x2;

		if(write_i2c_data(&WSEN_i2c_info) != NO_ERROR){

#ifdef DEBUG
		printf("[ERROR]\t[WSEN-TIDS]\tEnable software reset: %s \n", strerror(errno));
#endif

		return APP_REPORT(WSEN_TIDS, SEND_RESET_COMMAND);
		}

		sleep(0.3);

		buffer[0] = 0x0;

		if(write_i2c_data(&WSEN_i2c_info) != NO_ERROR){

#ifdef DEBUG
		printf("[ERROR]\t[WSEN-TIDS]\tDisable software reset: %s \n", strerror(errno));
#endif

		return APP_REPORT(WSEN_TIDS, SEND_RESET_COMMAND);

		}

		sleep(0.3);

		WSEN_i2c_info.reg_addr 		= WSEN_CTRL;
		buffer[0] = 0x5C;

		if(write_i2c_data(&WSEN_i2c_info) != NO_ERROR){

#ifdef DEBUG
		printf("[ERROR]\t[WSEN-TIDS]\tSend control command: %s \n", strerror(errno));
#endif

		return APP_REPORT(WSEN_TIDS, SEND_CTRL_COMMAND);
		}
	}

#ifdef DEBUG
		printf("[OK]\t[WSEN-TIDS]\tSensor Initialized \n");
#endif

	return NO_ERROR;
}

/*
 * @brief	Funcion que configura un LED de un color
 * @param	Temp	Led que se quiere configurar
 * @retval	Codigo de error
 */
error_type WSEN_TIDS_getTemperature(float* Temp)
{

	uint8_t array_temp[2] = {0, 0};

	WSEN_i2c_info.i2c_port 		= I2C_0;
	WSEN_i2c_info.dev_addr		= WSEN_ADDR;
	WSEN_i2c_info.data			= array_temp;
	WSEN_i2c_info.reg_addr 		= DATA_T_L;
	WSEN_i2c_info.length		= 2;

	if(read_i2c_data(&WSEN_i2c_info) != NO_ERROR){

#ifdef DEBUG
		printf("[ERROR]\t[WSEN-TIDS]\tReading temperature value: %s \n", strerror(errno));
#endif

		return APP_REPORT(WSEN_TIDS, READING_TEMPERATURE);
	}

	uint32_t raw_value = 0;

	raw_value = ((array_temp[1] << 8) | (array_temp[0]));

	*Temp = (float)(raw_value * 0.01);

#ifdef DEBUG
		printf("[OK]\t[WSEN-TIDS]\tTemperature value: %.1f C\n", *Temp);
#endif

	return NO_ERROR;
}
