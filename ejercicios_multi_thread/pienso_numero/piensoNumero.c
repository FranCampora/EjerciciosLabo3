#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <unistd.h>
#include <sys/shm.h>
#include <pthread.h>


#include "def.h"
#include "global.h"
#include "semaforo.h"
#include "archivos.h"
#include "funciones.h"
#include "clave.h"
#include "memoria.h"
#include "mensajes.h"
#include "threads.h"

int main(int argc, char  *argv[]){
    int id_cola_mensajes;
    int i;
    int local_pienso_numero;

    juego *memoria = NULL;
    int id_memoria;


    srand(time(NULL));

    /*inicializacion*/


    memoria = (juego*)creo_memoria(sizeof(juego)*1, &id_memoria);

    memoria[0].numeroPensado = 0;
    memoria[0].estadoAcierto = 0;

    crearArchivoSincronismo(FILE_SINCRONISMO, 1);
    borrar_mensajes(id_cola_mensajes);

    printf("PIENSO numero listo. Enter para empezar.\n");
    getchar();

    local_pienso_numero = rand()%(HASTA-DESDE)+DESDE;
    printf("Pienso el numero %d\n", local_pienso_numero);

    /*logica
    pienso numero
    me llegan numeros del adivino, y comparo con el numero pensado
    si no es correcto, escribo en la memoria compartida que no acerto osea 0
    y escribo en la meoria compartida el numero es 0

    si acerto, escribo en la memoria compartida que acerto osea 1

    PIENSO NO ESCRIBE EN LA MEMORIA el numero pensado, lo HACE EL ADIVINO
*/



    while (1)
    {
        espera_semaforo(id_semaforo);
    
        if (memoria[0].estadoAcierto == 0 && memoria[0].numeroPensado != 0)
        {
            printf("El adivino no acerto el numero pensado. ADIVINO EL %d\n", memoria[0].numeroPensado);
            memoria[0].estadoAcierto = 0;
        
            if (memoria[0].numeroPensado == local_pienso_numero)
            {
                printf("El adivino acerto el numero pensado. ADIVINO EL %d\n", memoria[0].numeroPensado);
                memoria[0].estadoAcierto = 1;
                levanta_semaforo(id_semaforo);
                usleep(1000*1000);
                break;
            }else
            {
                printf("El adivino no acerto el numero pensado. RESETO A 0 \n");
                memoria[0].estadoAcierto = 0;
                memoria[0].numeroPensado = 0;
                /*le pone el numero pensado VALOR 0, para que ADIVINO, siga buscando*/ 
            }
            
            
        }
        
        levanta_semaforo(id_semaforo);
        usleep(1000*1000);

    }
    printf("El adivino acerto el numero pensado. ADIVINO EL %d\n", memoria[0].numeroPensado);


    borrar_cola_de_mensajes(id_cola_mensajes);

    return 0;
}
