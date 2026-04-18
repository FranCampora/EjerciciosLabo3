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

	int id_semaforo;
	
	int i;
	
	char nombreArchivo[LARGO];
	char nombreNuevoArchivo[LARGO];
	char buffer[LARGO];
	Totales totales;
	Totales totalPorCajero[CANTIDAD_DE_CAJEROS+1];

	int lotePorCajero[CANTIDAD_DE_CAJEROS+1];

	int nroCajero;
	int modoCajero;
	int importe;
	int cantDeposito;
	/*inicializa los struct*/
	totales.cantEfectivo = 0;
	totales.cantCheque = 0;
	totales.totalEfectivo = 0;
	totales.totalCheque = 0;

	for (i = 1; i < CANTIDAD_DE_CAJEROS+1; i++){
		totalPorCajero[i].cantEfectivo = 0;
		totalPorCajero[i].cantCheque = 0;
		totalPorCajero[i].totalEfectivo = 0;
		totalPorCajero[i].totalCheque = 0;
		lotePorCajero[i] = 1;
	}

	id_semaforo = creo_semaforo();
	inicia_semaforo(id_semaforo, VERDE);/*semaforo en 1*/
	/*el tesoreroe inicia el proceso*/
	while (1){
		espera_semaforo(id_semaforo);

		printf("Cajero: Atendiendo cliente...\n");
		
		for (i = 1; i <= CANTIDAD_DE_CAJEROS; i++)
		{
			memset(&nombreArchivo,0x00,sizeof(nombreArchivo));
			sprintf(nombreArchivo, "cajero%d.dat", i);

			if(abrirArchivo(nombreArchivo,"r") == TRUE){/*abrir el file*/
				/*espera_semaforo(id_semaforo);*/
				printf("Proceso lote %s\n", nombreArchivo);

				while (leerArchivo(buffer) == TRUE)
				{
					if (sscanf(buffer, "%d-%d-%d-%d", &nroCajero, &modoCajero, &importe, &cantDeposito) == 4){
						if (modoCajero == 1)/*modo de efectivo*/
						{
							totales.cantEfectivo++;
							totales.totalEfectivo += importe;
							totalPorCajero[i].cantEfectivo++;
							totalPorCajero[i].totalEfectivo += importe;
						}else
						{
							totales.cantCheque++;
							totales.totalCheque += importe;
							totalPorCajero[i].cantCheque++;
							totalPorCajero[i].totalCheque += importe;
						}	
					}
				}
				cerrarArchivo();

				printf("TOTALES");
				printf("total CANTIDAD efectivo %d, total EFECTIVO %d, total CANTIDAD cheque %d, total CHEQUE %d\n",totales.cantEfectivo,totales.totalEfectivo,totales.cantCheque,totales.totalCheque);
				
				printf("CAJERO");
				printf("%d, total CANTIDAD efectivo %d, total EFECTIVO %d, total CANTIDAD cheque %d, total CHEQUE %d\n",i,totalPorCajero[i].cantEfectivo,totalPorCajero[i].totalEfectivo,totalPorCajero[i].cantCheque,totalPorCajero[i].totalCheque);

				memset(nombreNuevoArchivo,0x00,sizeof(nombreNuevoArchivo));
				
				sprintf(nombreNuevoArchivo, "cajero%d.%03d.dat", i, lotePorCajero[i]);

				rename(nombreArchivo, nombreNuevoArchivo);
                lotePorCajero[i]++;


			}
		}
	levanta_semaforo(id_semaforo);		
	usleep(1000*2500);		
	}
	return 0;
}
