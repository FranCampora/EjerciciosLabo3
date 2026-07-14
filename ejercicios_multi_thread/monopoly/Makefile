CC=gcc-4.1
FLAGS = -Wall -O3 -std=gnu89 -pedantic-errors -pthread
HEADERS = -I./
all: clean tablero jugador

tablero: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c tablero.c
	$(CC) -o tablero global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c tablero.c $(FLAGS) $(HEADERS)

jugador: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c jugador.c
	$(CC) -o jugador global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c jugador.c $(FLAGS) $(HEADERS)
clean:
	rm -rf *o tablero jugador *.dat sincro.dat
