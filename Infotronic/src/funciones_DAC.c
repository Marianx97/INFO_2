#include "inc/funciones.h"

/*
    Inicialización del módulo del DAC
*/
void initDAC(void) {
    // 1. Seleccionar la fuente de clk
    PCLKSEL0 &= ~(0x03<<22);

    // 2. Configurar pines I/O
    SetPINSEL(DAC_OUT, PINSEL_FUNC2);

    // 3. Cofigurar en modo de consumo mínimo
    DACR |= (0x01<<16);
}

/*
    Cargar el valor que se quiere transmitir en el DAC
*/
void setDAC(uint16_t value) {
    uint16_t aux = 0;
    aux = value >> 2;       // Ajustar el valor
    DACR = (aux << 6);      // Cargar el valor a enviar
    DACR |= (0x01 << 16);   // Cofigurar en modo de consumo mínimo
}
