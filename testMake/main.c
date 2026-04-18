#include "stdio.h"
#include "stdlib.h"
#include "funciones.h"
#include "def.h"
#include "global.h"
#include "time.h"

int main(int argc, char *argv[]){
	int idNumeroAl=0;

	srand(time(NULL));

	idNumeroAl=idNumeroAleatorio(DESDE,HASTA);

	printf("\n Numero: %d\n",idNumeroAl);
	return 0;
}
