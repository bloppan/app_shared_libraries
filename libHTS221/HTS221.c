/*
 * HTS221.c
 *
 *  Created on: Jul 1, 2021
 *      Author: bernardo
 */


#include "HTS221.h"

static i2c_device_struct HTS221_i2c_info = {0};	// Estructura de parametros I2C del sensor HTS221
static HTS221_struct	 HTS221_data = {0};		// Estructura de datos del sensor HTS221

/*
 * @brief	Funcion para hacer el calculo de una interpolacion lineal
 * @param	Puntos de referencia para realizar la interpolacion
 * @return	Resultado de la interpolacion
 */
static float linearInterpolation(float x, float x0, float x1, float y0, float y1)
{

	return ((y1 - y0)/(x1 - x0))*(x - x0) + y0;
}
/*
 * @brief	Funcion que obtiene los parametros de calibracion del sensor HTS221
 * @return	errorCode	Codigo de error tras ejecutar la funcion
 */
static error_type	HTS221_getCalibrationValues(void)
{
	uint8_t buffer[1];

	/*	Inicializacion de la estructura de datos I2C del sensor HTS221	*/

	HTS221_i2c_info.i2c_port 	= I2C_0;
	HTS221_i2c_info.dev_addr	= HTS221_ADDR;
	HTS221_i2c_info.data		= buffer;
	HTS221_i2c_info.length		= 1;

	/*	Get Temperature Calibration Data	*/

	HTS221_i2c_info.reg_addr = T0_degC_x8;
	if(read_i2c_data(&HTS221_i2c_info) != NO_ERROR) goto error;
	HTS221_data.T0_degC = buffer[0];

	HTS221_i2c_info.reg_addr = T1_degC_x8;
	if(read_i2c_data(&HTS221_i2c_info) != NO_ERROR) goto error;
	HTS221_data.T1_degC = buffer[0];

	HTS221_i2c_info.reg_addr = T1_T0;
	if(read_i2c_data(&HTS221_i2c_info) != NO_ERROR) goto error;
	HTS221_data.T0_degC += ((buffer[0]&0x03) << 8);
	HTS221_data.T0_degC /= 8;
	HTS221_data.T1_degC += ((buffer[0]&0x0C) << 6);
	HTS221_data.T1_degC /= 8;

	HTS221_i2c_info.reg_addr = T0_OUT_L;
	if(read_i2c_data(&HTS221_i2c_info) != NO_ERROR) goto error;
	HTS221_data.T0_OUT = buffer[0];

	HTS221_i2c_info.reg_addr = T0_OUT_H;
	if(read_i2c_data(&HTS221_i2c_info) != NO_ERROR) goto error;
	HTS221_data.T0_OUT |= (buffer[0] << 8);

	HTS221_i2c_info.reg_addr = T1_OUT_L;
	if(read_i2c_data(&HTS221_i2c_info) != NO_ERROR) goto error;
	HTS221_data.T1_OUT = buffer[0];

	HTS221_i2c_info.reg_addr = T1_OUT_H;
	if(read_i2c_data(&HTS221_i2c_info) != NO_ERROR) goto error;
	HTS221_data.T1_OUT |= (buffer[0] << 8);

#ifdef DEBUG

	printf("\n[OK]\t[HTS221]\tDatos de calibracion de temperatura: \n\n");
	printf("\tT0_degC = %.2f \n", HTS221_data.T0_degC);
	printf("\tT1_degC = %.2f \n", HTS221_data.T1_degC);
	printf("\tT0_OUT = %d -> 0x%x \n", HTS221_data.T0_OUT, HTS221_data.T0_OUT);
	printf("\tT1_OUT = %d -> 0x%x \n", HTS221_data.T1_OUT, HTS221_data.T1_OUT);
	printf("\n");

#endif

	/*	Get Humidity Calibration Data	*/

	HTS221_i2c_info.reg_addr = H0_rH_x2;
	if(read_i2c_data(&HTS221_i2c_info) != NO_ERROR) goto error;
	HTS221_data.H0_rH = buffer[0];
	HTS221_data.H0_rH /= 2;

	HTS221_i2c_info.reg_addr = H1_rH_x2;
	if(read_i2c_data(&HTS221_i2c_info) != NO_ERROR) goto error;
	HTS221_data.H1_rH = buffer[0];
	HTS221_data.H1_rH /= 2;

	HTS221_i2c_info.reg_addr = H0_T0_OUT_L;
	if(read_i2c_data(&HTS221_i2c_info) != NO_ERROR) goto error;
	HTS221_data.H0_T0_OUT = buffer[0];

	HTS221_i2c_info.reg_addr = H0_T0_OUT_H;
	if(read_i2c_data(&HTS221_i2c_info) != NO_ERROR) goto error;
	HTS221_data.H0_T0_OUT |= (buffer[0] << 8);

	HTS221_i2c_info.reg_addr = H1_T0_OUT_L;
	if(read_i2c_data(&HTS221_i2c_info) != NO_ERROR) goto error;
	HTS221_data.H1_T0_OUT = buffer[0];

	HTS221_i2c_info.reg_addr = H1_T0_OUT_H;
	if(read_i2c_data(&HTS221_i2c_info) != NO_ERROR) goto error;
	HTS221_data.H1_T0_OUT |= (buffer[0] << 8);

#ifdef DEBUG

	printf("\n[OK]\t[HTS221]\tDatos de calibracion de humedad: \n\n");
	printf("\tH0_RH = %.2f \n", HTS221_data.H0_rH);
	printf("\tH1_RH = %.2f \n", HTS221_data.H1_rH);
	printf("\tH0_T0_OUT = %d -> 0x%x \n", HTS221_data.H0_T0_OUT, HTS221_data.H0_T0_OUT);
	printf("\tH1_T0_OUT = %d -> 0x%x \n", HTS221_data.H1_T0_OUT, HTS221_data.H1_T0_OUT);
	printf("\n");

#endif

	return (error_type)NO_ERROR;

	error:

#ifdef DEBUG
	printf("[ERROR]\t[HTS221]\tReading HTS221 calibration values \n");
#endif

	return APP_REPORT(HTS221, READING_CALIBRATION_VALUES);
}

