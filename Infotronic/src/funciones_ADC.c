#include "inc/funciones.h"

/*
    Inicialización del módulo del ADC
*/
void initADC(void) {
    // 1. Energizar
    PCONP |= (0x01 << 12);
    
    // 2. Seleccionar la fuente de clk
    PCLKSEL0 &= ~(0x03 << 24); // PCLK = CCLK / 4 = 25 MHz
    AD0CR |= (0x01 << 8); // CLKDIV = 1
    //  =>  ADCLK = PCLK / (CLKDIV + 1) = 12.5 MHz

    // 3. Configurar pines I/O
    SetPINSEL(P0, 24, PINSEL_FUNC1);  // AD0.1

    // 4. Configurar interrupciones
    ISER0 |= (0x01 << 22); // Habilito las interrupciones por lectura lista en el NVIC
    AD0INTEN |= (0x01 << 1); // Habilito las interrupciones desde el ADC

    // 5. Configurar el ADC
    AD0CR |= (0x01 << 1); // Selecciono el canal 1 para samplear
    AD0CR |= (0x01 << 16); // Selecciono Modo Burst

    // 6. Activar el ADC
    AD0CR |= (0x01 << 21);

    // 7. Disparar el ADC
    AD0CR &= ~(0x07 << 24); // Start
}

/*
    Handler de interrupciones del ADC
*/
void ADC_IRQHandler (void) {
    uint32_t globalDataRegister;
    static uint8_t index = 0;

    do {
        globalDataRegister = AD0GDR; // El registro contiene la conversión más reciente

        // Desplazamos el valor del registro para quedarnos solo con el valor de conversión
        // lo guardamos en el buffer de muestras
        sampleBuffer[index] = ((globalDataRegister >> 4) & 0xfff);  
        
        // Aumentamos el valor del índice y corregimos el valor en caso de que se alcance el tope (buffer circular)
        index++;
        index %= MAX_SAMPLE_AMOUNT;
    } while(AD0GDR & (0x01 << 31));   // Mientras que haya una interrupcion
}