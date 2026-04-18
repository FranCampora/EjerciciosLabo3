#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>

#include "clave.h"
#include "define.h"

key_t creo_clave()
{
	key_t clave;
	clave = ftok(COMANDO,ENTERO);
	
	if (clave == (key_t)-1)
	{
		printf("No se pudo conseguir la clave\n");
		exit(0);
	}
	return clave;
}
