/**
 * @file stm32f401re_uart.h
 * @brief UART peripheral header for STM32F401RE
 * @author Harun Kaya
 * @date 07.11.2026
 */

#ifndef INC_STM32F401RE_UART_H_
#define INC_STM32F401RE_UART_H_

#include "stm32f401re.h"

/**
 * @brief UART peripheral states.
 */
typedef enum
{
	UART_STATE_FREE,
	UART_STATE_TXBUST,
	UART_STATE_RXBUSY
}UART_State;

/**
 * @brief UART communication word lengths.
 */
typedef enum
{
	UART_M_8,
	UART_M_9
}UART_M;

/**
 * @brief UART communication stop bit lengths.
 */
typedef enum
{
	UART_STOP_1,
	UART_STOP_2 = 0x2U
}UART_Stop;

/**
 * @brief UART communication over sampling frequencies.
 */

typedef enum
{
	UART_OVER_16,
	UART_OVER_8
}UART_Over;

/**
 * @brief UART communication sampling methods.
 */
typedef enum
{
	UART_ONEBIT_3,
	UART_ONEBIT_1
}UART_OneBit;

/**
 * @brief UART communication baud rates.
 */
typedef enum
{
	UART_BAUD_9600 = 9600,
	UART_BAUD_115200 = 115200,
	UART_BAUD_230400 = 230400
}UART_BaudRate;

/**
 * @brief UART peripheral configuration structure.
 */
typedef struct UART_Conf UART_Conf;
struct UART_Conf
{
	UART_M m;
	UART_Stop stop;
	UART_Over over;
	UART_OneBit oneBit;
	UART_BaudRate baudRate;
};

/**
 * @brief UART peripheral handle structure.
 */
typedef struct UART_Handle UART_Handle;
struct UART_Handle
{
	UART_RegMap *UARTX;
	UART_Conf *config;
	uint8_t *dataPtr;
	uint32_t dataLength;
	UART_State state;
};

/**
 * @brief UART peripheral initialization API.
 *
 * @param handle 		Handle pointer to the UART peripheral to be used
 */
void UART_Init(UART_Handle *handle);
#endif /* INC_STM32F401RE_UART_H_ */