/*
 * @brief	Funcion que calcula la humedad
 * @param	Hum			Puntero a la variable que almacena el valor de humedad
 * @return	errorCode	Codigo de error tras ejecutar la funcion
 */
error_type	HTS221_getHumidity(float *Hum)
{
	uint8_t buffer[0];

	HTS221_i2c_info.reg_addr = HUMIDITY_OUT_L;
	HTS221_i2c_info.data	 = buffer;
	if(read_i2c_data(&HTS221_i2c_info) != NO_ERROR) goto error;

	HTS221_data.H_OUT = buffer[0];

	HTS221_i2c_info.reg_addr = HUMIDITY_OUT_H;
	if(read_i2c_data(&HTS221_i2c_info) != NO_ERROR) goto error;

	HTS221_data.H_OUT += (buffer[0] << 8);

	*Hum = linearInterpolation(HTS221_data.H_OUT, HTS221_data.H0_T0_OUT, HTS221_data.H1_T0_OUT, HTS221_data.H0_rH, HTS221_data.H1_rH);

	return (error_type)NO_ERROR;

	error:

#ifdef DEBUG
	printf("[ERROR]\t[HTS221]\tReading HTS221 humidity value \n");
#endif

	return APP_REPORT(HTS221, READING_HUMIDITY);
}

/*
 * @brief	Funcion que calcula la temperatura
 * @param	Temp		Puntero a la variable que almacena el valor de temperatura
 * @return	errorCode	Codigo de error tras ejecutar la funcion
 */
error_type	HTS221_getTemperature(float *Temp)
{
	uint8_t buffer[0];

	HTS221_i2c_info.reg_addr = TEMP_OUT_L;
	HTS221_i2c_info.data	 = buffer;
	if(read_i2c_data(&HTS221_i2c_info) != NO_ERROR) goto error;

	HTS221_data.T_OUT = buffer[0];

	HTS221_i2c_info.reg_addr = TEMP_OUT_H;
	if(read_i2c_data(&HTS221_i2c_info) != NO_ERROR) goto error;

	HTS221_data.T_OUT += (buffer[0] << 8);

	*Temp = linearInterpolation(HTS221_data.T_OUT, HTS221_data.T0_OUT, HTS221_data.T1_OUT, HTS221_data.T0_degC, HTS221_data.T1_degC);

	return (error_type)NO_ERROR;

	error:

#ifdef DEBUG
	printf("[ERROR]\t[HTS221]\tReading HTS221 temperature value \n");
#endif

	return APP_REPORT(HTS221, READING_TEMPERATURE);
}
/*
 * @brief	Funcion de inicializacion del sensor HTS221. Se cargan las funciones de comunicacion I2C y se
 * 			obtienen los parametros de calibracion del sensor.
 * @return	errorCode	Codigo de error tras ejecutar la funcion
 */
error_type HTS221_Initialize(void)
{
	error_type errorCode = NO_ERROR;
	void *libHandlerI2C = NULL;
	char route[256] = {0};

	// Carga la libreria I2C
	strcat(route, I2C_LIBRARY_DIRECTORY);
	strcat(route, I2C_LIBRARY_NAME);

	libHandlerI2C = dlopen(route, RTLD_LAZY);

	if(libHandlerI2C == NULL) {

		errorCode = APP_REPORT(HTS221, LOADING_I2C_LIBRARY);
#ifdef DEBUG
		printf("[ERROR]\t[HTS221]\tLoading I2C library\n");
#endif
	}else {
#ifdef DEBUG
		printf("[OK]\t[HTS221]\tI2C library loaded successfully \n");
#endif
		// Carga de funciones de la libreria I2C:
		read_i2c_data = (error_type ( *)(i2c_device_struct *)) dlsym(libHandlerI2C, "read_i2c_data");
		write_i2c_data = (error_type ( *)(i2c_device_struct *)) dlsym(libHandlerI2C, "write_i2c_data");

		// Se obtienen los parametros de calibracion del sensor
		errorCode = HTS221_getCalibrationValues();
	}

	return errorCode;
}



