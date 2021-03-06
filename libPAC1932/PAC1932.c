/*
 * PAC1932.c
 *
 *  Created on: Jul 6, 2021
 *      Author: bernardo
 */

#include "PAC1932.h"

static i2c_device_struct PAC1932_i2c_info = {0};	// Estructura de datos I2C del sensor PAC1932
static float PAC1932_FSC 		= 0;	// Full Scale Current
static float PAC1932_PowerFSC 	= 0;	// Power Full Scale Current

/*
 * @brief	Funcion de inicializacion del sensor PAC1932. Se cargan las funciones de comunicacion I2C
 * @return	errorCode	Codigo de error
 */
error_type PAC1932_Initialize(void)
{
	error_type errorCode 	= NO_ERROR;
	void *libHandlerI2C 	= NULL;
	void *libHandlerGPIO	= NULL;
	char route[256] 		= {0};

	// Carga la libreria I2C
	strcat(route, I2C_LIBRARY_DIRECTORY);
	strcat(route, I2C_LIBRARY_NAME);

	libHandlerI2C = dlopen(route, RTLD_LAZY);

	if(libHandlerI2C == NULL) {

#ifdef DEBUG
		printf("[ERROR]\t[PAC1932]\tLoading I2C library \n");
#endif
		errorCode = APP_REPORT(PAC1932, LOADING_I2C_LIBRARY);

	}else {
#ifdef DEBUG
		printf("[OK]\t[PAC1932]\tI2C Library loaded successfully \n");
#endif
		// Carga la libreria GPIO
		memset(route, 0, sizeof(route));		// Inicializa de nuevo el array a 0
		strcat(route, GPIO_LIBRARY_DIRECTORY);
		strcat(route, GPIO_LIBRARY_NAME);

		libHandlerGPIO = dlopen(route, RTLD_LAZY);

		if(libHandlerGPIO == NULL){

			errorCode = APP_REPORT(PAC1932, LOADING_GPIO_LIBRARY);
#ifdef DEBUG
		printf("[ERROR]\t[PAC1932]\tLoading GPIO library \n");
#endif
		}else {
#ifdef DEBUG
		printf("[OK]\t[PAC1932]\tGPIO Library loaded successfully \n");
#endif
			// Carga de funciones de la libreria GPIO
			configGPIO = (error_type ( *)(uint8_t, char*)) dlsym(libHandlerGPIO, "configGPIO");
			freeGPIO = (error_type ( *)(uint8_t)) dlsym(libHandlerGPIO, "freeGPIO");
			getGPIO_Value = (error_type ( *)(uint8_t, uint8_t*)) dlsym(libHandlerGPIO, "getGPIO_Value");
			getGPIO_Direction = (error_type ( *)(uint8_t, char*)) dlsym(libHandlerGPIO, "getGPIO_Direction");
			setGPIO_Value = (error_type ( *)(uint8_t, uint8_t)) dlsym(libHandlerGPIO, "setGPIO_Value");

			// Habilita el sensor de potencia a nivel bajo (0)
			configGPIO(PWDN_CURR_SENSOR, "out");
			setGPIO_Value(PWDN_CURR_SENSOR, 1);

			// Carga de funciones de la libreria I2C:
			read_i2c_data = (error_type( *)(i2c_device_struct *)) dlsym(libHandlerI2C, "read_i2c_data");
			write_i2c_data = (error_type( *)(i2c_device_struct *)) dlsym(libHandlerI2C, "write_i2c_data");

			// Inicializacion de la estructura I2C del sensor PAC1932. A continuacion se indica el puerto I2C y la direccion del sensor
			PAC1932_i2c_info.i2c_port	= I2C_0;
			PAC1932_i2c_info.dev_addr	= PAC1932_ADDR;

			// Calculo del FULL-SCALE CURRENT (Equation 4.3 del Datasheet)
			PAC1932_FSC = (100 / R_SENSE);
			// Calculo del Power FULL-SCALE CURRENT (Equation 4.5 del Datasheet)
			PAC1932_PowerFSC = (3.2 / R_SENSE);
		}
	}

	return errorCode;
}
/*
 * @brief	Funcion que obtiene los valores de voltaje, intensidad y potencia de todos los canales
 * @param	PAC1932_Value	Puntero a la estructura de datos del sensor PAC1932
 * @return	errorCode		Codigo de error
 */
