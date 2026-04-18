#include "stdlib.h"
#include "stdio.h"
#include "funciones.h"
#include "def.h"

int idNumeroAleatorio(int desde, int hasta){
	int num = 0;
	num =(rand()%(hasta-desde+1))+desde;
	return num;
}

