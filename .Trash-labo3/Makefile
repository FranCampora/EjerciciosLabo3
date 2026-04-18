CC=gcc-4.1
FLAGS = -Wall -o3 -std=gnu89 -pedantic-errors -pthread
HEADERS = -I./
all: clean programa

programa: main.c global.c funciones.c
	$(CC) -o programaExe main.c global.c funciones.c $(FLAGS) $(HEADERS)

clean:
	rm -rf *o programaExe
