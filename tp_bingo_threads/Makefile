CC=gcc-4.1
FLAGS = -Wall -O3 -std=gnu89 -pedantic-errors -pthread
HEADERS = -I./
all: clean bingo

bingo: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c bingo.c
	$(CC) -o bingo global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c bingo.c $(FLAGS) $(HEADERS)


proceso2: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c proceso2.c
	$(CC) -o proceso2 global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c threads.c proceso2.c $(FLAGS) $(HEADERS)
clean:
	rm -rf *o bingo *.dat sincro.dat
