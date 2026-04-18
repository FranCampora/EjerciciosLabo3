CC=gcc-4.1
FLAGS = -Wall -O3 -std=gnu89 -pedantic-errors -pthread
HEADERS = -I./
all: clean programa productor consumidor

programa: main.c global.c clave.c ConsumidorConSemaforos.c Productor-ConSemaforos.c semaforo.c
	$(CC) -o programaExe main.c global.c clave.c ConsumidorConSemaforos.c Productor-ConSemaforos.c semaforo.c $(FLAGS) $(HEADERS)

productor: global.c clave.c Productor-ConSemaforos.c semaforo.c
	$(CC) -o productor global.c clave.c Productor-ConSemaforos.c semaforo.c $(FLAGS) $(HEADERS)

consumidor: global.c clave.c semaforo.c ConsumidorConSemaforos.c
	$(CC) -o consumidor global.c clave.c semaforo.c ConsumidorConSemaforos.c $(FLAGS) $(HEADERS)
clean:
	rm -rf *o programaExe consumidor productor
