CC=gcc-4.1
FLAGS = -Wall -O3 -std=gnu89 -pedantic-errors -pthread
HEADERS = -I./
all: clean banco cajero

banco: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c banco.c
	$(CC) -o banco global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c banco.c $(FLAGS) $(HEADERS)

cajero: global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c cajero.c
	$(CC) -o cajero global.c clave.c semaforo.c funciones.c archivos.c memoria.c mensajes.c cajero.c $(FLAGS) $(HEADERS)
clean:
	rm -rf *o banco cajero *.dat sincro.dat
