##############################################################################################
######################################### Directorios ########################################
##############################################################################################

SHARED_DIR=vm6_shared_libraries
LIB=lib
BUILD_DIR=../../build/$(SHARED_DIR)

##############################################################################################
########################################## Librerias #########################################
##############################################################################################

I2C=I2C_Medux
LIB_I2C_SOURCE=$(LIB)$(I2C)
LIB_I2C_OBJ=$(LIB_I2C_SOURCE)/Release

HTS221=HTS221
LIB_HTS221_SOURCE := $(LIB)$(HTS221)
LIB_HTS221_OBJ=$(LIB_HTS221_SOURCE)/Release

PAC1932=PAC1932
LIB_PAC1932_SOURCE := $(LIB)$(PAC1932)
LIB_PAC1932_OBJ=$(LIB_PAC1932_SOURCE)/Release

GPIO=GPIO
LIB_GPIO_SOURCE := $(LIB)$(GPIO)
LIB_GPIO_OBJ=$(LIB_GPIO_SOURCE)/Release

PCA9532=PCA9532
LIB_PCA9532_SOURCE := $(LIB)$(PCA9532)
LIB_PCA9532_OBJ=$(LIB_PCA9532_SOURCE)/Release

##############################################################################################
###################################### Punto de entrada ######################################
##############################################################################################

#Antes de compilar se hace un clean
all: clean
	# Si existe el directorio build/shared_libraries borra el contenido. Si no existe, crea el directorio 
	if [ -d $(BUILD_DIR)	]; then rm -rf $(BUILD_DIR)/*; else mkdir $(BUILD_DIR); fi
	
	# Compila las librerias
	cd $(LIB_I2C_OBJ); $(MAKE)
	cd $(LIB_HTS221_OBJ); $(MAKE)
	cd $(LIB_PAC1932_OBJ); $(MAKE)
	cd $(LIB_GPIO_OBJ); $(MAKE)
	cd $(LIB_PCA9532_OBJ); $(MAKE)
	
	# Copia las librerias al directorio de destino
	cp $(LIB_I2C_OBJ)/$(LIB)$(I2C).so* $(BUILD_DIR)
	cp $(LIB_HTS221_OBJ)/$(LIB)$(HTS221).so* $(BUILD_DIR)
	cp $(LIB_PAC1932_OBJ)/$(LIB)$(PAC1932).so* $(BUILD_DIR)
	cp $(LIB_GPIO_OBJ)/$(LIB)$(GPIO).so* $(BUILD_DIR)
	cp $(LIB_PCA9532_OBJ)/$(LIB)$(PCA9532).so* $(BUILD_DIR)
	
	@echo '--------------------------'
	@echo 'All shared libraries done!'
	@echo '--------------------------'
	
clean:
	cd $(LIB_I2C_OBJ) && rm -rf  $(I2C)* $(LIB)$(I2C).so*
	cd $(LIB_HTS221_OBJ); rm -rf $(HTS221)* $(LIB)$(HTS221).so*
	cd $(LIB_PAC1932_OBJ); rm -rf $(PAC1932)* $(LIB)$(PAC1932).so*
	cd $(LIB_GPIO_OBJ); rm -rf $(GPIO)* $(LIB)$(GPIO).so*
	cd $(LIB_PCA9532_OBJ); rm -rf $(PCA9532)* $(LIB)$(PCA9532).so*
	
	
	
	
	
	
