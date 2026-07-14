#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <pthread.h>

#include "global.h"
#include "define.h"
#include "funciones.h"
#include "archivos.h"
#include "semaforo.h"
#include "memoria.h"
#include "colamsg.h"
#include "thread.h"

int main(int argc, char *argV[])
{
	int i, arma_caida, nro_atacante, nro_equipo_atacante, dano, victima, vida_restante, se_acerto;
	int luchador_muerto, equipo_del_muerto;
	int fin = 0, turno_actual = 0, primera_muerte = 0;
	int luchadores_vivos[CANTIDAD_HILOS] = {TRUE, TRUE, TRUE, TRUE};
    int equipo_de_luchador[CANTIDAD_HILOS] = {0, 0, 1, 1};
	char buffer[LINEA];
	char nombre_muerto[CORTO];
	char uso_arma_mistica;

	mem_inicial *inicial = NULL;
	
	srand(time(NULL));
	
	inicial = (mem_inicial*)creo_memoria(sizeof(mem_inicial), &id_memoria);
	inicial->main2 = 1; /*Indico que el main2 inicio*/

	if(inicial->main1 != 1) /*Valido que el main1 haya iniciado*/
		printf("Esperando que inicie el proceso equipos....\n");

	while(inicial->main1 != 1){ 
		usleep(200 * MS);
	}
	if(inicial->main1 == inicial->main2){
		liberarMemoria(&id_memoria, inicial);
	}

	id_colamsg = creo_cola_mensajes(); /*Obtengo el id de la cola de mensajes*/

	
	system("clear");
	printf("\n================ PROCESO RING ================\n");
	
	printf("Presione <enter> para comenzar ");
	getchar();


	while(!fin)
    {
		memset(&msg,0x00,sizeof(mensaje));
		memset(&buffer,0x00,sizeof(buffer));

        /*Valida que el luchador del turno actual esté vivo*/
        if (luchadores_vivos[turno_actual]) {
            arma_caida = devolverNumAleatorio(1, 4); /*1 o 2 son armas, 3 o 4 nada*/
            
            /*Envia la orden de atacar*/
			memset(&buffer,0x00,sizeof(buffer));
			sprintf(buffer, "%d", arma_caida);

            enviar_mensaje(id_colamsg, MSG_HILO_BASE + turno_actual, MSG_RING, EVT_TURNO_PELEAR, buffer);

            /* 1. Recibe los datos del golpe generados por el hilo*/
            recibir_mensaje(id_colamsg, MSG_RING, &msg);
            
			if(msg.int_evento == EVT_ENVIAR_GOLPE)
			{
                sscanf(msg.char_mensaje, "%d %d %d", &nro_atacante, &nro_equipo_atacante, &dano);

                /*El golpe va a los 2 integrantes del equipo opuesto si estan vivos*/
                for(i=0;i<CANTIDAD_HILOS;i++) {
                    if(equipo_de_luchador[i] != nro_equipo_atacante && luchadores_vivos[i]) {
						memset(&buffer,0x00,sizeof(buffer));
                        sprintf(buffer, "%d %d", nro_atacante, dano);
                        enviar_mensaje(id_colamsg, MSG_HILO_BASE + i, MSG_RING, EVT_PROCESAR_GOLPE, buffer);
                        
                        /*Espera respuesta de impacto o muerte de ese oponente*/
                        recibir_mensaje(id_colamsg, MSG_RING, &msg);
						memset(&buffer,0x00,sizeof(buffer));

						/*CASO sobrevive*/
						switch (msg.int_evento)
						{
						case EVT_RESPUESTA_GOLPE:
							sscanf(msg.char_mensaje, "%d %d %d", &victima, &vida_restante, &se_acerto);
                            
							sprintf(buffer, "Luchador %d ataca a %d | Daño: %d | Acierto: %d | Vida: %d\n", nro_atacante, victima, dano, se_acerto, vida_restante);

							break;
						case EVT_INFORMAR_MUERTE:
							sscanf(msg.char_mensaje, "%d %d %s", &luchador_muerto, &equipo_del_muerto, nombre_muerto);
							luchadores_vivos[luchador_muerto] = FALSE;
							
							sprintf(buffer, "Luchador %d elimino a '%s' del equipo '%d'\n", nro_atacante, nombre_muerto, equipo_del_muerto +1);
							
							if(!primera_muerte)
							{
								printf("Primer luchador caido del equipo %d\nUtilizara el arma mistica??\n",equipo_del_muerto+1);

								/*Envia mensaje al proceso padre de los hilos preguntando si usar el arma mistica*/
								memset(&buffer,0x00,sizeof(buffer));
								sprintf(buffer, "%d", equipo_del_muerto);
								enviar_mensaje(id_colamsg, MSG_EQUIPOS, MSG_RING, EVT_ARMA_MISTICA, buffer);

								recibir_mensaje(id_colamsg, MSG_RING, &msg);
								sscanf(msg.char_mensaje, "%c", &uso_arma_mistica);
								if(uso_arma_mistica == 's')
								{
									/*0. explota y muere, 1. gana*/
									if(devolverNumAleatorio(0, 1)) /*gana*/
									{
										printf("\nEl arma mistica funciono\n");
										printf("\nGana el equipo %d!\n", equipo_del_muerto+1);
										fin = 1;
									}else /*Explota y pierden*/
									{
										printf("\nEl luchador del equipo %d explota y muere\n",equipo_del_muerto+1);
										printf("\nGana el equipo %d!\n", nro_equipo_atacante+1);
										fin = 1;
									}
								}else
									printf("[RING] El equipo %d no utiliza el arma mistica. Continúa el combate\n", equipo_del_muerto+1);

								primera_muerte = 1;
							}
							break;
						default:
							break;
						}

						/* Guarda en el archivo*/
						if(abrirArchivo(ARCHIVO, "a")) {
							escribirArchivo(buffer);
							printf(buffer);
							cerrarArchivo();
						}
                    }
                }
            }


            /*Fin de juego (si un equipo se queda sin nadie) */
            if (!luchadores_vivos[0] && !luchadores_vivos[1]) {
                printf("\nGana el equipo 2!\n");
                fin = 1;
            } else if (!luchadores_vivos[2] && !luchadores_vivos[3]) {
                printf("\n¡Gana el equipo 1!\n");
                fin = 1;
            }

            usleep(200 * MS);
        }

		/*Cede el turno al siguiente participante*/
        turno_actual = (turno_actual + 1) % CANTIDAD_HILOS;
    }

    /*Avisa a todos los hilos que terminen*/
    for(i=0;i<CANTIDAD_HILOS;i++) {
        enviar_mensaje(id_colamsg, MSG_HILO_BASE + i, MSG_RING, EVT_FIN_COMBATE, NULL);
    }

	return 0;
}

