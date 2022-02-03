/*
 * gpio.c
 *
 *  Created on: Jul 23, 2021
 *      Author: bernardo
 */

#include "GPIO.h"

static pthread_mutex_t gpio_mutex = PTHREAD_MUTEX_INITIALIZER;

//#define	DEBUG

/**
 * @brief	Funcion que exporta un gpio y lo configura como output o input. Primero se exporta el GPIO y despues se configura.
 * @param	gpio		Numero de GPIO que se va a configurar
 * @param	direction	Direccion (out o in)
 * @retval	Codigo de error
 */
error_type configGPIO(uint8_t gpio, char* direction)
{
	error_type 	codeError = NO_ERROR;
	int8_t 		fd_export;					// File descriptor export file
	int8_t  	fd_direction;				// File descriptor direction file
	char 		gpio_str[4] = {0}; 			// String gpio
	char		gpio_path_str[50] = {0}; 	// String path gpio

	// Bloqueo mutex
	pthread_mutex_lock (&gpio_mutex);

	fd_export = open("/sys/class/gpio/export", O_WRONLY);

	if(fd_export < 0){

		codeError = APP_REPORT(GPIO, OPENING_EXPORT_FILE);
#ifdef DEBUG
		printf("[ERROR]\t[GPIO]\t\tOpening GPIO %d export file: %s \n", gpio, strerror(errno));
#endif
	}else{
#ifdef DEBUG
		printf("[OK]\t[GPIO]\t\tExport file opened \n");
#endif
		// Numero de gpio como cadena de caracteres
		sprintf(gpio_str, "%d", gpio);

		// Exporta el pin escribiendo el numero de gpio en el fichero /sys/class/gpio/export
		if(write(fd_export, gpio_str, strlen(gpio_str)) < 1){

			codeError = APP_REPORT(GPIO, WRITING_EXPORT_FILE);
#ifdef DEBUG
		printf("[ERROR]\t[GPIO]\t\tWriting GPIO %d export file: %s. Maybe the GPIO is already exported \n", gpio, strerror(errno));
#endif
		}else{

			sprintf(gpio_path_str, "/sys/class/gpio/gpio%d/direction", gpio);

			fd_direction = open(gpio_path_str, O_WRONLY);

			if(fd_direction < 0){

				codeError = APP_REPORT(GPIO, OPENING_DIRECTION_FILE);
#ifdef DEBUG
		printf("[ERROR]\t[GPIO]\t\tOpening gpio direction file: %s \n", strerror(errno));
#endif
			}else{

				if(!strncmp("out", direction, 3)){

					if(write(fd_direction, "out", strlen("out")) < 0){

						codeError = APP_REPORT(GPIO, WRITING_DIRECTION_FILE);
#ifdef DEBUG
		printf("[ERROR]\t[GPIO]\t\tSetting GPIO %d as output \n", gpio);
#endif
					}else{
#ifdef DEBUG
		printf("[OK]\t[GPIO]\t\tGPIO %d set as out\n", gpio);
#endif
					}
				}else if(!strncmp("in", direction, 2)){

					if(write(fd_direction, "in", strlen("in")) < 0){

						codeError = APP_REPORT(GPIO, WRITING_DIRECTION_FILE);
#ifdef DEBUG
		printf("[ERROR]\t[GPIO]\t\tSetting GPIO %d as input \n", gpio);
#endif
					}else{
#ifdef DEBUG
		printf("[OK]\t[GPIO]\t\tGPIO %d set as in\n", gpio);
#endif
					}
				}else{
					codeError = APP_REPORT(GPIO, INVALID_DIRECTION_VALUE);
#ifdef DEBUG
		printf("[ERROR]\t[GPIO]\t\tDirection \"%s\" is not valid. Must be \"out\" or \"in\" \n", direction);
#endif
				}
				close(fd_direction);
			}
		}
		close(fd_export);
	}

	// Desbloqueo mutex
	pthread_mutex_unlock (&gpio_mutex);

	return codeError;
}

/**
 * @brief	Funcion que libera un GPIO (necesario hacer cuando se quiere cambiar de output a input o viceversa si ya esta exportado).
 * @param	gpio	Numero de GPIO que se quiere liberar
 * @retval	Codigo de error
 */
error_type freeGPIO(uint8_t gpio)
{
	error_type	codeError = NO_ERROR;
	int8_t 		fd_unexport;					// File descriptor export file
	char 		gpio_str[4] = {0}; 			// String gpio

	// Bloqueo mutex
	pthread_mutex_lock (&gpio_mutex);

	fd_unexport = open("/sys/class/gpio/unexport", O_WRONLY);

	if(fd_unexport < 0){

		codeError = APP_REPORT(GPIO, OPENING_UNEXPORT_FILE);
#ifdef DEBUG
		printf("[ERROR]\t[GPIO]\t\tOpening gpio unexport file: %s \n", strerror(errno));
#endif
	}else{
#ifdef DEBUG
		printf("[OK]\t[GPIO]\t\tUnexport file opened \n");
#endif

		// Numero de gpio como cadena de caracteres
		sprintf(gpio_str, "%d", gpio);

		// Exporta el pin escribiendo el numero de gpio en el fichero /sys/class/gpio/export
		if(write(fd_unexport, gpio_str, strlen(gpio_str)) < 1){

			codeError = APP_REPORT(GPIO, WRITING_UNEXPORT_FILE);
#ifdef DEBUG
		printf("[ERROR]\t[GPIO]\t\tWriting GPIO %d unexport file: %s. Maybe the GPIO is already unexported \n", gpio, strerror(errno));
#endif
		}else{
#ifdef DEBUG
		printf("[OK]\t[GPIO]\t\tUnexported GPIO %d \n", gpio);
#endif
		}

		close(fd_unexport);
	}

	// Desbloqueo mutex
	pthread_mutex_unlock (&gpio_mutex);

	return codeError;
}

