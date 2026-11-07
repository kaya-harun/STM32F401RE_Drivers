#include "stm32f401re_uart.h"

void UART_Init(UART_Handle *handle)
{
	uint32_t mantissa, fraction, reminder, denominator;
	uint8_t samplingFreq;
	handle->UARTX->CR1 &= ~(0x1U << 12); //Clear field
	handle->UARTX->CR1 |= (handle->config->m); //Set m bit

	handle->UARTX->CR2 &= ~(0x3U << 12); //Clear field
	handle->UARTX->CR2 |= handle->config->stop; //Set stop bits

	handle->UARTX->CR3 &= ~(0x1U << 11); //Clear field
	handle->UARTX->CR3 |= handle->config->oneBit; //Set onebit bit

	handle->UARTX->CR1 &= ~(0x1U << 15); //Clear field
	handle->UARTX->CR1 |= handle->config->over; //Set over bit

	//Calculate UARTDIV
	samplingFreq = handle->config->over ? 8 : 16;
	denominator = (samplingFreq * handle->config->baudRate);
	mantissa = 16000000 / denominator;
	reminder = 16000000 % denominator;
	fraction = ((reminder * samplingFreq) + (denominator / 2)) / denominator;

	if(fraction >= samplingFreq)
	{
		mantissa++;
		fraction = 0;
	}

	handle->UARTX->BRR &= ~((0x1U << 16) - 1); //Clear field
	handle->UARTX->BRR |= (mantissa << 4) | (fraction); //Set baud rate
}
