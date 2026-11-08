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

void UART_Transfer_Pol(UART_Handle *handle, uint8_t *dataPtr, uint32_t dataLength)
{
	handle->txDataPtr = dataPtr;
	handle->txDataLength = dataLength;

	handle->UARTX->CR1 |= 0x1U << 13; //Enable UART
	handle->UARTX->CR1 |= 0x1U << 3; //Send idle

	while(handle->txDataLength > 0)
	{
		while(!((handle->UARTX->SR >> 7) & 0x1U)); //Wait for TxE
		handle->UARTX->DR = *(handle->txDataPtr); //Send the data
		(handle->txDataPtr)++;
		(handle->txDataLength)--;
	}

	while(!((handle->UARTX->SR >> 6) & 0x1U)); //Wait for TC

	handle->UARTX->CR1 &= ~(0x1U << 3); //Close TE
	handle->UARTX->CR1 &= ~(0x1U << 13); //Disable UART
}

void UART_Receive_Pol(UART_Handle *handle, uint8_t *dataPtr, uint32_t dataLength)
{
	handle->rxDataPtr = dataPtr;
	handle->rxDataLength = dataLength;

	handle->UARTX->CR1 |= 0x1U << 13; //Enable UART
	handle->UARTX->CR1 |= 0x1U << 2; //Enable receiving

	while(handle->rxDataLength > 0)
	{
		while(!((handle->UARTX->SR >> 5) & 0x1U)); //Wait for RxE
		*(handle->rxDataPtr) = handle->UARTX->DR; //Read data
		(handle->rxDataPtr)++;
		(handle->rxDataLength)--;
	}

	handle->UARTX->CR1 &= ~(0x1U << 2); //Disable receiving
	handle->UARTX->CR1 &= ~(0x1U << 13); //Disable UART
}

UART_State UART_Start_Transfer_It(UART_Handle *handle, uint8_t *dataPtr, uint32_t dataLength, UART_IRQ_Number IRQNumber)
{
	uint32_t IPRX, IPRBit, ISERX, ISERBit;

	if(handle->txState == UART_STATE_FREE)
	{
		handle->txState = UART_STATE_TXBUST;

		handle->txDataPtr = dataPtr;
		handle->txDataLength = dataLength;

		IPRX = IRQNumber / 4; //IPR number
		IPRBit = IRQNumber % 4; //Corresponding bit
		NVIC->IPR[IPRX] &= ~(0xFFU << (8 * IPRBit)); //Clear field
		NVIC->IPR[IPRX] |= handle->config->priority << ((8 * IPRBit) + 4); //Set priority

		ISERX = IRQNumber / 32; //ICER number
		ISERBit = IRQNumber % 32; //Corresponding bit
		NVIC->ISER[ISERX] |= 0x1U << ISERBit; //Enable interrupt

		handle->UARTX->CR1 |= 0x1U << 13; //Enable UART
		handle->UARTX->CR1 |= 0x1U << 3; //Send idle

		handle->UARTX->CR1 |= 0x1U << 7; //Enable peripheral side interrupts
		handle->UARTX->CR1 |= 0x1U << 6;

		return (handle->txState);
	}

	return (handle->txState);
}

UART_State UART_Start_Receive_It(UART_Handle *handle, uint8_t *dataPtr, uint32_t dataLength, UART_IRQ_Number IRQNumber)
{
	uint32_t IPRX, IPRBit, ISERX, ISERBit;

	if(handle->rxState == UART_STATE_FREE)
	{
		handle->rxState = UART_STATE_RXBUSY;

		handle->rxDataPtr = dataPtr;
		handle->rxDataLength = dataLength;

		IPRX = IRQNumber / 4; //IPR number
		IPRBit = IRQNumber % 4; //Corresponding bit
		NVIC->IPR[IPRX] &= ~(0xFFU << (8 * IPRBit)); //Clear field
		NVIC->IPR[IPRX] |= handle->config->priority << ((8 * IPRBit) + 4); //Set priority

		ISERX = IRQNumber / 32; //ICER number
		ISERBit = IRQNumber % 32; //Corresponding bit
		NVIC->ISER[ISERX] |= 0x1U << ISERBit; //Enable interrupt

		handle->UARTX->CR1 |= 0x1U << 13; //Enable UART
		handle->UARTX->CR1 |= 0x1U << 2; //Enable receivinge

		handle->UARTX->CR1 |= 0x1U << 5; //Enable peripheral side interrupts

		return (handle->rxState);
	}

	return (handle->rxState);

}

void UART_IRQ_Handle(UART_Handle *handle, UART_IRQ_Number IRQNumber)
{
	uint32_t ISERX, ISERBit;

	if((handle->UARTX->SR >> 7) & 0x1U) //TxE handler
	{
		if(handle->txDataLength > 0)
		{
			handle->UARTX->DR = *(handle->txDataPtr); //Send the data
			(handle->txDataPtr)++;
			(handle->txDataLength)--;
		}
	}

	if(((handle->UARTX->SR >> 6) & 0x1U) && (handle->txDataLength <= 0)) //TC handler
	{
		handle->txState = UART_STATE_FREE;
		handle->UARTX->CR1 &= ~(0x1U << 7); //Disable peripheral side interrupts
		handle->UARTX->CR1 &= ~(0x1U << 6);
		handle->UARTX->CR1 &= ~(0x1U << 3); //Close TE

		if(handle->rxState == UART_STATE_FREE)
		{
			handle->UARTX->CR1 &= ~(0x1U << 13); //Disable UART

			ISERX = IRQNumber / 32; //ICER number
			ISERBit = IRQNumber % 32; //Corresponding bit
			NVIC->ICER[ISERX] |= 0x1U << ISERBit; //Disable interrupt
		}
	}

	if((handle->UARTX->SR >> 5) & 0x1U) //RxE handler
	{
		if(handle->rxDataLength > 0)
		{
			*(handle->rxDataPtr) = handle->UARTX->DR; //Read data
			(handle->rxDataPtr)++;
			(handle->rxDataLength)--;

			if(handle->rxDataLength <= 0)
			{
				handle->rxState = UART_STATE_FREE;
				handle->UARTX->CR1 &= ~(0x1U << 5); //Disable peripheral side interrupts
				handle->UARTX->CR1 &= ~(0x1U << 2); //Disable receiving

				if(handle->txState == UART_STATE_FREE)
				{
					handle->UARTX->CR1 &= ~(0x1U << 13); //Disable UART

					ISERX = IRQNumber / 32; //ICER number
					ISERBit = IRQNumber % 32; //Corresponding bit
					NVIC->ICER[ISERX] |= 0x1U << ISERBit; //Disable interrupt
				}
			}
		}

	}
}
