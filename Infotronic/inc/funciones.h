#include "Regs_LPC176x.h"

// *****************************************************
//           Funciones para manejar Pines
// *****************************************************

void setPinSel(uint8_t port, uint8_t pin, uint8_t mode);
void SetDir(uint8_t port, uint8_t pin, uint8_t type);
void SetPinMode(uint8_t port, uint8_t pin, uint8_t mode);
void setPinModeOD(uint8_t port, uint8_t pin, uint8_t mode);
void setPin(uint8_t port, uint8_t pin, uint8_t value);
uint8_t getPin(uint8_t port, uint8_t pin);


// *****************************************************
//         Funciones para manejar el systick
// *****************************************************

void sysTickInit (void);
void sysTickHandler (void);


// ******************************************************************
//          Definiciones, variables y funciones para manejar timers
// ******************************************************************

/* Definiciones */
#define N_TIMERS     32

#define RUNNING         ((uint8_t) 0)
#define PAUSED          ((uint8_t) 1)

#define DEC             ( (uint32_t) 40 )
#define SEG             ( (uint32_t) DEC * 10 )
#define MIN             ( (uint32_t) SEG * 60 )
#define HRS             ( (uint32_t) MIN * 60 )

typedef void (*timerHandler)(void);     // Definimos timerHandler como un tipo para punteros a funcion

#define NULL			(( void *) 0)

/* Variables globales */
/* Buffers de los timers */

extern volatile uint32_t tmrRun[N_TIMERS];
extern volatile uint8_t tmrState[N_TIMERS];
extern volatile uint8_t tmrEvents[N_TIMERS];
extern volatile uint8_t tmrBase[N_TIMERS];
extern volatile timerHandler tmrHandlers[N_TIMERS];

/* Funciones */

void timersInit(void);
void analizeTimers(void);
void timerEvents(void);
void timerStart(uint8_t event, uint32_t time, timerHandler handler , uint32_t base);
void setTimer(uint8_t event, uint32_t time);
uint32_t getTimer(uint8_t event);
void timerChangeState(uint8_t event , uint8_t new_state);
void timerStop(uint8_t event);
void timerClose(uint8_t event);
void timerCloseAll(void);


// *******************************************************************
//          Definiciones, variables y funciones para manejar la UART
// *******************************************************************

/* Variables globales */
extern volatile uint8_t TxStart;
extern volatile uint8_t bufferRx[UART_RX_BUFF_SIZE];
extern volatile uint8_t indexRxIN;
extern volatile uint8_t indexRxOUT;
extern volatile uint8_t indexTxIN;
extern volatile uint8_t indexTxOUT;
extern volatile uint8_t bufferTx[UART_TX_BUFF_SIZE];

/* Funciones */

void initUART0(void);
void UART0_IRQHandler(void);
void pushRx(uint8_t dato);
uint8_t popTx(void);
void pushTx(uint8_t dato);
uint8_t popRx(void);


// **********************************************************************
//          Definiciones, variables y funciones para manejar el Teclado
// **********************************************************************

/* Definiciones */
/* suponiendo un teclado de 4x4 */

#define KEY_0   0
#define KEY_1   1
#define KEY_2   2
#define KEY_3   3
#define KEY_4   4
#define KEY_5   5
#define KEY_6   6
#define KEY_7   7
#define KEY_8   8
#define KEY_9   9
#define KEY_10  10
#define KEY_11  11
#define KEY_12  12
#define KEY_13  13
#define KEY_14  14
#define KEY_15  15

#define NO_KEY  255

#define ON      0x01
#define OFF     0x00

// Las siguientes definiciones de filas y columnas son a fines didácticos
// desconozco si se puede implementar un teclado matricial con esta configuración

#define ROW0    P0,0
#define ROW1    P0,1
#define ROW2    P0,2
#define ROW3    P0,3

#define COL0    P0,4
#define COL1    P0,5
#define COL2    P0,6
#define COL3    P0,7

#define BOUNCE_LIMIT 1000

/* Variables globales */
extern volatile uint8_t keyBuffer;

/* Funciones */

uint8_t getKey(void);
uint8_t keyboardHW(void);
void keyboardSW(void);
