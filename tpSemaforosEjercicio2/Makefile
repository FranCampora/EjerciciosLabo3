CC=gcc-4.1
FLAGS = -Wall -O3 -std=gnu89 -pedantic-errors -pthread
HEADERS = -I./
all: clean  cajero tesorero

cajero: global.c clave.c semaforo.c cajero.c funciones.c archivos.c
	$(CC) -o cajero global.c clave.c semaforo.c cajero.c funciones.c archivos.c $(FLAGS) $(HEADERS)

tesorero: global.c clave.c semaforo.c tesorero.c funciones.c archivos.c
	$(CC) -o tesorero global.c clave.c semaforo.c tesorero.c funciones.c archivos.c $(FLAGS) $(HEADERS)
clean:
	rm -rf *o cajero tesorero
