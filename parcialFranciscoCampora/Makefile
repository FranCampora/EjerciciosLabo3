CC=gcc-4.1
FLAGS = -Wall -O3 -std=gnu89 -pedantic-errors -pthread
HEADERS = -I./
all: clean usuariosFinanciera entidadFinanciera

usuariosFinanciera: global.c clave.c semaforo.c funciones.c archivos.c usuariosFinanciera.c
	$(CC) -o usuariosFinanciera global.c clave.c semaforo.c funciones.c archivos.c usuariosFinanciera.c $(FLAGS) $(HEADERS)

entidadFinanciera: global.c clave.c semaforo.c funciones.c archivos.c entidadFinanciera.c
	$(CC) -o entidadFinanciera global.c clave.c semaforo.c funciones.c archivos.c entidadFinanciera.c $(FLAGS) $(HEADERS)
clean:
	rm -rf *o usuariosFinanciera entidadFinanciera *.dat
