Segundo parcial Laboratorio 3


El proceso mosquitos, lanza los hilos, espera a que terminen y antes de finalizar libera todos los recursos.

El proceso pista, envía el inicio a todos los hilos, hace avanzar al humo y recibe cuantos metros en total llevan los mosquitos
Verifica si alguno llegó a la meta o si fue atrapado por el humo, en ambos caso, envía EVT_FIN
En caso contrario, continuúa avanzando

El thread, espera recibir un mensaje y bloquea el mutex:
inicio del turno 
	Calcula los metros que avanza y envía evt_avanzar, con el total de metros.
	Libera el mutex
fin
	Recibe el resultado (escapó o fue atrapado)
	Libera el mutex y sale del while


Luego de compilar el framework, se deben ejecutar ambos proceso para que comience:

./pista
./mosquitos


