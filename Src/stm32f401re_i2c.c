#include "stm32f401re_i2c.h"

void I2C_Init(I2C_Handle *handle)
{
	handle->I2CX->CR2 |= 16U;
	handle->I2CX->CCR |= 16000000U / (2 * 100000U);
	handle->I2CX->TRISE |= 17U;
	handle->I2CX->CR1 |= 0x1U;
	handle->I2CX->CR1 |= 0x1U << 10;
}
