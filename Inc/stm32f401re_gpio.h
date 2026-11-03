/**
 * @file stm32f401re_gpio.h
 * @brief GPIO peripheral header for STM32F401RE
 * @author Harun Kaya
 * @date 03.11.2026
 */

#ifndef INC_STM32F401RE_GPIO_H_
#define INC_STM32F401RE_GPIO_H_

#include "stm32f401re.h"

/**
 * @defgroup GPIO_Typedefs GPIO typedefs
 *@{
 */
typedef uint8_t GPIO_Priority;
/**
 * @}
 */

/**
 * @brief GPIO pin numbers.
 */
typedef enum
{
	GPIO_PIN_0,
	GPIO_PIN_1,
	GPIO_PIN_2,
	GPIO_PIN_3,
	GPIO_PIN_4,
	GPIO_PIN_5,
	GPIO_PIN_6,
	GPIO_PIN_7,
	GPIO_PIN_8,
	GPIO_PIN_9,
	GPIO_PIN_10,
	GPIO_PIN_11,
	GPIO_PIN_12,
	GPIO_PIN_13,
	GPIO_PIN_14,
	GPIO_PIN_15
}GPIO_Pin_Number;

/**
 * @brief GPIO pin modes.
 */
typedef enum
{
	GPIO_MODE_IN,
	GPIO_MODE_OUT,
	GPIO_MODE_AF,
	GPIO_MODE_IT_RT,
	GPIO_MODE_IT_FT,
	GPIO_MODE_IT_RFT
}GPIO_Mode;

/**
 * @brief GPIO output types.
 */
typedef enum
{
	GPIO_OTYPE_PP,
	GPIO_OTYPE_OD
}GPIO_OType;

/**
 * @brief GPIO pull-up / pull-down configurations.
 */
typedef enum
{
	GPIO_PUPD_NONE,
	GPIO_PUPD_PU,
	GPIO_PUPD_PD
}GPIO_PuPd;

/**
 * @brief GPIO alternate function numbers.
 */
typedef enum
{
	GPIO_AF_0,
	GPIO_AF_1,
	GPIO_AF_2,
	GPIO_AF_3,
	GPIO_AF_4,
	GPIO_AF_5,
	GPIO_AF_6,
	GPIO_AF_7,
	GPIO_AF_8,
	GPIO_AF_9,
	GPIO_AF_10,
	GPIO_AF_11,
	GPIO_AF_12,
	GPIO_AF_13,
	GPIO_AF_14
}GPIO_AF;

/**
 * @brief GPIO IRQ numbers.
 */
typedef enum
{
	GPIO_IRQ_EXTI0 = 0x6U,
	GPIO_IRQ_EXTI1 = 0x7U,
	GPIO_IRQ_EXTI2 = 0x8U,
	GPIO_IRQ_EXTI3 = 0x9U,
	GPIO_IRQ_EXTI4 = 0xAU,
	GPIO_IRQ_EXTI9_5 = 0x17U,
	GPIO_IRQ_EXTI15_10 = 0x28U
}GPIO_IRQ_Number;

/**
 * @brief GPIO pin states.
 */
typedef enum
{
	GPIO_Pin_Low,
	GPIO_Pin_High
}GPIO_Pin_State;

/**
 * @brief GPIO pin configuration structure.
 */
typedef struct GPIO_Conf GPIO_Conf;
struct GPIO_Conf
{
	GPIO_Pin_Number pinNumber;	/**< GPIO pin number */
	GPIO_Mode mode;				/**< GPIO pin mode */
	GPIO_OType oType; 			/**< GPIO pin output type */
	GPIO_PuPd puPd;				/**< GPIO pin pull-up/pull-down */
	GPIO_AF aF;					/**< GPIO pin alternate function */
	GPIO_IRQ_Number IRQNumber;	/**< GPIO pin IRQ number */
	GPIO_Priority priority;		/**< GPIO pin 4-bit priority*/
};

/**
 * @brief GPIO pin initialization API.
 *
 * @param GPIOX 		Pointer to the port to which the pin belongs
 * @param config 		Pointer to the pin configuration structure
 */
void GPIO_Init(GPIO_RegMap *GPIOX, GPIO_Conf *config);

/**
 * @brief GPIO pin write API.
 *
 * @param GPIOX 		Pointer to the port to which the pin belongs
 * @param state 		State to be given
 * @param pinNumber 	Pin number to be written
 */
void GPIO_Write(GPIO_RegMap *GPIOX, GPIO_Pin_State state, GPIO_Pin_Number pinNumber);

/**
 * @brief GPIO pin read API.
 *
 * @param GPIOX 		Pointer to the port to which the pin belongs
 * @param pinNumber		Pin number to be read
 *
 * @retval GPIO_Pin_Low 	(0x0U)		Pin is in low state
 * @retval GPIO_Pin_High 	(0x1U)		Pin is in high state
 */
GPIO_Pin_State GPIO_Read(GPIO_RegMap *GPIOX, GPIO_Pin_Number pinNumber);

/**
 * @brief GPIO pin toggle API.
 *
 * @param GPIOX 		Pointer to the port to which the pin belongs
 * @param pinNumber		Pin number to be toggled
 */
void GPIO_Toggle(GPIO_RegMap *GPIOX, GPIO_Pin_Number pinNumber);

/**
 * @brief GPIO pin interrupt handle API.
 *
 * @param pinNumber		Pin number to be used
 */
void GPIO_IRQ_Handle(GPIO_Pin_Number pinNumber);

#endif /* INC_STM32F401RE_GPIO_H_ */
