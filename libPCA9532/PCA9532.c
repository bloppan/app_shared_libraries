/*
 * PCA9532.c
 *
 *  Created on: Aug 2, 2021
 *      Author: bernardo
 */

#include "PCA9532.h"

LED_struct 	LED[12] = {0};

static i2c_device_struct	PCA9532_I2C_info = {0};

/*
 * @brief	Funcion que configura un LED de un color
 * @param	LEDn	Led que se quiere configurar
 * @param	color	Color del led
 * @param	blink	Parpadeo (0 -> led fijo) (1 -> led con parpadeo)
 * @retval	Codigo de error
 */
error_type setLED_Color_Blink(uint8_t LEDn, uint8_t color, uint8_t blink)
{
	error_type errorCode = NO_ERROR;
	uint8_t buffer[3] = {0};

	switch(color) {

		case BLUE:

			LED[LEDn*3 + BLUE].value 	= 1 + blink;
			LED[LEDn*3 + GREEN].value 	= 0;
			LED[LEDn*3 + RED].value 	= 0;

			break;
		case GREEN:

			LED[LEDn*3 + BLUE].value 	= 0;
			LED[LEDn*3 + GREEN].value 	= 1 + blink;
			LED[LEDn*3 + RED].value 	= 0;


			break;
		case RED:

			LED[LEDn*3 + BLUE].value 	= 0;
			LED[LEDn*3 + GREEN].value	= 0;
			LED[LEDn*3 + RED].value 	= 1 + blink;

			break;
		case PURPLE:

			LED[LEDn*3 + BLUE].value 	= 1 + blink;
			LED[LEDn*3 + GREEN].value 	= 0;
			LED[LEDn*3 + RED].value 	= 1 + blink;

			break;

		case YELLOW:

			LED[LEDn*3 + BLUE].value 	= 0;
			LED[LEDn*3 + GREEN].value 	= 1 + blink;
			LED[LEDn*3 + RED].value 	= 1 + blink;

			break;

		case CYAN:

			LED[LEDn*3 + BLUE].value 	= 1 + blink;
			LED[LEDn*3 + GREEN].value 	= 1 + blink;
			LED[LEDn*3 + RED].value 	= 0;

			break;

		case WHITE:

			LED[LEDn*3 + BLUE].value 	= 1 + blink;
			LED[LEDn*3 + GREEN].value 	= 1 + blink;
			LED[LEDn*3 + RED].value 	= 1 + blink;

			break;

		case LED_OFF:

			LED[LEDn*3 + BLUE].value 	= 0;
			LED[LEDn*3 + GREEN].value 	= 0;
			LED[LEDn*3 + RED].value 	= 0;

			break;
		default:

			errorCode = APP_REPORT(PCA9532, INCORRECT_COLOR);
#ifdef DEBUG
			printf("[ERROR]\t[PCA9532]\tIncorrect color \n");
#endif
			break;
	}

	buffer[0] = LED[LED0].value | (LED[LED1].value << 2) | (LED[LED2].value << 4) | (LED[LED3].value << 6);
	buffer[1] = LED[LED4].value | (LED[LED5].value << 2) | (LED[LED6].value << 4) | (LED[LED7].value << 6);
	buffer[2] = LED[LED8].value | (LED[LED9].value << 2) | (LED[LED10].value << 4) | (LED[LED11].value << 6);

	// Datos del envio I2C
	PCA9532_I2C_info.reg_addr 	= 0x16; // LED0 to LED4 with Autoincrement (0x10 | 0x06)
	PCA9532_I2C_info.length		= 3;
	PCA9532_I2C_info.data		= buffer;

	if(write_i2c_data(&PCA9532_I2C_info) != NO_ERROR) {

#ifdef DEBUG
		errorCode = APP_REPORT(PCA9532, SETTING_LED_COLOR);
		printf("[ERROR]\t[PCA9532]\tWriting LED color \n");
#endif
	}else{

#ifdef DEBUG
		printf("[OK]\t[PCA9532]\tLED color set \n");
#endif
	}

	return errorCode;
}
/*
 * @brief	Configuracion del parpadeo de leds
 * @retval	Codigo de error
 */
