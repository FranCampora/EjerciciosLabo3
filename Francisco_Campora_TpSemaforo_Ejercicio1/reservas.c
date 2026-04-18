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
	int numLote = 0;
	char nuevoNombre[LARGO];
	int numVuelo;
	char destino[LARGO];
	char nomPasajero[LARGO];
	char buffer[LARGO];
	
	int id_semaforo;

	id_semaforo = creo_semaforo();

	
	while(1){
		espera_semaforo(id_semaforo);/*en cada ejecucion de reserva*/
		if(abrirArchivo(NOMBRE_FILE,"r") == TRUE){
			while(leerArchivo(buffer) == TRUE){
					sscanf(buffer, " %d-%[^-]-%[^\n]", &numVuelo, destino, nomPasajero);/*lee el buffer y lo separa por - y lo guarda en las variables*/
					printf("Vuelo: %d, destino:%s, pasajero:%s\n",numVuelo,destino,nomPasajero);
				}

			cerrarArchivo();
			
			memset(nuevoNombre,0x00,sizeof(nuevoNombre));
			sprintf(nuevoNombre,"lote.%03d.dat",numLote);/*armo el nuevo nombre del file. %03d le da 3 digitos antes*/
			rename(NOMBRE_FILE,nuevoNombre);/*1er parametro el nombre del file, 2do el nuevo q le estoy dando*/
			/*cambio el nombre de lote.dat entonces sale de while 1*/
			numLote++;
		}else{
			printf("No se abre el file LOTE\n");/*r es solo para lectura, si el file no existe falla*/
			}
		levanta_semaforo(id_semaforo);	/*semaforo en 1*/
		usleep(2000*1000);
	}
	return 0;
}
