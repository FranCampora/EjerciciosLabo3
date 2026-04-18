#include "clave.h"
#include "stdio.h"
#include "stdlib.h"
#include "def.h"


key_t creo_clave(){
	key_t clave;
	clave = ftok(PATH_CLAVE,ENTERO_CLAVE);
	if(clave == (key_t)-1){
		printf("No puedo conseguir clave semaforo, mem compartida\n");
		exit(0);
	
	}
	return clave;

}
