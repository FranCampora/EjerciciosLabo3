CC=gcc-4.1
FLAGS = -Wall -O3 -std=gnu89 -pedantic-errors -pthread
HEADERS = -I./
all: clean coliseo luchador

coliseo: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c coliseo.c
	$(CC) -o coliseo global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c coliseo.c $(FLAGS) $(HEADERS)

luchador: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c luchador.c
	$(CC) -o luchador global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c luchador.c $(FLAGS) $(HEADERS)
clean:
	rm -rf *o coliseo luchador *.dat sincro.dat resultados.txt
