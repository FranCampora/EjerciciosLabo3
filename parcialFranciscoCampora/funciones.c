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

int cargarPrecio(char *mensaje){
	int num = 0;
	printf("%s", mensaje);
	scanf("%d", &num);
	return num;
}


void cargarData(char *mensaje, char *data){
	printf("%s",mensaje);
	scanf(" %[^\n]", data);
}
