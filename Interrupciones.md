
# Resumen - Interrupciones:

- Atención de una interrupción:
  1. Termino lo que estaba haciendo (finalizar instrucción en curso)
  2. Recuerdo dónde tengo que continuar (guardar dirección de retorno)
  3. Atiendo la interrupción 
  4. Retorno al punto donde me había quedado

- Cuando llegan varias interrupciones juntas:
  * Establezco prioridades

- Las interrupciones pueden bloquearse o no en función de su criticidad

## Configuración de las interrupciones

- ¿Cómo se configuran?
  * Inicializar la tabla de vectores
  * Configurar el NVIC
  * Configurar el periférico correspondiente + los pines asociados
  * Escribir la rutina de servicio de la interrupción (ISR)

### Registros del NVIC para configurar interrupciones

- ISER0-ISER1: Habilitan las fuentes de interrupción. Cada uno de sus bits corresponde a una fuente de interrupcion. Poner un '1' en un bit implica habilitar la fuente correspondiente a ese bit.
- ICER0-ICER1: Deshabilitan las fuentes de interrupción. Cada uno de sus bits corresponde a una fuente de interrupcion. Poner un '1' en un bit implica deshabilitar la fuente correspondiente a ese bit.
- ISPR0-ISPR1: Setean los flags de interrupción pendiente. Cada uno de sus bits corresponde a una fuente de interrupcion. Poner un '1' en un bit implica marcar esa fuente de interrupción como pendiente.
- ICPR0-ICPR1: Resetean los flags de interrupción pendiente. Cada uno de sus bits corresponde a una fuente de interrupcion. Poner un '1' en un bit implica marcar esa fuente de interrupción como atendida.
- IABR0-IABR1: Sirven para indicar las interrupciones activas. Cada uno de sus bits corresponde a una fuente de interrupcion. Leer un '1' en uno de esos bits indica que esa fuente de interrupción está activa.
- IPR0..IPR8: Sirven para configurar el nivel de prioridad de cada fuente de interrupción. Se utilizan "4" bits para determinar la prioridad de cada fuente
- STIR: Permite generar interrupciones por software para cualquier vector o fuente externa.

### Registros de interrupciones externas

- EXTINT: Bits que indican si una fuente de interrupción externa está activa
  - [bit 0] EINT0: Se encuentra en '1' si se produjo una interrupción en esta fuente, '0' para caso contrario.
  - [bit 1] EINT1: Se encuentra en '1' si se produjo una interrupción en esta fuente, '0' para caso contrario.
  - [bit 2] EINT2: Se encuentra en '1' si se produjo una interrupción en esta fuente, '0' para caso contrario.
  - [bit 3] EINT3: Se encuentra en '1' si se produjo una interrupción en esta fuente, '0' para caso contrario.
  - bits [4 a 31]: RESERVED
- EXTMODE: Bits para configurar si los bits de interrupción externa se activan por flanco o por nivel
  - [bit 0] EXTMODE0: Si se encuentra en '1' la interrupción se activa por flanco, si se encuentra en '0' se activa por nivel.
  - [bit 1] EXTMODE1: Si se encuentra en '1' la interrupción se activa por flanco, si se encuentra en '0' se activa por nivel.
  - [bit 2] EXTMODE2: Si se encuentra en '1' la interrupción se activa por flanco, si se encuentra en '0' se activa por nivel.
  - [bit 3] EXTMODE3: Si se encuentra en '1' la interrupción se activa por flanco, si se encuentra en '0' se activa por nivel.
  - bits [4 a 31]: RESERVED
- EXTPOLAR: Bits para configurar el tipo de flanco o nivel que activará el bit de interrupción externa
  - [bit 0] EXTPOLAR0: Corresponde a EXTINT0. Si se encuentra en '1' la interrupción se activa por flanco ascendente o nivel alto, si se encuentra en '0' se activa por flanco descendente o nivel bajo.
  - [bit 1] EXTPOLAR1: Corresponde a EXTINT1. Si se encuentra en '1' la interrupción se activa por flanco ascendente o nivel alto, si se encuentra en '0' se activa por flanco descendente o nivel bajo.
  - [bit 2] EXTPOLAR2: Corresponde a EXTINT2. Si se encuentra en '1' la interrupción se activa por flanco ascendente o nivel alto, si se encuentra en '0' se activa por flanco descendente o nivel bajo.
  - [bit 3] EXTPOLAR3: Corresponde a EXTINT3. Si se encuentra en '1' la interrupción se activa por flanco ascendente o nivel alto, si se encuentra en '0' se activa por flanco descendente o nivel bajo.
  - bits [4 a 31]: RESERVED

### Ejemplo de configuración

| Interrupción externa | Puerto | Infotronic   | Función |
|----------------------|--------|--------------|---------|
| EINT0                | P210   | SW1          | KEY0    |
| EINT1                | P211   | ENT.DIG2     | BORNERA |
| EINT2                | P212   | EXPANSION 17 | DIP2_2  |
| EINT3                | P213   | SW10         | KEY3    |

```c
//-----------------------------------------------------------------------------
// Configuración de interrupciones externas
//-----------------------------------------------------------------------------

// DEFINICIONES:

// #define KEY0            P2,10
// #define KEY3            P2,13
// #define PINSEL_FUNC1    0x01
// #define	EXTMODE 		*( ( registro * ) 0x400FC148UL)
// #define EXTPOLAR 		*( ( registro * ) 0x400FC14CUL)
// #define ISER		    ( ( registro  * ) 0xE000E100UL )
// #define ISER0		    ISER[0]

void InicExtInt(void) {
    // Configuracion interrupcion externa EINT3, EINT2, EINT1, EINT0
    setPinSel(KEY0, PINSEL_FUNC1);  // Configuro el P210 como interrup Ext EINT 0
    setPinSel(KEY3, PINSEL_FUNC1);  // Configuro el P213 como interrup Ext EINT 3
    EXTMODE = (0x0F);               // Todas x Flanco
    EXTPOLAR = (0x00);              // Todas Polaridad activo bajo
    ISER0 |=(0x01 <<18);            // Habilito Interrupcion externa 0
    ISER0 |=(0x01 <<21);            // Habilito Interrupcion externa 3
}
```
