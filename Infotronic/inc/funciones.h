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

extern volatile uint8_t TxStart;
extern volatile uint8_t bufferRx[UART_RX_BUFF_SIZE];
extern volatile uint8_t indexRxIN;
extern volatile uint8_t indexRxOUT;
extern volatile uint8_t indexTxIN;
extern volatile uint8_t indexTxOUT;
extern volatile uint8_t bufferTx[UART_TX_BUFF_SIZE];

void initUART0(void);
void UART0_IRQHandler(void);
void pushRx(uint8_t dato);
uint8_t popTx(void);
void pushTx(uint8_t dato);
uint8_t popRx(void);
