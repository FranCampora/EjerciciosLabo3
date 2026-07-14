CC=gcc-4.1
FLAGS = -Wall -O3 -std=gnu89 -pedantic-errors -pthread
HEADERS = -I./
all: clean jugador kioscko

jugador: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c jugador.c
	$(CC) -o jugador global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c jugador.c $(FLAGS) $(HEADERS)

kioscko: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c kioscko.c
	$(CC) -o kioscko global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c kioscko.c $(FLAGS) $(HEADERS)
clean:
	rm -rf *o jugador kioscko *.dat sincro.dat