/**
 * @brief	Funcion que obtiene el valor actual de un GPIO
 * @param	gpio	Numero de GPIO del que se quiere obtener el valor
 * @param	value	Puntero a la variable donde se almacenara el valor
 * @retval	Codigo de error
 */
error_type getGPIO_Value(uint8_t gpio, uint8_t* value)
{
	error_type codeError = NO_ERROR;

	char 	gpio_value_str[2] = {0}; 	// String gpio
	char	gpio_path_str[50] = {0}; 	// String path gpio

	int8_t	fd_value;		// File descriptor

	sprintf(gpio_path_str, "/sys/class/gpio/gpio%d/value", gpio);

	// Bloqueo mutex
	pthread_mutex_lock (&gpio_mutex);

	fd_value = open(gpio_path_str, O_RDONLY);

	if(fd_value < 0){

		codeError = APP_REPORT(GPIO, OPENING_VALUE_FILE);
#ifdef DEBUG
	printf("[ERROR]\t[GPIO]\t\tOpening gpio %d value file: %s \n", gpio, strerror(errno));
#endif
	}else{
		if(read(fd_value, gpio_value_str, 1) < 1){

			codeError = APP_REPORT(GPIO, READING_VALUE_FILE);
#ifdef DEBUG
	printf("[ERROR]\t[GPIO]\t\tOpening gpio %d value file: %s \n", gpio, strerror(errno));
#endif
		}else{

			*value = atoi(gpio_value_str);
#ifdef DEBUG
	printf("[OK]\t[GPIO]\t\tGPIO %d value is: %d \n", gpio, *value);
#endif
		}

		close(fd_value);
	}

	// Desbloqueo mutex
	pthread_mutex_unlock (&gpio_mutex);

	return codeError;
}

/**
 * @brief	Funcion que obtiene la direccion actual de un GPIO (output o input)
 * @param	gpio		Numero de GPIO del cual se quiere obtener la direccion
 * @param	direction	Puntero a la cadena de caracteres donde se almacenara la direccion
 * @retval	Codigo de error
 */
error_type getGPIO_Direction(uint8_t gpio, char* direction)
{
	error_type codeError = NO_ERROR;

	char	gpio_path_str[50] = {0}; 	// String path gpio
	int8_t	fd_direction;		// File descriptor

	sprintf(gpio_path_str, "/sys/class/gpio/gpio%d/direction", gpio);

	// Bloqueo mutex
	pthread_mutex_lock (&gpio_mutex);

	fd_direction = open(gpio_path_str, O_RDONLY);

	if(fd_direction < 0){

		codeError = APP_REPORT(GPIO, OPENING_DIRECTION_FILE);
#ifdef DEBUG
	printf("[ERROR]\t[GPIO]\t\tOpening gpio %d value file: %s \n", gpio, strerror(errno));
#endif
	}else{
		if(read(fd_direction, direction, 4) < 2){

			codeError = APP_REPORT(GPIO, READING_DIRECTION_FILE);
#ifdef DEBUG
	printf("[ERROR]\t[GPIO]\t\tOpening gpio %d value file: %s \n", gpio, strerror(errno));
#endif
		}else{

#ifdef DEBUG
	printf("[OK]\t[GPIO]\t\tGPIO %d direction is: %s \n", gpio, direction);
#endif
		}

		close(fd_direction);
	}

	// Desbloqueo mutex
	pthread_mutex_unlock (&gpio_mutex);

	return codeError;
}

/**
 * @brief	Funcion que cambia el valor de un GPIO de salida (configurado como output)
 * @param	gpio	Numero de GPIO del cual se quiere cambiar el valor
 * @param	value	Valor que se quiere dar al GPIO (debe ser 0 o 1)
 * @reval	Codigo de error
 */
error_type setGPIO_Value(uint8_t gpio, uint8_t value)
{
	error_type	codeError = NO_ERROR;

	char 	gpio_value_str[2] = {0}; 	// String gpio
	char	gpio_path_str[50] = {0}; 	// String path gpio

	int8_t	fd_value;

	// Bloqueo mutex
	pthread_mutex_lock (&gpio_mutex);

	if(value >= 0 && value <= 1) {

		sprintf(gpio_path_str, "/sys/class/gpio/gpio%d/value", gpio);

		fd_value = open(gpio_path_str, O_WRONLY);

		if(fd_value < 0){

			codeError = APP_REPORT(GPIO, OPENING_VALUE_FILE);
#ifdef DEBUG
		printf("[ERROR]\t\t[GPIO]\tOpening GPIO %d value file: %s \n", gpio, strerror(errno));
#endif
		}else {

			sprintf(gpio_value_str, "%d", value);

			if(write(fd_value, gpio_value_str, strlen(gpio_value_str)) < 0){

				codeError = APP_REPORT(GPIO, WRITING_VALUE_FILE);
#ifdef DEBUG
		printf("[ERROR]\t\t[GPIO]\tWriting GPIO %d value file: %s \n", gpio, strerror(errno));
#endif

			}else{
#ifdef DEBUG
		printf("[OK]\t[GPIO]\t\tGPIO %d value set: %s \n", gpio, gpio_value_str);
#endif
			}
			close(fd_value);
		}
	}else{

		codeError = APP_REPORT(GPIO, INVALID_OUTPUT_VALUE);
#ifdef DEBUG
		printf("[ERROR]\t[GPIO]\t\tValue must be \"0\" or \"1\" \n");
#endif
	}

	// Desbloqueo mutex
	pthread_mutex_unlock (&gpio_mutex);

	return codeError;
}

