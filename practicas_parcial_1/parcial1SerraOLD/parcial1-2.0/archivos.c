#include "stdlib.h" 
#include "stdio.h"
#include "string.h"
#include "unistd.h"
#include "archivos.h"
#include "define.h"

FILE *fp;

int abrirArchivo(char *path, char *modo)
{
	fp = fopen(path,modo);
	if(fp == NULL)	
		return FALSE;
	else
		return TRUE;
	/*printf("\nArchivo abierto\n");*/
}

void cerrarArchivo()
{
	fclose(fp);
	/*printf("\nArchivo cerrado\n");*/
}	

int escribirArchivo(char *texto)
{
	if(fprintf(fp,"%s",texto) > 0)
		return TRUE;
	else
		return FALSE;

	/*printf("Se escribió en el archivo: %s", path);*/
}

int leerLinea(char *buffer)
{	
	if(fscanf(fp, "%[^\n]", buffer) != 1)
		return FALSE;
	fgetc(fp);
	return TRUE;
}


int existe(char *path) {
	if (abrirArchivo(path,"r")) {
		cerrarArchivo();
		return TRUE;
	}
	return FALSE;
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

/* ---------------------------------------------------------------------- */


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





