#include "stdlib.h"
#include "stdio.h"
#include "funciones.h"
#include "def.h"
#include "archivos.h"
#include "global.h"

FILE *fp;

int abrirArchivo(char *nombre, char *tipo){
	fp = fopen(nombre,tipo);
	if(fp == NULL){
		printf("ERROR AL ABRIR EL FILE");
		return FALSE;

	}
	return TRUE;

}

void cerrarArchivo(){
	fclose(fp);
}

void escribirArchivo(char *texto){
	fprintf(fp,texto);
	printf("archivo escrito\n");

}

void leerArchivo(){
	char buffer[255];
	
	while(fgets(buffer,sizeof(buffer),fp) != NULL){
		printf("%s",buffer);
	}
}
