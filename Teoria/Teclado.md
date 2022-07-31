
# Resumen - Teclado

Suponiendo la implementación de un teclado matricial, donde ambas patas de las teclas o pulsadores están conectadas a diferentes puertos del microcontrolador, se puede detectar una pulsación estableciendo un valor lógico en los puertos conectados a una de las patas (filas) y realizando un barrido de el valor lógico opuesto en los puertos conectados a las otras patas (columnas). Al realizar este barrido, si se detecta algún cambio en el valor de las filas, se puede identificar qué pulsador está oprimido en función de los valores de fila y columna.

Para implementar esto, es necesario implementarlo en el modelo de capas.

- En la capa de Drivers, desarrollamos dos funciones. Una de ellas `keyboardHW()` se encargará de realizar el barrido y retornará el valor de un pulsador presionado si es que hay. La otra `keyboardSW()` hará uso de la anterior e implementará una determinada lógica antirebote para validar correctamente la opresión de una tecla. La función `keyboardSW()` debe ser incluida dentro del `sysTickHandler()` para mantener una determinada actualización del teclado.

- En la capa de buffers o memoria se encontrará una variable `keyBuffer`. La función `keyboardSW()` hará uso de este buffer para almacenar el valor del pulsador que ha sido presionado para que la función primitiva pueda hacer uso de este.

- En la capa de aplicación, habrá una función primitiva `getKey()` que tomará el valor presente en `keyBuffer` y luego limpiará el buffer.
