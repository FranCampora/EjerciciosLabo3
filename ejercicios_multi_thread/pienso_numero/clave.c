#include "clave.h"
#include "stdio.h"
#include "stdlib.h"
#include "def.h"


key_t creo_clave(){
	key_t clave;
	clave = ftok(PATH_CLAVE,ENTERO_CLAVE);
	/*path y entero es un punto de encuentro para dos funciones usando un semaforo*/
	if(clave == (key_t)-1){
		printf("No puedo conseguir clave semaforo, mem compartida\n");
		exit(0);
	
	}
	return clave;

}
