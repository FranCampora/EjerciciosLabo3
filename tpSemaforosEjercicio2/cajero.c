#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <unistd.h>

#include "def.h"
#include "global.h"
#include "semaforo.h"
#include "archivos.h"
#include "funciones.h"

int main(int argc,char *argv[]){
	int numeroCajero;
	int modoCajero;
	int importe;
	int cantDeposito;
	char buffer[LARGO];
	int id_semaforo;
	int i;
	char nombreArchivo[LARGO];

	id_semaforo = creo_semaforo();

	srand(time(NULL));

	if (argc != 2)
		{
			printf("Ingrese el numero de cajero (1, 2 o 3)\n",argv[0]);
			exit(1);
		}
		numeroCajero = atoi(argv[1]);
		if (numeroCajero < 1 || numeroCajero > 3)
		{
			printf("Numero de cajero invalido.\n");
			exit(1);
		}
	
	while (1)
	{
		espera_semaforo(id_semaforo);
		cantDeposito = idNumeroAleatorio(DEPOSITO_MINIMO, DEPOSITO_MAXIMO);

		memset(nombreArchivo,0x00,sizeof(nombreArchivo));
		sprintf(nombreArchivo, "cajero%d.dat", numeroCajero);
		if(abrirArchivo(nombreArchivo,"a") == TRUE){
			for( i = 0; i < cantDeposito; i++)
			{
				importe = idNumeroAleatorio(IMPORTE_MINIMO, IMPORTE_MAXIMO);
				modoCajero = idNumeroAleatorio(0, 1);

				memset(buffer,0x00,sizeof(buffer));
				sprintf(buffer, "%d-%d-%d-%d\n", numeroCajero, modoCajero, importe, cantDeposito);
				escribirArchivo(buffer);
				
			}
			cerrarArchivo();

			printf("El cajero %d , lote de %d se genero en %s\n", numeroCajero, cantDeposito, nombreArchivo);


		}else
		{
			printf("Error cuando el cajero %d quiso abrir el archivo %s\n", numeroCajero, nombreArchivo);
		}

		levanta_semaforo(id_semaforo);
		usleep(1000*2500);
		




	}
	
	return 0;
}
