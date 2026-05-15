#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <unistd.h>
#include <sys/shm.h>

#include "def.h"
#include "global.h"
#include "semaforo.h"
#include "archivos.h"
#include "funciones.h"
#include "clave.h"
#include "memoria.h"

int main(int argc, char  *argv[]){
    juego *memoria = NULL;
    int id_semaforo;
    int contador = 0;
    int numero_adivino;
    int id_memoria;

    esperarArchivoSincronismo(FILE_SINCRONISMO, 1);

    
    id_semaforo = creo_semaforo();

    memoria = (juego*)creo_memoria(sizeof(juego)*1, &id_memoria);
    srand(time(NULL));
    
    printf("Adivino numero listo. Enter para empezar.\n");
    getchar();
    
    /*voy numero x numero hasta HASTA
    comparo cada numero con el numero escrit en la zona de memoria compartida
    cuando encuentre el numero, escribo en el struct que acerto*/
    cargarData("Ingrese nombre de jugador: ", memoria[0].nombreJugador);

    while (1)
    {
        espera_semaforo(id_semaforo);

        if (memoria[0].numeroPensado == 0 && memoria[0].estadoAcierto == 0)/*el valor 0 se lo puso el PIENSO*/
        {
            numero_adivino = rand()%(HASTA-DESDE)+DESDE;
            contador++;
            printf("Adivino el numero %d\n", numero_adivino);
            memoria[0].numeroPensado = numero_adivino;
        }else if (memoria[0].estadoAcierto == 1)
        {
            printf("Adivine el numero pensado en %d intentos\n", contador);
            contador = 0;
            levanta_semaforo(id_semaforo);
            usleep(2000*1000);
            break;
        }
        
        levanta_semaforo(id_semaforo);
        usleep(1000*1000);
    }
    
    shmdt ((char *)memoria);
	shmctl (id_memoria, IPC_RMID, (struct shmid_ds *)NULL);

    return 0;
}
