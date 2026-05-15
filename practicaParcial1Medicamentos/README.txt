Francisco Campora - parcial de repaso - sincronica jueves - 1er cuatrimestre 2026

Pense la logica de la siguiente manera: el superderivador recibe, lo escrobe en un file
los cajeros estan leyendo ese file y dependiendo la cantidad que lean "agarran" ese pedido y lo procesan
la eleccion del cajero es depende de la cantidad del producto, definida en def.h. 
cada cajero tiene su minima cantidad y su maxima cantidad.
el file se llama buffer.det

El precio lo calcula cada cajero. El superderivador le pasa el precio y la cantidad.

la struct de Producto esta definida en def.h y tiene los atributos de precio, cantidad y descripción

con el comando make se compila los dos files llamados caja.c y superDerivador.c

intrucciones:
1. compilar con make
2. ejecutar el superDerivador con ./superDerivador
3. ejecutar los tres procesos de caja en cualquier orden: ./caja 1 , ./caja 2 , ./caja 3
4. cargar un producto en la terminal de superDerivador
5. va a aparecer en pantalla de la terminal correspondiente dependiendo de la cantidad de productos