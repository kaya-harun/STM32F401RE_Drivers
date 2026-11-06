/**
 * @file stm32f401re_i2c.h
 * @brief I2C peripheral header for STM32F401RE
 * @author Harun Kaya
 * @date 06.11.2026
 */

#ifndef INC_STM32F401RE_I2C_H_
#define INC_STM32F401RE_I2C_H_

#include "stm32f401re.h"

/**
 * @brief I2C peripheral states.
 */
typedef enum
{
	I2C_State_Free,
	I2C_State_TXBusy,
	I2C_State_RXBust
}I2C_State;

/**
 * @brief I2C peripheral configuration structure.
 */
typedef struct I2C_Conf I2C_Conf;
struct I2C_Conf
{
};

/**
 * @brief I2C peripheral handle structure.
 */
typedef struct I2C_Handle I2C_Handle;
struct I2C_Handle
{
	I2C_RegMap *I2CX;		/**< I2C peripheral pointer */
	I2C_Conf *config;		/**< I2C configuration structure pointer */
	uint8_t trgAddr;		/**< I2C target address */
	uint8_t *dataPtr;		/**< DO NOT CONFIGURE */
	uint32_t dataLength;	/**< DO NOT CONFIGURE */
	I2C_State state;		/**< DO NOT CONFIGURE */
};

/**
 * @brief I2C peripheral initialization API.
 *
 * @param handle 		Handle pointer to the I2C peripheral to be used
 */
void I2C_Init(I2C_Handle *handle);

#endif /* INC_STM32F401RE_I2C_H_ */
