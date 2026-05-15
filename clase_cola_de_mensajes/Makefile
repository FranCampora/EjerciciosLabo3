CC=gcc-4.1
FLAGS = -Wall -O3 -std=gnu89 -pedantic-errors -pthread
HEADERS = -I./
all: clean colaMensajes1 colaMensajes2

colaMensajes1: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c colaMensajes1.c
	$(CC) -o colaMensajes1 global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c colaMensajes1.c $(FLAGS) $(HEADERS)

colaMensajes2: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c colaMensajes2.c
	$(CC) -o colaMensajes2 global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c colaMensajes2.c $(FLAGS) $(HEADERS)
clean:
	rm -rf *o colaMensajes1 colaMensajes2 *.dat sincro.dat
