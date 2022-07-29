#include "inc/funciones.h"

/*
    Determina el modo de configuración de un pin de un puerto.
    - port: Nº del puerto a configurar
    - pin: Nº del pin a configurar
    - mode: modo en el que se lo configura. [#0: GPIO, #1: func_1, #2: func_2, #3: func_3]
*/
void setPinSel(uint8_t port, uint8_t pin, uint8_t mode) {
    if (pin < 16) {
        // Máscara 1 (Limpieza): Se realiza una AND con 00 en los bits que corresponden al pin que se quiere configurar
        PINSEL[port * 2] &= ~(3 << (pin*2));
        // Máscara 2 (Configuracion): Se realiza una OR con el modo en los bits que corresponden al pin que se quiere configurar
        PINSEL[port * 2] |= mode << (pin*2);
    } else {
        // En caso de que el pin sea mayor o igual al Nº 16, se le resta 16 y se ajusta el Nº de registro PINSEL
        pin -= 16;
        PINSEL[port * 2 + 1] &= ~(3 << (pin*2));    // Máscara 1
        PINSEL[port * 2 + 1] |= mode << (pin*2);    // Máscara 2
    }
}

/*
    Determina si un pin configurado como GPIO es entrada o salida.
    Hace uso de los registros FIODIR en función de su dirección respecto de GPIO_BASE
    - port: Nº del puerto a configurar
    - pin: Nº del pin a configurar
    - type: entrada o salida
*/
void SetDir(uint8_t port, uint8_t pin, uint8_t type) {
    if (type == GPIO_IN) GPIO_BASE[8*port] &= ~(1 << pin);
    else GPIO_BASE[8*port] |= 1 << pin;
}

/*
    Determina el modo para un pin configurado como entrada.
    - port: Nº del puerto a configurar
    - pin: Nº del pin a configurar
    - mode: modo de funcionamiento de la entrada [#0: PULL-UP, #1: REPEATER, #2: ni PULL-UP ni PULL-DOWN, #3: PULL-DOWN]
*/
void SetPinMode(uint8_t port, uint8_t pin, uint8_t mode) {
    if (pin < 16) {
        PINMODE[port * 2] &= ~(3 << (pin*2));   // Máscara 1
        PINMODE[port * 2] |= mode << (pin*2);   // Máscara 2
    } else {
        pin -= 16;
        PINMODE[port * 2 + 1] &= ~(3 << (pin*2)); // Máscara 1
        PINMODE[port * 2 + 1] |= mode << (pin*2); // Máscara 2
    }
}

/*
    Determina el modo para un pin configurado como salida.
    - port: Nº del puerto a configurar
    - pin: Nº del pin a configurar
    - mode: modo de funcionamiento de la salida [#0: modo normal, #1: modo open-drain]
*/
void setPinModeOD(uint8_t port, uint8_t pin, uint8_t mode) {
    if (mode == GPIO_OUT_NORMAL) PINMODE_OD[port] &= ~(1 << pin);
    else PINMODE_OD[port] |= 1 << pin;
}

/*
    Determina un valor específico para un pin de un puerto determinado.
    Hace uso de los registros FIOPIN en función de su dirección respecto de GPIO_BASE
    - port: Nº del puerto al que corresponde el pin que se quiere setear
    - pin: Nº del pin a setear
    - value: valor que se quiere setear en el pin
*/
void setPin(uint8_t port, uint8_t pin, uint8_t value) {
    if (value == 0) GPIO0_BASE[8*port + 5] &= ~(1 << pin);
    else GPIO_BASE[8*port + 5] |= 1 << pin;
}

/*
    Retorna el valor de un pin de un puerto determinado.
    Hace uso de los registros FIOPIN en función de su dirección respecto de GPIO_BASE
    - port: Nº del puerto al que corresponde el pin que se quiere leer
    - pin: Nº del pin a leer
*/
uint8_t getPin(uint8_t port, uint8_t pin) {
    return (GPIO_BASE[8*port + 5] >> pin & 1);
}
