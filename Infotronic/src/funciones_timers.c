#include "inc/funciones.h"

/*
  Inicializa los timers del sistema
*/
void timersInit( void ){
  for(uint8_t i = 0; i < N_TIMERS; i++ ){
      tmrHandlers[i] = NULL;
      tmrRun[i] = 0;
      tmrState[i] = PAUSED;
      tmrEvents[i] = 0;
  }
}

/*
  Recorre el array de timers. Si alguno venció, lo indica en el registro de eventos
  - Esta función debería ejecutarse dentro del sysTickHandler()
*/
void analizeTimers(void) {
    for (int i = 0; i < N_TIMERS; i++) {
        if (tmrRun[i]) {
            // si tmrRun[i] (el valor de timerRun para el timer_i) es distinto de 0, se decrementa
            tmrRun[i]--;
            // si al decrementarlo se llega a 0, se activa el evento en el array de eventos
            if(!tmrRun[i]) tmrEvents[i] = 1;
        }
    }
}

/*
  - Monitorea los flags de los timers en el array tmrEvents
  - Cuando detecta que un evento está seteado, lo resetea para dar a entender que reconoció el evento
  - Luego ejecuta la acción correspondiente a ese determinado evento
  - Se utiliza dentro del while(1) del main para revisar los eventos en cada pasada
*/
void timerEvents(void) {
    for (uint8_t i = 0; i < N_TIMERS; i++) {
        if (tmrEvents[i] == 1) {
            // Reseteo el evento si este se encontraba activo
            tmrEvents[i] = 0;
            // Ejecuto la función del timer
            (*tmrHandlers[i])();
        }
    }
}

/*
  Configura y arranca un timer de los 32 disponibles
  - event: Nº de evento entre 0 y 31
  - time: tiempo del evento (depende de la base)
  - handler: rutina que atiende al evento en su vencimiento
  - base: base de tiempo elegida (DEC = decimas, SEG = segundos, MIN = minutos)
*/
void timerStart(uint8_t event, uint32_t time, timerHandler handler , uint32_t base) {
    tmrHandlers[event] = handler;   // determino la función a ejecutar cuando venza el timer
    tmrRun[event] = time * base;    // cuenta del timer en función de la base y el tiempo
    tmrState[event] = RUNNING;      // estado inicial luego de iniciar el timer (corriendo)
    tmrEvents[event] = 0;           // al iniciar el timer no hay evento
    tmrBase[event] = base;          // configuro la base con la que trabajará el timer
}

/*
  Reinicializa el timer del evento en función un los valores de tiempo y base. (No lo resetea)
  - event: Nº de evento entre 0 y 31
  - time: tiempo del evento (depende de la base)
*/
void setTimer(uint8_t event, uint32_t time) {
    tmrRun[event] = tmrBase[event] * time;  // configuro una nueva cuenta para el timer
    tmrState[event] = RUNNING;              // establezco que el estado del timer es corriendo
}

/*
  Lee el valor en el momento del timer del evento seleccionado
  - event: Nº de evento entre 0 y 31
*/
uint32_t getTimer(uint8_t event) {
    return tmrRun[event];
}

/*
  Detiene o arranca un determinado timer. Lo pasa de estado RUN a PAUSED o viceversa. (No lo resetea)
  - event: Nº de evento entre 0 y 31
  - new_state: estado en el que se quiere poner el timer (RUN/PAUSED)
*/
void standByTimer(uint8_t event , uint8_t new_state) {
    tmrState[event] = new_state;
}

/*
  Detiene un determinado timer
  - event: Nº de evento entre 0 y 31
*/
void timerStop(uint8_t event) {
    tmrHandlers[event] = NULL;
    tmrRun[event] = 0;
    tmrState[event] = PAUSED;
    tmrEvents[event] = 0;
}

/*
  Detiene todos los timers
*/ 
void timerCloseAll(void) {
    for (uint8_t i = 0; i < N_TIMERS; i++) timerStop(i);
}