error_type PAC1932_GetAllValues(PAC1932_struct* PAC1932_Value)
{
	uint8_t		buffer[50] = {0};	// Buffer de recepcion de datos del bus I2C
	uint64_t 	valor_aux  = 0;		// Variable auxiliar para obtener los valores de potencia, voltaje e intensidad
	uint8_t 	index_reg = 0;		// Indice de registros leidos
	uint8_t 	index_byte = 0;		// Indice de bytes leidos
	uint8_t 	i, j;


	// En primer lugar se envia el comando REFRESH para que se actualicen los valores de los registros del sensor
	PAC1932_i2c_info.data		= buffer;
	PAC1932_i2c_info.reg_addr 	= REFRESH;
	PAC1932_i2c_info.length		= 0;

	if(write_i2c_data(&PAC1932_i2c_info) != NO_ERROR) goto error;

	// Segun el datasheet hay que esperar al menos 1ms para que los valores de los registros se estabilicen
	usleep(1500); // Espera 1,5ms

	// Se leen los valores ACC_COUNT, VPOWER_ACC, VBUS y VSENSE de todos los canales
	PAC1932_i2c_info.reg_addr 	= ACC_COUNT;
	PAC1932_i2c_info.length		= (ACC_COUNT_LENGTH + VPOWER_ACC_LENGTH + VBUS_LENGTH + VSENSE_LENGTH) * 4; // Longitud total de bytes a leer

	if(read_i2c_data(&PAC1932_i2c_info) != NO_ERROR) goto error;

	// Valor del ACC_COUNT
	PAC1932_Value->Acc_Count = 0;

	for(i = 0; i < ACC_COUNT_LENGTH; i++){
		PAC1932_Value->Acc_Count = (PAC1932_Value->Acc_Count << 8) | buffer[i];
		index_byte++;
	}
	index_reg = index_byte;

	// Valores de POTENCIA (W)
	for(i = 0; i < TOTAL_CHANNELS; i ++){
		valor_aux = 0;
		for(j = index_reg; j < (VPOWER_ACC_LENGTH + index_reg); j++){
			valor_aux = (valor_aux << 8) | buffer[j];
			index_byte++;
		}
		index_reg = index_byte;

		/*
		 * El valor de potencia obtenido es la acumulacion (suma) de varias muestras. Por eso hay que dividirlo por el
		 * numero de veces que se han realizado dichas acumulaciones (ACC_COUNT)
		 */
		switch(i){

			case(SOM):
				PAC1932_Value->SOM.Power = ((valor_aux * PAC1932_PowerFSC) / (DEN_UNIPOLAR_MODE)) / PAC1932_Value->Acc_Count;
				break;
			case(Peripherals):
				PAC1932_Value->Peripherals.Power = ((valor_aux * PAC1932_PowerFSC) / (DEN_UNIPOLAR_MODE)) / PAC1932_Value->Acc_Count;
				break;
			case(Terminal_Battery):
				PAC1932_Value->Terminal_Battery.Power = ((valor_aux * PAC1932_PowerFSC) / (DEN_UNIPOLAR_MODE)) / PAC1932_Value->Acc_Count;
				break;
			case(USB_Connector):
				PAC1932_Value->USB_Connector.Power = ((valor_aux * PAC1932_PowerFSC) / (DEN_UNIPOLAR_MODE)) / PAC1932_Value->Acc_Count;
				break;
			default:
				goto error;
				break;
			}
		}

	// Valores de VOLTAJE (V)

	for(i = 0; i < TOTAL_CHANNELS; i ++){
		valor_aux = 0;
		for(j = index_reg; j < (VBUS_LENGTH + index_reg); j++){
			valor_aux = (valor_aux << 8) | buffer[j];
			index_byte++;
		}
		index_reg = index_byte;

		switch(i){

			case(SOM):
				PAC1932_Value->SOM.Voltage = (valor_aux * 32.0) / DEN_UNIPOLAR_MEAS;
				break;
			case(Peripherals):
				PAC1932_Value->Peripherals.Voltage = (valor_aux * 32.0) / DEN_UNIPOLAR_MEAS;
				break;
			case(Terminal_Battery):
				PAC1932_Value->Terminal_Battery.Voltage = (valor_aux * 32.0) / DEN_UNIPOLAR_MEAS;
				break;
			case(USB_Connector):
				PAC1932_Value->USB_Connector.Voltage = (valor_aux * 32.0) / DEN_UNIPOLAR_MEAS;
				break;
			default:
				goto error;
				break;
		}
	}

	// Valores de INTENSIDAD (mA)

	for(i = 0; i < TOTAL_CHANNELS; i ++){
		valor_aux = 0;
		for(j = index_reg; j < (VSENSE_LENGTH + index_reg); j++){
			valor_aux = (valor_aux << 8) | buffer[j];
			index_byte++;
		}
		index_reg = index_byte;

		switch(i){

			case(SOM):
				PAC1932_Value->SOM.Intensity = (valor_aux * PAC1932_FSC) / DEN_UNIPOLAR_MEAS;
				break;
			case(Peripherals):
				PAC1932_Value->Peripherals.Intensity = (valor_aux * PAC1932_FSC) / DEN_UNIPOLAR_MEAS;
				break;
			case(Terminal_Battery):
				PAC1932_Value->Terminal_Battery.Intensity = (valor_aux * PAC1932_FSC) / DEN_UNIPOLAR_MEAS;
				break;
			case(USB_Connector):
				PAC1932_Value->USB_Connector.Intensity = (valor_aux * PAC1932_FSC) / DEN_UNIPOLAR_MEAS;
				break;
			default:
				goto error;
				break;
		}
	}

#ifdef DEBUG
	printf("[OK]\t[PAC1932]\tChannel 0 -> Voltaje: %f V; Intensidad: %f mA; Potencia: %f W\n",PAC1932_Value->SOM.Voltage, PAC1932_Value->SOM.Intensity, PAC1932_Value->SOM.Power);
	printf("[OK]\t[PAC1932]\tChannel 1 -> Voltaje: %f V; Intensidad: %f mA; Potencia: %f W\n",PAC1932_Value->Peripherals.Voltage, PAC1932_Value->Peripherals.Intensity, PAC1932_Value->Peripherals.Power);
	printf("[OK]\t[PAC1932]\tChannel 2 -> Voltaje: %f V; Intensidad: %f mA; Potencia: %f W\n",PAC1932_Value->Terminal_Battery.Voltage, PAC1932_Value->Terminal_Battery.Intensity, PAC1932_Value->Terminal_Battery.Power);
	printf("[OK]\t[PAC1932]\tChannel 3 -> Voltaje: %f V; Intensidad: %f mA; Potencia: %f W\n",PAC1932_Value->USB_Connector.Voltage, PAC1932_Value->USB_Connector.Intensity, PAC1932_Value->USB_Connector.Power);
#endif

	return (error_type)NO_ERROR;

	error:

#ifdef DEBUG
	printf("[ERROR]\t[PAC1932]\tReading PAC1932 values \n");
#endif

	return APP_REPORT(PAC1932, READING_PAC1932_VALUES);
}





