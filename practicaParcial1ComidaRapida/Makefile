CC=gcc-4.1
FLAGS = -Wall -O3 -std=gnu89 -pedantic-errors -pthread
HEADERS = -I./
all: clean menu derivadorMenu

menu: global.c clave.c semaforo.c funciones.c archivos.c menu.c
	$(CC) -o menu global.c clave.c semaforo.c funciones.c archivos.c menu.c $(FLAGS) $(HEADERS)

derivadorMenu: global.c clave.c semaforo.c funciones.c archivos.c derivadorMenu.c
	$(CC) -o derivadorMenu global.c clave.c semaforo.c funciones.c archivos.c derivadorMenu.c $(FLAGS) $(HEADERS)
clean:
	rm -rf *o menu derivadorMenu buffer.dat
