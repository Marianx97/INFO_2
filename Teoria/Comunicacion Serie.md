# Comunicación serie

## Características

- Velocidad menor que la de la comunicación en paralelo (hay que enviar un bit a la vez)
- Capaz de abarcar grandes distancias
- La transmición puede ser asincrónica o sincrónica

## Tipos

- Simplex: Un dispositivo transmisor y uno receptor. La comunicación es unidireccional
- Hard-Duplex: Dos dispositivos transmisores y dos receptores, en total cuatro dispositivos. La comunicación es bidireccional, pero los pares tx y rx se turnan para comunicarse en una dirección y en la otra.
- Duplex: Dos dispositivos capaces de transmitir y recibir simultáneamente.

## Sincronismo

- Comunicación sincrónica: Ambos dispositivos comparten una línea de clk.
- Comunicación asincrónica: Cada dispositivo posee su propia línea de clk, conocen la velocidad de transmición y se sincronizan utilizando un protocolo que incluye bits de start y stop y una cantidad determinada de bits de datos.

### La trama

Es un concepto que se refiere a un conjunto de bits que representan un mensaje entre el transmisor y el receptor. La trama está compuesta por bits que indican el inicio y el fin de la trama (bit de start, bit de stop), bits que conrresponden particularmente al dato que se quiere transmitir (bits de datos) y un bit de paridad que sirve como control de errores.

### Tipos de errores

- Error de paridad: Se detecta un error en el bit de paridad
- Error en la trama: Se espera un bit de stop pero no coincide, implica que se perdió la sincronización
- Error de sobreescritura: Falló la lectura de uno de los símbolos y se reemplaza con uno nuevo que llegó después
- Error de break: La línea Rx queda en un estado bajo un tiempo mayor al de un caracter luego de una trama válida.

## UART - Universal Asynchronous Receiver Transmiter

Es un dispositivo que se encarga de la elaboración y comunicación de la trama de datos. Está compuesto por:
* Asynchronous Communication Element (ACE): Se ocupa del armado de la trama
* Baud Rate Generator (BRG): Se ocupa de generar la velocidad de transmición

Los bits transmitidos deben evaluarse en el centro del bit para asegurarse de que es un bit correctamente enviado.

El BaudRate se calcula como la señal de clock que recibe la UART dividida por el producto entre 16 y un divisor programable. Este cálculo se realiza para sobremuestrear la señal de entrada.

$BR = \frac{PCLK\_UARTn}{16*Divisor}$

### Registros de la UART

* RBR: Receiver Buffer Register
* THR: Transmiter Holding Register
* DLL: Divisor Latch LSB
* DLM: Divisor Latch MSB
* IER: Interrupt Enable Register
* IIR: Interrupt Identification Register
* FCT: FIFO Control Register
* LCR: Line Control Register
* MCR: Modem Control Register
* LSR: Line Status Register
* MSR: Modem Status Register
* SCR: Scratch Register

### Trabajando con la UART

Paso a paso para utilizar la UART
1. Prender la UART
    El registro PCONP (Power Control for Peripherals Register) controla el encendido y apagado de los periféricos y permite ahorrar energía apagando aquellos que no se encuentren en uso. Cada uno de sus bits controla un periférico. Los bits 3 y 4 corresponden a las UART:
    - bit 3: PCUART0
    - bit 4: PCUART1

2. Determinar qué señal de clk se le envía a la UART
    Los registros PCLKSEL0 y PCLKSEL1 controlan la señal de clk que recibe cada periférico, que puede ser igual a la señal de clk del sistema (CCLK) o un submúltiplo de esta. Para el caso de la UART:
    - PCLKSEL0[bits 6 y 7]: PCLK_UART0
    - PCLKSEL0[bits 8 y 9]: PCLK_UART1
    Los valores que pueden tomar este par de bits son:
    - 00: CCLK/4
    - 01: CCLK
    - 10: CCLK/2
    - 11: CCLK/8

3. Configuración de la UART a través del registro LCR
  - En este paso, es necesario que el bit DLAB del registro LCR se encuentre en 1 para tener acceso a los registros DLL y DLM, que son los que permiten configurar el valor del BaudRate
4. Cargar los valores de DLL y DLM para determinar el BaudRate
  - El BaudRate se calcula como: $BR = \frac{UARTn_clk}{MSB * 256 + LSB}$
  - Suponiendo $MSB = 10$ y $LSB = 44$, entonces: $BR = \frac{25MHz}{10 * 256 + 44} = 9600,61$ => 9600 baudios
5. Configurar los pines para utilizar como las líneas Rx y Tx
  - Trabajando con la UART0, el pin 2 del puerto 0, configurado en su función 1, corresponde a Tx y el pin 3 del puerto 0, configurado en su función 1, corresponde a Rx
6. Configurar el bit DLAB del registro LCR en 0 para tener acceso a los registros IER, RBR y THR
  - RBR (Receiver Buffer Register): Contiene el próximo caracter recibido por ser leído
  - THR (Transmiter Holding Register): Contiene el próximo caracter por ser enviado
  - IER (Interrupt Enable Register): Configura las interrupciones de la UART
7. Habilitar las interrupciones
  - En la UART utilizando el registro UnIER
  - En el NVIC utilizando el registro ISER0


### Ejemplo de función de inicialización

