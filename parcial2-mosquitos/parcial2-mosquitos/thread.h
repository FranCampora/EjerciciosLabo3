#ifndef _THREAD_H
#define _THREAD_H

void *funcionThread(void *arg);


typedef struct arg_thread{
	int nro_hilo;
	int id_colamsg;
}st_data;


#endif

