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
	int numVuelo;
	char destino[LARGO];
	char nomPasajero[LARGO];
	char buffer[LARGO];

	int id_semaforo;

	id_semaforo = creo_semaforo();
	inicia_semaforo(id_semaforo, VERDE);/*semaforo en 1*/
	
	while (1)
	{
		espera_semaforo(id_semaforo);
		
		numVuelo = cargarInt("Vuelo (0 para terminar): ");
		while ((numVuelo < 1000 || numVuelo > 1010) && numVuelo != 0)
		{
			printf("El numero de vuelo debe ser entre 1000 y 1010\n");
			numVuelo = cargarInt("Vuelo (0 para terminar): ");
		}
		
		if (numVuelo == 0)
		{
			printf("Carga finalizada\n");
			
			exit(0);
		}
	
		
		cargarDestino("Destino de vuelo: ", destino);
		cargarPasajero("Nombre de pasajero: ", nomPasajero);

		
		memset(buffer,0x00,sizeof(buffer));
		sprintf(buffer, "%d-%s-%s\n", numVuelo, destino, nomPasajero);
		

		if(abrirArchivo("lote.dat","a") == TRUE){
			escribirArchivo(buffer);
			cerrarArchivo();
		}
	levanta_semaforo(id_semaforo);/*se le saca el uso a la carga*/
	usleep(2000*1000);
	}	
	return 0;
}
