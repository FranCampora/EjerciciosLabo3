#ifndef _MEMORIA_H
#define _MEMORIA_H


typedef struct inicial{
	int main1;
	int main2;
}mem_inicial;


void* creo_memoria(int size, int *r_id_memoria);
void liberarMemoria(int *id_memoria, void *memoria);


#endif


