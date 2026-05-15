#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "sys/ipc.h"
#include "sys/sem.h"
#include "time.h"
#include "unistd.h"
#include "sys/shm.h"

#include "funciones.h"
#include "archivos.h"
#include "def.h"
#include "memoria.h"
#include "semaforo.h"
#include "clave.h"
#include "global.h"
#include "archivos.h"


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

void actualizarArchivo(char *nombreArchivo, int buffer){
	char szBuffer[LARGO];
	if (abrirArchivo(nombreArchivo,"w") == TRUE)
	{
		memset(szBuffer,0x00,sizeof(szBuffer));
		sprintf(szBuffer, "%d\n", buffer);
		escribirArchivo(szBuffer);
		cerrarArchivo();
	}
}

void crearArchivoSincronismo(char *nombreArchivo, int valorInicial){
	char szBuffer[LARGO];
	if (abrirArchivo(nombreArchivo,"w") == TRUE)
	{
		memset(szBuffer,0x00,sizeof(szBuffer));
		sprintf(szBuffer, "%d\n", valorInicial);
		escribirArchivo(szBuffer);
		cerrarArchivo();
		printf("Archivo de sincronismo llamado %s creado con valor: %d\n", nombreArchivo, valorInicial);
		
	}
}

void esperarArchivoSincronismo(char *nombreArchivo, int valorEsperado){
	char szBuffer[LARGO];
	int valorLeido;
	int continuar;
	
	continuar = FALSE;
	valorLeido = -1; /*valor por defecto, no es 0 ni 1 para evitar confusiones*/

	printf("SINCRONISMO DE PROCESO VIA FILE");
	while (continuar == FALSE)
	{
		if (abrirArchivo(nombreArchivo,"r") == FALSE)
		{
			printf("Error SINCRONISMO. ARCHIVO %s NO ENCONTRADO. El proceso inicializador NO COMENZO. Esperando...\n", nombreArchivo);
			sleep(1); /* Espera 1 segundo antes de volver a intentar */
		}else
		{
			if (leerArchivo(szBuffer)==FALSE)
			{
				printf("Error SINCRONISMO. Archivo %s detectado pero VACIO O ILEGIBLE. Esperando...\n", nombreArchivo);
				cerrarArchivo();
			}else
			{
				sscanf(szBuffer, "%d\n", &valorLeido);
				cerrarArchivo();

				if (valorLeido != valorEsperado)
				{
					printf("ERROR SINCRONISMO. Valor del archivo de sincronismo %s es: %d. Se esperaba: %d. Esperando...\n", nombreArchivo, valorLeido, valorEsperado);
				}
				else
				{
					printf("EXITO SINCRONISMO. Valor del archivo de sincronismo %s es: %d. Continuando...\n", nombreArchivo, valorLeido);
					continuar = TRUE;
				
				}
			}
		}
		if (continuar == FALSE)
		{
			sleep(1); /* Espera 1 segundo antes de volver a intentar */
		}
		printf("--------------------------------------------------\n");
		printf("SINCRONISMO FINALIZADO. Valor del archivo de sincronismo %s es: %d. Continuando...\n", nombreArchivo, valorLeido);
	}
}
