#include "stdlib.h"
#include "stdio.h"
#include "funciones.h"
#include "archivos.h"
#include "def.h"


int idNumeroAleatorio(int desde, int hasta){
	int num = 0;
	num =(rand()%(hasta-desde+1))+desde;
	return num;
}

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

void mostrarResumen(char *etiqueta, Totales totales){
	printf("%s", etiqueta);
	printf("Cant. Efectivo: %d\n", totales.cantEfectivo);
	printf("Cant. Cheque: %d\n", totales.cantCheque);
	printf("Total Efectivo: %d\n", totales.totalEfectivo);
	printf("Total Cheque: %d\n", totales.totalCheque);
}

