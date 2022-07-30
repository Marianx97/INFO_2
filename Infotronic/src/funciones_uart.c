#include "inc/funciones.h"

/*
  Inicialización de la UART
*/

void initUART0 (void) {
  // 1. Prender la UART
  PCONP |= 0x01 << 3;

  // 2. Determinar qué señal de clk se le envía a la UART
  PCLKSEL0 &= ~(0x03 << 6);

  // 3. Configuración de la UART a través del registro LCR
  U0LCR = 0x83; // "0x83"|16 = "10000011"|2

  // 4. Cargar los valores de DLL y DLM para determinar el BaudRate
  U0DLM = 0x0A;   // MSB: 10
  U0DLL = 0x2C;   // LSB: 44
  
  // 5. Configurar los pines para utilizar como las líneas Rx y Tx
  SetPinSel(P0, 2, PINSEL_FUNC1);
  SetPinSel(P0, 3, PINSEL_FUNC1);

  // Configurar el bit DLAB del registro LCR en 0 para tener acceso a los registros IER, RBR y THR
  U0LCR = 0x03;
  
  // 7. Habilitar las interrupciones
  U0IER = 0x03;
  ISER0 |= (1 << 5);
}

/*
  Handler para las interrupciones producidas por la UART0
*/
void UART0_IRQHandler (void) {
  uint8_t iir, dato, aux;

  do {
    // Este registro se resetea cuando es leído
    iir = U0IIR;

    if (iir & 0x02) {
      // La condición se cumple cuando se detecta un dato para transmitir (THR disponible)
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
  } while (!(iir & 0x01)); // puede ocurrir que lleguen 2 interrupciones a la vez
}

// Capa de drivers //

/*
  Esta función nos permite cargar los datos recibidos por la UART en el buffer para que luego sean retirados por PopRx
  - dato: el dato que se quiere cargar en el buffer de recepción
*/
void pushRx (uint8_t dato) {
  bufferRx[indexRxIN] = dato;       // Cargo el dato recibido por la UART al buffer de recepción
  indexRxIN++;                      // Incremento el índice del buffer de recepción
  indexRxIN %= UART_RX_BUFF_SIZE;   // Ajusto el índice si llegó al tope del buffer (buffer circular)
}

/*
  Esta función nos permite retirar del buffer de transmición los datos que fueron cargados para transmitirlos por la UART
*/
uint8_t popTx (void) {
  uint8_t aux = -1;                   // Seteamos el valor de retorno como error para el caso en que no haya dato para retirar del buffer
  if (indexTxIN != indexTxOUT) {      // Si los índices difieren, entonces hay un dato para retirar
    aux = bufferTx[indexTxOUT];       // Cargamos el valor de retorno con el dato a retirar
    indexTxOUT++;                     // Incrementamos el índice
    indexTxOUT %= UART_TX_BUFF_SIZE;  // Ajusto el índice si llegó al tope del buffer (buffer circular)
  }
  return aux;
}

// Capa de primitivas //

/*
  Esta función nos permite enviar un dato al buffer de transmición para que luego sea transmitido a través de la UART
  - dato: el dato que se quiere cargar en el buffer de transmición
*/
void pushTx (uint8_t dato) {
  bufferTx[indexTxIN] = dato;         // Cargo el dato que quiero enviar por la UART al buffer de transmición
  indexTxIN++;                        // Incremento el índice del buffer de transmición
  indexTxIN %= UART_TX_BUFF_SIZE;     // Ajusto el índice si llegó al tope del buffer (buffer circular)

  if (TxStart == 0) {
    TxStart = 1;                      // Indico que comienza la transmición
    U0THR = bufferTx[indexTxOUT];     // Cargo el dato que quiero transmitir del buffer de transmición al registro de la UART
  }
}

/*
  Esta función nos permite obtener un dato que fue recibido por la UART y que está almacenado en el buffer de recepción
*/

uint8_t popRx(void) {
  uint8_t dato = -1;                   // Seteamos el valor de retorno como error para el caso en que no haya dato para retirar del buffer
  if (indexRxIN != indexRxOUT) {      // Si los índices difieren, entonces hay un dato para retirar
    dato = bufferRx[indexRxOUT];       // Cargamos el valor de retorno con el dato a retirar
    indexRxOUT++;                     // Incrementamos el índice
    indexRxOUT %= UART_RX_BUFF_SIZE;  // Ajusto el índice si llegó al tope del buffer (buffer circular)
  }
  return dato;
}
