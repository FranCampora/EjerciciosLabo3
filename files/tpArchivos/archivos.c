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
		/*printf("ERROR AL ABRIR EL FILE %s\n",nombre);*/
		/*NO DEBO USAR printf en las funciones de files, incosistencia con el concepto de FRAMEWORK. Las dejo por si en un futuro tengo problemas de debbug*/
		return FALSE;

	}
	/*printf("SE ABRIO EL FILE %s\n",nombre);*/
	return TRUE;
}

void cerrarArchivo(){
	fclose(fp);
	/*printf("Se cerro el file\n");*/
}

int escribirArchivo(char *texto){
	int inRes = 0;
	inRes = fprintf(fp, "%s", texto);
	if(inRes < 0){
		/*printf("HUBO UN ERROR AL ESCRIBIR EL FILE\n");*/
		return FALSE;
		/*osea salio algo mal*/
	}
	return TRUE;
}

int leerArchivo(char *szBuffer){
	if (fscanf(fp, "%s", szBuffer) != 1)
	{
		/*printf("HUBO UN ERROR AL LEER EL FILE\n");*/
		return FALSE;
	}
	return TRUE;
}
