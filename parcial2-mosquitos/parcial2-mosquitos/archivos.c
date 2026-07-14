#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "archivos.h"
#include "define.h"
#include "global.h"

/*FILE *fp  GLOBAL*/

int abrirArchivo(char *path, char *modo)
{
	fp = fopen(path,modo);
	if(fp != NULL)	
		return TRUE;

	return FALSE;
	/*printf("\nArchivo abierto\n");*/
}

int cerrarArchivo()
{
	if(fp != NULL){
		fclose(fp);
		return TRUE;
	}
	
	return FALSE;
	
	/*printf("\nArchivo cerrado\n");*/	
}	

int leerArchivo(char *path)
{	
	char buffer[LINEA]; /*Buffer para cada línea*/
	memset(buffer,0x00,sizeof(buffer));

	if(abrirArchivo(path,"r") == FALSE)
		return FALSE;

	/*Leer línea por línea hasta el último caracter*/
	while(fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		printf("%s",buffer);
	}
	cerrarArchivo();
	return TRUE;
}

int escribirArchivo(char *path, char *modo, char *string)
{
	if(abrirArchivo(path,modo) == FALSE)
		return FALSE;
	
	fprintf(fp,"%s\n",string);

	/*printf("Se escribió en el archivo: %s", path);*/
	cerrarArchivo();

	return TRUE;
}



int escribirLinea(char *string)
{
	if(fp == NULL)
		return FALSE;

	fprintf(fp, "%s\n",string);
	return TRUE;
}


char* leerLinea()
{
	char *string = (char *)malloc(LINEA*sizeof(char));
	memset(string, 0x00, sizeof(string));

	if(fscanf(fp, "%s\n",string) != 1)
	{
		printf("No se pudo leer el archivo\n");
		free(string);
		return NULL;
	}
	return string;
}



int existe(char *path) {
	if (abrirArchivo(path,"r")) {
		cerrarArchivo();
		return TRUE;
	}
	return FALSE;
}

int contarLineas(char *path)
{	
	char buffer[LINEA]; /*Buffer para cada línea*/
	int contador = 0;

	if(abrirArchivo(path,"r") == FALSE)
		return FALSE;
    

	/*Leer línea por línea hasta el último caracter*/
	while(fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		contador++;
	}
	cerrarArchivo();
	return contador;
}

int renombrarArchivo(char *oldName, char *newName)
{
	if(existe(oldName)){
		rename(oldName,newName);
		return TRUE;
	}
	return FALSE;
}

int borrarArchivo(char *path)
{
	if(existe(path)){
		remove(path);
		return TRUE;
	}
	return FALSE;
}


int leerUltimaLinea(char *path, char *modo, char *ultimaLinea)
{
	char buffer[64];
	memset(buffer,0x00,sizeof(buffer));
	

	if(abrirArchivo(path,"r") == FALSE)
		return FALSE;
	

	while(fgets(buffer,sizeof(buffer),fp) != NULL);

	memcpy(ultimaLinea,buffer,sizeof(buffer));

	cerrarArchivo();
	return TRUE;
}