void PCA9532_Configure_Blink(void)
{
	uint8_t buffer[1] = {0};

	// Valor que se va a guardar en el registro PSC0
	buffer[0] = 0x77;

	// Datos del envio I2C
	PCA9532_I2C_info.reg_addr 	= 0x02; // PSC0 reg address
	PCA9532_I2C_info.length		= 1;
	PCA9532_I2C_info.data		= buffer;

	if(write_i2c_data(&PCA9532_I2C_info) != NO_ERROR) {

#ifdef DEBUG
		printf("[ERROR]\t[PCA9532]\tWriting LED color \n");
#endif
	}else{
#ifdef DEBUG
		printf("[OK]\t[PCA9532]\tLED color set \n");
#endif
	}

	// Valor que se va a guardar en el registro PSC1
	buffer[0] = 0xEE;

	// Datos del envio I2C
	PCA9532_I2C_info.reg_addr 	= 0x04; // PSC1 reg address

	if(write_i2c_data(&PCA9532_I2C_info) != NO_ERROR) {

#ifdef DEBUG
		printf("[ERROR]\t[PCA9532]\tWriting LED color \n");
#endif
	}else{
#ifdef DEBUG
		printf("[OK]\t[PCA9532]\tLED color set \n");
#endif
	}
}
/*
 * @brief	Funcion de inicializacion del PCA9532
 * @retval	Codigo de error
 */
error_type PCA9532_Initialize(void)
{

	error_type errorCode = NO_ERROR;

	void *libHandlerI2C = NULL;
	void *libHandlerGPIO	= NULL;

	char route[256] = {0};

	// Carga la libreria I2C
	strcat(route, I2C_LIBRARY_DIRECTORY);
	strcat(route, I2C_LIBRARY_NAME);

	libHandlerI2C = dlopen(route, RTLD_LAZY);

	if(libHandlerI2C == NULL) {

		errorCode = APP_REPORT(PCA9532, LOADING_I2C_LIBRARY);
#ifdef DEBUG
		printf("[ERROR]\t[PCA9532]\tLoading I2C library\n");
#endif
	}else {
#ifdef DEBUG
		printf("[OK]\t[PCA9532]\tI2C library loaded successfully \n");
#endif
		// Carga la libreria GPIO
			memset(route, 0, sizeof(route));		// Inicializa de nuevo el array a 0
			strcat(route, GPIO_LIBRARY_DIRECTORY);
			strcat(route, GPIO_LIBRARY_NAME);

			libHandlerGPIO = dlopen(route, RTLD_LAZY);

			if(libHandlerGPIO == NULL){

				errorCode = APP_REPORT(PCA9532, LOADING_GPIO_LIBRARY);
	#ifdef DEBUG
			printf("[ERROR]\t[PCA9532]\tLoading GPIO library \n");
	#endif
			}else {
	#ifdef DEBUG
			printf("[OK]\t[PCA9532]\tGPIO Library loaded successfully \n");
	#endif
				// Carga de funciones de la libreria GPIO
				configGPIO = (error_type ( *)(uint8_t, char*)) dlsym(libHandlerGPIO, "configGPIO");
				freeGPIO = (error_type ( *)(uint8_t)) dlsym(libHandlerGPIO, "freeGPIO");
				getGPIO_Value = (error_type ( *)(uint8_t, uint8_t*)) dlsym(libHandlerGPIO, "getGPIO_Value");
				getGPIO_Direction = (error_type ( *)(uint8_t, char*)) dlsym(libHandlerGPIO, "getGPIO_Direction");
				setGPIO_Value = (error_type ( *)(uint8_t, uint8_t)) dlsym(libHandlerGPIO, "setGPIO_Value");

				// Habilita el PCA9532 a nivel alto (1)
				configGPIO(LED_RST, "out");
				setGPIO_Value(LED_RST, 1);

				// Carga de funciones de la libreria I2C:
				read_i2c_data = (error_type ( *)(i2c_device_struct *)) dlsym(libHandlerI2C, "read_i2c_data");
				write_i2c_data = (error_type ( *)(i2c_device_struct *)) dlsym(libHandlerI2C, "write_i2c_data");

				/*	Inicializacion de la estructura de datos I2C del sensor PCA9532	*/
				PCA9532_I2C_info.i2c_port 	= I2C_0;
				PCA9532_I2C_info.dev_addr	= PCA9532_ADDR;

				// Configura el registro PSC0 (parpadeo)
				PCA9532_Configure_Blink();
			}
	}

	return errorCode;
}

