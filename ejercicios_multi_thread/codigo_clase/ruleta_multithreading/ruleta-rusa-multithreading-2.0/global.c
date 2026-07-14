#include <stdio.h>
#include <pthread.h>
#include "global.h"

FILE *fp;
int id_semaforo, id_memoria, id_colamsg;
pthread_mutex_t mutex;
pthread_t *idHilo;
pthread_attr_t atributos;


