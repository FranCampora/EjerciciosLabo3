#ifndef _SEMAFORO_H
#define _SEMAFORO_H

#include "clave.h"
#include "def.h"

int creo_semaforo();
void inicia_semaforo(int id_semaforo, int valor);
void levanta_semaforo(int id_semaforo);
void espera_semaforo(int id_semaforo);

#endif
