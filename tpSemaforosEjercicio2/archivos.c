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
		/*NO DEBO USAR printf en las funciones de files, incosistencia con el concepto de FRAMEWORK. Las dejo por si en un futuro tengo problemas de debbug*/
		return FALSE;

	}
	
	return TRUE;
}

void cerrarArchivo(){
	fclose(fp);
	
}

int escribirArchivo(char *texto){
	int inRes = 0;
	inRes = fprintf(fp, "%s", texto);
	if(inRes < 0){
		return FALSE;
	
	}
	return TRUE;
}

int leerArchivo(char *szBuffer)
{
	if(fscanf(fp, " %[^\n]", szBuffer) != 1){
		return FALSE;
	}
		
	fgetc(fp);
	return TRUE;
}
