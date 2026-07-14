#include <stdio.h>
#include <pthread.h>
#include "global.h"

FILE *fp;
int id_semaforo, id_memoria, id_colamsg;
pthread_mutex_t mutex;
pthread_t *id_hilo;
pthread_attr_t atributos;


