CC=gcc-4.1
FLAGS = -Wall -O3 -std=gnu89 -pedantic-errors -pthread
HEADERS = -I./
all: clean cancha equipo

cancha: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c cancha.c
	$(CC) -o cancha global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c cancha.c $(FLAGS) $(HEADERS)

equipo: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c equipo.c
	$(CC) -o equipo global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c equipo.c $(FLAGS) $(HEADERS)
clean:
	rm -rf *o cancha equipo *.dat sincro.dat
