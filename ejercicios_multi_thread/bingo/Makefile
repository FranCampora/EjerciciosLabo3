CC=gcc-4.1
FLAGS = -Wall -O3 -std=gnu89 -pedantic-errors -pthread
HEADERS = -I./
all: clean bingo jugadorBingo

bingo: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c bingo.c
	$(CC) -o bingo global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c bingo.c $(FLAGS) $(HEADERS)

jugadorBingo: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c jugadorBingo.c
	$(CC) -o jugadorBingo global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c jugadorBingo.c $(FLAGS) $(HEADERS)
clean:
	rm -rf *o bingo jugadorBingo *.dat sincro.dat
