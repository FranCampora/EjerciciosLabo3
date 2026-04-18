CC=gcc-4.1
FLAGS = -Wall -O3 -std=gnu89 -pedantic-errors -pthread
HEADERS = -I./
all: clean carga reservas

carga: global.c clave.c semaforo.c carga.c funciones.c archivos.c
	$(CC) -o carga global.c clave.c semaforo.c carga.c funciones.c archivos.c $(FLAGS) $(HEADERS)

reservas: global.c clave.c semaforo.c reservas.c funciones.c archivos.c
	$(CC) -o reservas global.c clave.c semaforo.c reservas.c funciones.c archivos.c $(FLAGS) $(HEADERS)
clean:
	rm -rf *o carga reservas
