#include "inc/funciones.h"

/*
    Trae del buffer del teclado del código de la tecla pulsada
*/
uint8_t getKey(void) {
    uint8_t key;

    key = keyBuffer;
    keyBuffer = NO_KEY;

    return key;
}

/*
    Realiza el barrido de los puertos del teclado y retornará el valor de una tecla presionada.
    En caso de que no haya tecla presionada, retornará NO_KEY.
*/
uint8_t keyboardHW(void) {
    setPin(ROW0, OFF); setPin(ROW1, ON);  setPin(ROW2, ON);  setPin(ROW3, ON);
    setPin(ROW0, OFF); setPin(ROW1, ON);  setPin(ROW2, ON);  setPin(ROW3, ON);

    if (!getPin(COL0)) return KEY_0;
    if (!getPin(COL1)) return KEY_1;
    if (!getPin(COL2)) return KEY_2;
    if (!getPin(COL3)) return KEY_3;

    setPin(ROW0, ON);  setPin(ROW1, OFF); setPin(ROW2, ON);  setPin(ROW3, ON);
    setPin(ROW0, ON);  setPin(ROW1, OFF); setPin(ROW2, ON);  setPin(ROW3, ON);

    if (!getPin(COL0)) return KEY_4;
    if (!getPin(COL1)) return KEY_5;
    if (!getPin(COL2)) return KEY_6;
    if (!getPin(COL3)) return KEY_7;

    setPin(ROW0, ON);  setPin(ROW1, ON);  setPin(ROW2, OFF); setPin(ROW3, ON);
    setPin(ROW0, ON);  setPin(ROW1, ON);  setPin(ROW2, OFF); setPin(ROW3, ON);

    if (!getPin(COL0)) return KEY_8;
    if (!getPin(COL1)) return KEY_9;
    if (!getPin(COL2)) return KEY_10;
    if (!getPin(COL3)) return KEY_11;

    setPin(ROW0, ON);  setPin(ROW1, ON);  setPin(ROW2, ON);  setPin(ROW3, OFF);
    setPin(ROW0, ON);  setPin(ROW1, ON);  setPin(ROW2, ON);  setPin(ROW3, OFF);

    if (!getPin(COL0)) return KEY_12;
    if (!getPin(COL1)) return KEY_13;
    if (!getPin(COL2)) return KEY_14;
    if (!getPin(COL3)) return KEY_15;

    return NO_KEY;
}

/*
    Valida que una tecla haya sido realmente presionada implementando una rutina de antirebote.
*/
void keyboardSW(void) {
    static uint8_t lastCode = NO_KEY;
    static uint8_t stableStates = 0;
    uint8_t currentCode;

    currentCode = keyboardHW();

    // No hay tecla pulsada o hubo rebote
    if (currentCode == NO_KEY) { 
        lastCode = NO_KEY;
        stableStates = 0;
        return;
    }

    // Detecta la tecla por 1ra vez
    if (stableStates == 0) {
        lastCode = currentCode;
        stableStates++;
        return;
    }

    // Valido como correcta la tecla
    if (stableStates == BOUNCE_LIMIT) {
        stableStates++;
        keyBuffer = lastCode;
        return;
    }

    // La tecla sigue presionada
    if (stableStates > BOUNCE_LIMIT) return;

    if (currentCode == lastCode) stableStates++;    // validando la tecla
    else stableStates = 0;                          // se soltó la tecla

    return;
}
