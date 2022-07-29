#include "inc/funciones.h"

// El led 0 se encuentra en el pin 0 del puerto 2
#define LED_0 2,0

/*
    Funcion para inicializar el módulo SysTick del microcontrolador
*/
void sysTickInit (void) {
    STRELOAD = (RELOAD_VAL);    // Valor que determina cada cuánto se vence el timer
    STCURR = 0;                 // Al escribir cualquier valor sobre este registro, se resetea la cuenta
    ENABLE = 1;                 // Corresponde al bit Enable del registro STCTRL y determina que el systick está activo
    TICKINT = 1;                // Corresponde al bit Tickint del registro STCTRL y determina que están activas las interrupciones por timer
    CLKSOURCE = 1;              // Corresponde al bit ClkSource del registro STCTRL y determina que se utilizará la señal de clk interna
}

/*
    Handler de la interrupción producida por el systick.
    Aquí se determina qué se debe hacer en el caso de que se venza el timer.
*/
void sysTickHandler (void) {
    // Planteo de un handler genérico y cómo manejarlo

    __RW uint32_t dummy;

    dummy = STCTRL; // Limpia flag de int

    static __RW uint8_t contTimeout = 100;
    /*
        Uso esta variable para contabilizar la cantidad de veces que vence el contador.
        De esta manera podemos controlar el tiempo para que sucedan eventos
        en función del tiempo de interrupción y la cantidad de veces que ocurre la interrupción
    */

    contTimeout--;
    if (!contTimeout) {
        // Cuando el contador alcanza 0, se cumplió el tiempo del evento que configuramos
        contTimeout = 100; // Si el evento es recurrente, se vuelve a cargar la variable del contador

        /*
            Se realiza la tarea que corresponda.
            Por ejemplo: cambiar el estado de un Led (toggle) para hacerlo parpadear con determinada frecuencia
        */
        if (!getPin(LED_0)) setPin(LED_0, 1); // getPin retorna el estado del pin, si es 0, al negarlo entra en la condición y lo setea en 1
        else setPin(LED_0, 0);                // si no es 0, entonces es 1, y por ende lo ponemos en 0
    }
}
