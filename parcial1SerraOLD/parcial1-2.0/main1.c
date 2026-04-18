#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <unistd.h>

#include "define.h"
#include "semaforo.h"
#include "archivos.h"
#include "funciones.h"

int main(int arg, char *argc[])
{
	int i, id_semaforo, nroUsuario, saldo;
	char buffer[LINEA];

/*creo e inicializo el semaforo*/
	id_semaforo = crearSemaforo();
	iniciarSemaforo(id_semaforo);

	/*Cargo el archivo inicial. Tambien lo utilizo como flag para que los cajeros puedan iniciar*/
	if(abrirArchivo(ARCHIVO, "w")) {
        for(i = 0; i < USUARIOS_MAX; i++) {
            sprintf(buffer, "%d-%d\n", i + 1, SALDO_INICIAL);
            escribirArchivo(buffer);
        }
        cerrarArchivo();
    }

	while(1)
	{
		system("clear");
		printf("======= Proceso BANCO =========\n");
		
		esperarSemaforo(id_semaforo);
		printf("\n-- Saldos actuales --\n");

		if(abrirArchivo(ARCHIVO,"r"))
		{
			while (leerLinea(buffer))
			{
				sscanf(buffer,"%d-%d",&nroUsuario,&saldo);
				printf("Usuario %d -- saldo: $%d\n", nroUsuario, saldo);
			}
			cerrarArchivo();
		}

		levantarSemaforo(id_semaforo);
		usleep(5000 * MS);
	}

	return 0;
}

