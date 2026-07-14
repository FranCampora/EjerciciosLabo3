#ifndef _GLOBAL_H
#define _GLOBAL_H

extern FILE *fp;
extern int id_semaforo, id_memoria, id_colamsg;

extern pthread_mutex_t mutex;
extern pthread_t *idHilo;
extern pthread_attr_t atributos;

#endif

