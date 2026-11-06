#include "stm32f401re_i2c.h"

void I2C_Init(I2C_Handle *handle)
{
	handle->I2CX->CR2 |= 16U;
	handle->I2CX->CCR |= 16000000U / (2 * 100000U);
	handle->I2CX->TRISE |= 17U;
	handle->I2CX->CR1 |= 0x1U;
	handle->I2CX->CR1 |= 0x1U << 10;
}

I2C_Error I2C_Transfer_Pol(I2C_Handle *handle, uint8_t *dataPtr, uint32_t dataLength, uint8_t trgAddr)
{
	uint32_t temp;

	handle->dataLength = dataLength;
	handle->dataPtr = dataPtr;
	handle->trgAddr = trgAddr;

	handle->I2CX->CR1 |= 0x1U << 8; //START

	while(!(handle->I2CX->SR1 & 0x1U)); //Check SB
	handle->I2CX->DR = trgAddr << 1; //Send target address

	while(!((handle->I2CX->SR1 >> 1) & 0x1U)) //Check ADDR
	{
		if((handle->I2CX->SR1 >> 10) & 0x1U) //Check AF
		{
			handle->I2CX->CR1 |= 0x1U << 9; //STOP
			handle->I2CX->SR1 &= ~(1U << 10); //Clear AF
			return I2C_Error_NACK;
		}
	}
	temp = handle->I2CX->SR2; //Clear ADDR
	(void)temp;

	while(!((handle->I2CX->SR1 >> 7) & 0x1U)) //Check TxE
	{
		if((handle->I2CX->SR1 >> 10) & 0x1U) //Check AF
		{
			handle->I2CX->CR1 |= 0x1U << 9; //STOP
			handle->I2CX->SR1 &= ~(1U << 10); //Clear AF
			return I2C_Error_NACK;
		}
	}
	handle->I2CX->DR = *dataPtr; //Send data
	dataPtr++;
	dataLength--;

	while(dataLength > 0)
	{
		while(!((handle->I2CX->SR1 >> 7) & 0x1U)) //Check TxE
		{
			if((handle->I2CX->SR1 >> 10) & 0x1U) //Check AF
			{
				handle->I2CX->CR1 |= 0x1U << 9; //STOP
				handle->I2CX->SR1 &= ~(1U << 10); //Clear AF
				return I2C_Error_NACK;
			}
		}
		handle->I2CX->DR = *dataPtr;
		dataPtr++;
		dataLength--;
	}

	while(!((handle->I2CX->SR1 >> 2) & 	0x1U)); //Check BTF
	handle->I2CX->CR1 |= 0x1U << 9; //STOP
	return I2C_Error_No;
}

I2C_State I2C_Start_Transfer_It(I2C_Handle *handle, uint8_t *dataPtr, uint32_t dataLength, uint8_t trgAddr, I2C_IRQ_Number EV, I2C_IRQ_Number ER)
{
	uint32_t IPRX, IPRBit, ISERX, ISERBit;

	if(handle->state == I2C_State_Free)
	{
		handle->state = I2C_State_TXBusy;

		handle->dataPtr = dataPtr;
		handle->dataLength = dataLength;
		handle->trgAddr = trgAddr;

		handle->I2CX->CR2 |= 0x7U << 8; //Enable peripheral side interrupts

		IPRX = EV / 4; //IPR number
		IPRBit = EV % 4; //Corresponding bit
		NVIC->IPR[IPRX] &= ~(0xFFU << (8 * IPRBit)); //Clear field
		NVIC->IPR[IPRX] |= handle->config->priority << ((8 * IPRBit) + 4); //Set priority

		IPRX = ER / 4; //IPR number
		IPRBit = ER % 4; //Corresponding bit
		NVIC->IPR[IPRX] &= ~(0xFFU << (8 * IPRBit)); //Clear field
		NVIC->IPR[IPRX] |= handle->config->priority << ((8 * IPRBit) + 4); //Set priority

		ISERX = EV / 32; //ICER number
		ISERBit = EV % 32; //Corresponding bit
		NVIC->ISER[ISERX] |= 0x1U << ISERBit; //Enable interrupt

		ISERX = ER / 32; //ICER number
		ISERBit = ER % 32; //Corresponding bit
		NVIC->ISER[ISERX] |= 0x1U << ISERBit; //Enable interrupt

		handle->I2CX->CR1 |= 0x1U << 8; //START

		return (handle->state);
	}

	return (handle->state);
}

void I2C_IRQ_EV_Handle(I2C_Handle *handle, I2C_IRQ_Number EV, I2C_IRQ_Number ER)
{
	uint32_t temp;
	uint8_t ISERX, ISERBit;

	if(handle->I2CX->SR1 & 0x1U) //SR handler
	{
		handle->I2CX->DR = handle->trgAddr << 1; //Send target address
	}

	if((handle->I2CX->SR1 >> 1) & 0x1U) //ADDR handler
	{
		temp = handle->I2CX->SR2; //Clear ADDR
		(void)temp;
	}

	if((handle->I2CX->SR1 >> 7) & 0x1U) //TxE  handler
	{
		if(handle->dataLength > 0)
		{
			handle->I2CX->DR = *(handle->dataPtr);
			handle->dataPtr++;
			handle->dataLength--;
		}
	}

	if(((handle->I2CX->SR1 >> 2) & 0x1U) && (handle->dataLength <= 0))
	{
		handle->I2CX->CR1 |= 0x1U << 9; //STOP

		handle->I2CX->CR2 &= ~(((0x1U << 3) - 1) << 8); //Disable peripheral side interrupts

		ISERX = ER / 32; //ICER number
		ISERBit = ER % 32; //Corresponding bit
		NVIC->ICER[ISERX] |= 0x1U << ISERBit; //Disable interrupt

		ISERX = EV / 32; //ICER number
		ISERBit = EV % 32; //Corresponding bit
		NVIC->ICER[ISERX] |= 0x1U << ISERBit; //Disable interrupt

		handle->state = I2C_State_Free;
	}
}

void I2C_IRQ_ER_Handle(I2C_Handle *handle, I2C_IRQ_Number EV, I2C_IRQ_Number ER)
{
	uint8_t ISERX, ISERBit;

	if((handle->I2CX->SR1 >> 10) & 0x1U) //NACK handler
	{
		handle->I2CX->CR1 |= 0x1U << 9; //STOP
		handle->I2CX->SR1 &= ~(0x1U << 10); //Clear AF

		handle->I2CX->CR2 &= ~(((0x1U << 3) - 1) << 8); //Disable peripheral side interrupts

		ISERX = ER / 32; //ICER number
		ISERBit = ER % 32; //Corresponding bit
		NVIC->ICER[ISERX] |= 0x1U << ISERBit; //Disable interrupt

		ISERX = EV / 32; //ICER number
		ISERBit = EV % 32; //Corresponding bit
		NVIC->ICER[ISERX] |= 0x1U << ISERBit; //Disable interrupt

		handle->state = I2C_State_Free;
	}
}
