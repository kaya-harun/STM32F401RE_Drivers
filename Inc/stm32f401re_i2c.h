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
 * @defgroup I2C_Typedefs I2C typedefs
 *@{
 */
typedef uint32_t I2C_Priority;
/**
 * @}
 */

/**
 * @brief I2C peripheral errors.
 */
typedef enum
{
	I2C_Error_No,
	I2C_Error_NACK
}I2C_Error;

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
 * @brief GPIO IRQ numbers.
 */
typedef enum
{
	I2C_IRQ_EV1 = 0x1F,
	I2C_IRQ_ER1 = 0x20,
	I2C_IRQ_EV2 = 0x21,
	I2C_IRQ_ER2 = 0x22,
	I2C_IRQ_EV3 = 0x48,
	I2C_IRQ_ER3 = 0x49,
}I2C_IRQ_Number;

/**
 * @brief I2C peripheral configuration structure.
 */
typedef struct I2C_Conf I2C_Conf;
struct I2C_Conf
{
	I2C_Priority priority;			/**< I2C 4-bit priority */
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

/**
 * @brief I2C polling transfer API.
 *
 * @param handle 		Handle pointer to the I2C peripheral to be used
 */
I2C_Error I2C_Transfer_Pol(I2C_Handle *handle, uint8_t *dataPtr, uint32_t dataLength, uint8_t trgAddr);

/**
 * @brief I2C interrupt transfer start API.
 *
 * @param handle 		Handle pointer to the I2C peripheral to be used
 */
I2C_State I2C_Start_Transfer_It(I2C_Handle *handle, uint8_t *dataPtr, uint32_t dataLength, uint8_t trgAddr, I2C_IRQ_Number EV, I2C_IRQ_Number ER);

/**
 * @brief I2C interrupt handler API.
 *
 * @param handle		Handle pointer to the I2C peripheral to be used
 */
void I2C_IRQ_EV_Handle(I2C_Handle *handle, I2C_IRQ_Number EV, I2C_IRQ_Number ER);

/**
 * @brief I2C interrupt error handler API.
 *
 * @param handle		Handle pointer to the I2C peripheral to be used
 */
void I2C_IRQ_ER_Handle(I2C_Handle *handle, I2C_IRQ_Number EV, I2C_IRQ_Number ER);


#endif /* INC_STM32F401RE_I2C_H_ */
