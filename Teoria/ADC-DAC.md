
# Conversores Analógico-Digital

Estos dispositivos son módulos que permiten transformar una señal analógica en un valor digital y viceversa. En cierta forma, son un elemento clave para comunicar cualquier circuito con el mundo real, ya que en este, la mayoría de las señales son analógicas, es decir que tienen valores continuos. Por ejemplo, las temperaturas que mide un sensor no pasan discretamente de un valor entero a otro, sino que aumentan por infinitos valores intermedios.

El ADC permite transformar los valores que mide el sensor en una señal digital utilizando una técnica de muestreo. Durante un periodo $T$, el sistema toma $n$ cantidad de muestras y luego interpreta esas muestras como una señal.

Por su parte, el DAC realiza el efecto opuesto, transformando una serie discreta de valores en una señal analógica continua. Mientras más muestras discretas haya, más "suave" será la señal construida.

## Configuración del ADC

1. Energizar
  - El bit 12 del registro PCONP corresponde al ADC

2. Seleccionar la fuente de clk
  - Los bits 24 y 25 del registro PCLKSEL0 corresponden a la fuente de clk del ADC (PCLK_ADC)
    - si PCLK_ADC = 0: fCLK_ADC = fCCLK/4
    - si PCLK_ADC = 1: fCLK_ADC = fCCLK
    - si PCLK_ADC = 2: fCLK_ADC = fCCLK/2
    - si PCLK_ADC = 3: fCLK_ADC = fCCLK/8
  - Los bits 8 al 15 del registro AD0CR corresponden al divisor
  - La frecuencia de muestreo del ADC se calcula como: $fADC = \frac{fCLK_ADC}{65 * (CLKDIV + 1)}$

3. Configurar los pines que se utilizarán como entradas analógicas

4. Configurar las interrupciones

5. Configurar el ADC

6. Habilitar el ADC

7. Disparar el ADC
