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
		printf("ERROR AL ABRIR EL FILE\n");
		return FALSE;

	}
	return TRUE;
	printf("SE ABRIO CORRECTAMENTE EL FILE\n");
}

void cerrarArchivo(){
	fclose(fp);
	printf("Se cerro el file\n");
}

void escribirArchivo(char *texto){
	int inRes = 0;
	inRes = fprintf(fp, "%s", texto);
	if(inRes < 0){
		return FALSE;
		printf("HUBO UN ERROR AL ESCRIBIR EL FILE\n");
		/*osea salio algo mal*/
	}
	return TRUE;
	printf("ESCRIBIO CORRECTAME EL FILE \n");	
}

void leerArchivo(char *szBuffer){
	fscanf(fp, "%s", szBuffer);
	return TRUE;
}
