#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semaforo.h"
#include "unistd.h"


int main(int argc,char *argv[])
{
	/*char cadena[LARGO]; /* Un array lo suficientemente grande como para guardar la l�nea m�s larga del fichero 
	int nro_producto=0, nro_partida=0;
	FILE *productor; */

	
	int id_semaforo;
	
	id_semaforo =  creo_semaforo();
	inicia_semaforo(id_semaforo, VERDE);
	
	while(1)
	{
		espera_semaforo(id_semaforo);
			/*
			printf("\nPRODUCIMOS-PARTIDA-%04d\n",nro_partida);
			productor = fopen("producto.txt", "a");
			if (productor!=NULL)
			{
				for (nro_producto=0; nro_producto<CANTIDAD_PARTIDA; nro_producto++)
				{
					sprintf(cadena, "PRODUCTO-%04d-%04d\n", nro_partida, nro_producto);
					printf("%s", cadena);
					fprintf(productor,"%s",cadena);
					usleep(INTERVALO_PRODUCTO*1000);
				}
				fclose(productor);
				nro_partida++;
				printf("\nESPERAMOS\n");
			}
			else
			{
				perror ("Error al abrir producto.txt");
			}*/
			printf("\n PRODUCTOR\n");
		levanta_semaforo(id_semaforo);
		usleep(INTERVALO_PARTIDA*1000);
	};
	return 0;
}
