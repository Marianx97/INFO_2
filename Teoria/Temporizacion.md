
# Resumen - Temporización:

- ¿Qué es un timer?
  * Se puede definir como un contador de ciclos de clk.
  * Clock (clk) es una señal periódica que está configurado a una determinada frecuencia.
  * El preescaler sirve como herramienta para dividir la frecuencia.

- La frecuencia de trabajo
  * Queda establecida a partir de la ejecución de la función `InicPLL()`
  * Una vez ejecutada, la frecuencia del clk del sistema queda configurada en $100MHz$

## El SysTick

* Es un timer simple, es decir un contador de pulsos de clk.
* Es parte del NVIC del Cortex M3

### Características

* Timer simple de 24 bits
* Cuenta descendente
* Genera una interrupción cuando la cuenta llega a 0 (si la interrupción está habilitada)
* Puede recibir la señal de clk de un bus o del pin 26 del puerto 3 cuando está configurado con su función 1 (STCLK)

### Registros del Systick

- STCTRL: Controla el funcionamiento del SysTick
  - [bit 0] ENABLE: cuando este bit está en 0 el contador está deshabilitado, en 1 está habilitado
  - [bit 1] TICKINT: cuando este bit está en 0 el contador no genera interrupciones, en 1 genera interrupciones
  - [bit 2] CLKSOURCE: cuando este bit está en 0 la fuente de clk es el pin STCLK, en 1 la fuente de clk es la CPU
  - [bits 3 a 15] RESERVED
  - [bit 16] COUNTFLAG: indica que se alcanzó la cuenta. Si está en 1 se alcanzó, en 0 aún no. Este bit se limpia al leerlo, aunque en la práctica "se limpia solo".
  - [bits 17 a 31] RESERVED
- STRELOAD: Se utiliza en la inicialización para establecer el valor de la cuenta
  - [bits 0 a 23] RELOAD: valor que se debe contar
  - [bits 24 a 31] RESERVED
- STCURR: Tiene la cuenta actual del timer
  - [bits 0 a 23] CURRENT: valor actual de la cuenta
  - [bits 24 a 31] RESERVED
- STCALIB: Un valor de cuenta por defecto que permite generar interrupciones cada $10ms$ si el clk es de $100MHz$.

## Timers

Los timers son herramientas de temporización que se pueden utilizar para configurar eventos que deben accionarse en función del tiempo, ya sea de manera periodica o en función de otro parámetro.
Estos mismos eventos _podrían_ ser configurados dentro del systick, pero puede ocurrir que se ejecuten en función de distíntas bases de tiempo. Eventos que se ejecuten cada décimas de segundos o eventos que se ejecuten una vez por día. Utilizando timers, se pueden configurar la atención de la ocurrencia de estos eventos sin llenar el handler del systick con muchísima lógica.

- Para su correcto funcionamiento, los timers deben inicializarse. `timersInit()`
- Dentro del `sysTickHandler()` se deberá llamar a la función `analizeTimers()` para analizar si alguno de los timers alcanzó la cuenta que tiene configurada y en caso de que haya ocurrido, indicar el evento correspondiente.
- Dentro del `while(1)` del programa general, se deberá llamar a la función `timerEvents()` que revisa el array de eventos de los timers y en caso de que encuentre uno seteado, lo resetea y ejecuta la función correspondiente.

Se cuenta con un set de funciones desarrolladas para configurar y controlar el funcionamiento de los handlers. Estas son:

```c
// Configura y arranca un timer de los 32 disponibles
// - event: Nº de evento entre 0 y 31
// - time: tiempo del evento (depende de la base)
// - handler: rutina que atiende al evento en su vencimiento
// - base: base de tiempo elegida (DEC = decimas, SEG = segundos, MIN = minutos)
void timerStart(uint8_t event, uint32_t time, timerHandler handler , uint32_t base);

// Reinicializa el timer del evento en función un los valores de tiempo y base. (No lo resetea)
// - event: Nº de evento entre 0 y 31
// - time: tiempo del evento (depende de la base)
void setTimer(uint8_t event, uint32_t time);

// Lee el valor en el momento del timer del evento seleccionado
// - event: Nº de evento entre 0 y 31
uint32_t getTimer(uint8_t event);

// Detiene o arranca un determinado timer. Lo pasa de estado RUNNING a PAUSED o viceversa. (No lo resetea)
// - event: Nº de evento entre 0 y 31
// - new_state: estado en el que se quiere poner el timer (RUNNING/PAUSED)
void standByTimer(uint8_t event , uint8_t new_state);

// Detiene un determinado timer
// - event: Nº de evento entre 0 y 31
void timerStop(uint8_t event);

// Detiene todos los timers 
void timerCloseAll(void)
```
