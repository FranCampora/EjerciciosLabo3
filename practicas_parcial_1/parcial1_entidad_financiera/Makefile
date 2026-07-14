CC=gcc-4.1
FLAGS = -Wall -O3 -std=gnu89 -pedantic-errors -pthread
HEADERS = -I./
all: clean proceso1 proceso2

proceso1: global.c clave.c semaforo.c funciones.c archivos.c memoria.c proceso1.c
	$(CC) -o proceso1 global.c clave.c semaforo.c funciones.c archivos.c memoria.c proceso1.c $(FLAGS) $(HEADERS)

proceso2: global.c clave.c semaforo.c funciones.c archivos.c memoria.c proceso2.c
	$(CC) -o proceso2 global.c clave.c semaforo.c funciones.c archivos.c memoria.c proceso2.c $(FLAGS) $(HEADERS)
clean:
	rm -rf *o proceso1 proceso2 *.dat sincro.dat
