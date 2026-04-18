#include "stdlib.h"
#include "stdio.h"
#include "funciones.h"
#include "archivos.h"
#include "def.h"

int cargarInt(char *mensaje){
	int num = 0;
	printf("%s", mensaje);
	scanf("%d", &num);
	return num;
}

void cargarDestino(char *mensaje, char *destino){
	printf("%s",mensaje);
	scanf(" %[^\n]", destino);
}

void cargarPasajero(char *mensaje, char *pasajero){
	printf("%s",mensaje);
	scanf(" %[^\n]", pasajero);
}
