#ifndef _MEMORIA_H
#define _MEMORIA_H


typedef struct{
	int main1;
	int main2;
	int meta;
}mem_inicial;

typedef struct{
	int id_cliente;
	int saldo;
}mem_data;

mem_data* creo_memoria(int size, int *r_id_memoria);
void liberarMemoria(int *id_memoria, void *memoria);


#endif


