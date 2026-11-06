#include "stm32f401re_i2c.h"

void I2C_Init(I2C_Handle *handle)
{
	handle->I2CX->CR2 |= 16U;
	handle->I2CX->CCR |= 16000000U / (2 * 100000U);
	handle->I2CX->TRISE |= 17U;
	handle->I2CX->CR1 |= 0x1U;
	handle->I2CX->CR1 |= 0x1U << 10;
}

void I2C_Transfer_Pol(I2C_Handle *handle, uint8_t *dataPtr, uint32_t dataLength, uint8_t trgAddr)
{
	uint32_t temp;

	handle->dataLength = dataLength;
	handle->dataPtr = dataPtr;
	handle->trgAddr = trgAddr;

	handle->I2CX->CR1 |= 0x1U << 8; //START
	while(!(handle->I2CX->SR1 & 0x1U)); //Check SB
	handle->I2CX->DR = trgAddr << 1; //Send target address
	while(!((handle->I2CX->SR1 >> 1) & 0x1U)); //Check ADDR
	temp = handle->I2CX->SR2; //Clear ADDR
	(void)temp;
	while(!((handle->I2CX->SR1 >> 7) & 0x1U)); //Check TxE
	handle->I2CX->DR = *dataPtr; //Send data
	dataPtr++;
	dataLength--;

	while(dataLength > 0)
	{
		while(!((handle->I2CX->SR1 >> 7) & 0x1U)); //Check TxE
		handle->I2CX->DR = *dataPtr;
		dataPtr++;
		dataLength--;
	}

	while(!((handle->I2CX->SR1 >> 2) & 	0x1U)); //Check BTF
	handle->I2CX->CR1 |= 0x1U << 9; //STOP
}