```c
void initUART0 (void) {
    // 1. Prender la UART
    PCONP |= 0x01 << 3;
    /* Ponemos un '1' en el bit 3 del registro PCONP para "prender" la UART0 */

    // 2. Determinar qué señal de clk se le envía a la UART
    PCLKSEL0 &= ~(0x03 << 6);
    /* PCLKSEL0[6,7]: 00 -> PCLK_UART0: CCLK/4 (CCLK = 100MHz -> clk de la UART0 = 25MHz) */

    // 3. Configuración de la UART a través del registro LCR
    U0LCR = 0x83; // "0x83"|16 = "10000011"|2
    /*
        Al poner este valor configuramos la UART0 con las siguientes especificaciones:
        - DLAB: 1 => accedemos a los divisores de latches para configurar el baudrate
        - SB: 0 => desabilitamos la condición de break
        - SP: 0 => desabilitamos el forzado de bit de paridad
        - EPS: 0 => paridad impar (indistinto ya que la paridad está desabilitada)
        - PEN: 0 => desabilitamos el bit de paridad
        - STB: 0 => cantidad de bits de stop = 1
        - WLS0/1 : 11 => la longitud del mensaje es 8 bits de datos
    */

    // 4. Cargar los valores de DLL y DLM para determinar el BaudRate
    U0DLM = 0x0A;   // MSB: 10
    U0DLL = 0x2C;   // LSB: 44
    /* BR = 25MHz / (10 * 256 + 44) = 9600,61 => 9600 baudios */
    
    // 5. Configurar los pines para utilizar como las líneas Rx y Tx
    SetPinSel(P0, 2, PINSEL_FUNC1);
    SetPinSel(P0, 3, PINSEL_FUNC1);
    /*
        Configuramos el pin 2 del puerto 0 con su función 1, que es TX de la UART0
        Configuramos el pin 3 del puerto 0 con su función 1, que es RX de la UART0
    */

    // Configurar el bit DLAB del registro LCR en 0 para tener acceso a los registros IER, RBR y THR
    U0LCR = 0x03;
    
    // 7. Habilitar las interrupciones
    U0IER = 0x03;
    ISER0 |= (1 << 5);
    /*
        El registro U0IER habilita la generación de interrupciones en la UART
        El registro ISER0 habilita las interrupciones de la UART en el NVIC
    */
}
```

### Atención de interrupciones de la UART

En función de cómo se realice la configuración de la UART, puede haber diversas razones por la cual la UART puede generar interrupciones. Por lo tanto, dentro del handler de las interrupciones de la UART se debe analizar el registro IIR (Interrupt Identification Register).

El registro IIR se resetea cuando se lo lee, por lo tanto, es necesario almacenar su valor para identificar la/s causas de la interrupción producida por la UART, ya que puede ser más de una al mismo tiempo.

- Si la causa de la interrupción fue que _la transmición en curso finalizó_ entonces podemos enviar el próximo dato a transmitir que se encuentra en el buffer de transmición.
- Si la causa de la interrupción fue que _la recepción en curso finalizó_ entonces podemos cargar el dato recibido en el buffer de recepción.
- Si la causa de la interrupción fue algún error, entonces deberemos actuar acorde a la política establecida para tratamiento de errores.

```c
void UART0_IRQHandler (void) {
    uint8_t iir, dato;
    int aux;
    do {
        // Este registro se resetea cuando es leído
        iir = U0IIR;

        if (iir & 0x02) {
            // La condición se cumple cuando terminó la transmición en curso (THR disponible)
            aux = popTx();              // retiramos del buffer de transmición el próximo dato a ser enviado
            if (aux > 0) U0THR = aux;   // si había un dato, lo cargamos en el registro de transmición
            else TxStart = 0;           // si no había un dato, indicamos que la transmición activa finallizó (THR queda disponible)
        } else if (iir & 0x04)  {
            // La condición se cumple cuando se recibió un dato (Data Ready)
            dato = U0RBR;               // cargo en el registro auxiliar el dato recibido por la UART
            pushRx(dato);               // envío el dato recibido al buffer de recepción
        } else if (iir & 0x06)  {
            // La condición se cumple cuando se produjo algún error
            /*
                Analizar el registro U0LSR para determinar qué error se produjo
                LSR[1]: (OE) Overrun Error
                LSR[2]: (PE) Parity Error
                LSR[3]: (FE) Framing Error
                LSR[4]: (BI) Break Interrupt
            */
        }
    } while (!(irr & 0x01));
}
```

### Aplicación de la UART en el modelo de capas

Para este ejemplo, considero que tengo una aplicación que es capaz de comunicarse con otro dispositivo a través de comunicación serie. Para manejar la comunicación, voy a necesitar distintas funciones y buffers en mi modelo de capas:
- Capa de la aplicación:
  - `transmitir(dato)` es la función que voy a llamar para transmitir un determinado dato o mensaje.
  - `leer()` es la función que voy a llamar para leer los datos que haya recibido.
- Capa de primitivas:
  - `pushTx()` es la función que voy a utilizar para enviar los datos que quiero transmitir al buffer de transmición.
  - `popRx()` es la función que voy a utilizar para tomar los datos que quiero leer del buffer de recepción
- Capa de buffers:
  - buffer de transmición: Cuando se carga el buffer, se dispara una interrupción que llama al handler de la UART
  - buffer de recepción: Cuando se quiere leer un dato recibido, popRx accede a este buffer.
- Capa de drivers:
  - `popTx()` es la función que se llama dentro del handler de la UART y toma un dato del buffer de transmición para ponerlo en el registro de transmición de la UART y comenzar la transmición serial.
  - `pushRx()` es la función utilizada por el handler de la UART cuando detecta que se recibió un dato para cargarlo en el buffer de recepción.
