/**
 * @file stm32f401re.h
 * @brief Device specific header for STM32F401RE
 * @author Harun Kaya
 * @date 02.11.2026
 */

#ifndef INC_STM32F401RE_H_
#define INC_STM32F401RE_H_

#include <stdint.h>

/**
 * @brief RCC register map structure.
 */
typedef struct RCC_RegMap RCC_RegMap;
struct RCC_RegMap
{
	volatile uint32_t CR;
	volatile uint32_t PLLCFGR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t AHB1RSTR;
	volatile uint32_t AHB2RSTR;
	volatile uint32_t RESERVED[2];
	volatile uint32_t APB1RSTR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t RESERVED1[2];
	volatile uint32_t AHB1ENR;
	volatile uint32_t AHB2ENR;
	volatile uint32_t RESERVED2[2];
	volatile uint32_t APB1ENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t RESERVED3[2];
	volatile uint32_t AHB1LPENR;
	volatile uint32_t AHB2LPENR;
	volatile uint32_t RESERVED4[2];
	volatile uint32_t APB1LPENR;
	volatile uint32_t APB2LPENR;
	volatile uint32_t RESERVED5[2];
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
	volatile uint32_t RESERVED6[2];
	volatile uint32_t SSCGR;
	volatile uint32_t PLLI2SCFGR;
	volatile uint32_t RESERVED7;
	volatile uint32_t DCKCFGR;
};

/**
 * @brief GPIO register map structure.
 */
typedef struct GPIO_RegMap GPIO_RegMap;
struct GPIO_RegMap
{
	volatile uint32_t MODER;
	volatile uint32_t OTYPER;
	volatile uint32_t OSPEEDR;
	volatile uint32_t PUPDR;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t LCKR;
	volatile uint32_t AFR[2];
};

/**
 * @brief EXTI register map structure.
 */
typedef struct EXTI_RegMap EXTI_RegMap;
struct EXTI_RegMap
{
	volatile uint32_t IMR;
	volatile uint32_t EMR;
	volatile uint32_t RTSR;
	volatile uint32_t FTSR;
	volatile uint32_t SWIER;
	volatile uint32_t PR;
};

/**
 * @brief SYSCFG register map structure.
 */
typedef struct SYSCFG_RegMap SYSCFG_RegMap;
struct SYSCFG_RegMap
{
	volatile uint32_t MEMRMP;
	volatile uint32_t PMC;
	volatile uint32_t EXTICR[4];
	volatile uint32_t CMPCR;
};

/**
 * @brief NVIC register map structure.
 */
typedef struct NVIC_RegMap NVIC_RegMap;
struct NVIC_RegMap
{
	volatile uint32_t ISER[8];
	volatile uint32_t RESERVED[24];
	volatile uint32_t ICER[8];
	volatile uint32_t RESERVED1[24];
	volatile uint32_t ISPR[8];
	volatile uint32_t RESERVED2[24];
	volatile uint32_t ICPR[8];
	volatile uint32_t RESERVED3[24];
	volatile uint32_t IABR[8];
	volatile uint32_t RESERVED4[56];
	volatile uint32_t IPR[60];
	volatile uint32_t RESERVED5[644];
	volatile uint32_t STIR;
};

/**
 * @brief I2C register map structure.
 */
typedef struct I2C_RegMap I2C_RegMap;
struct I2C_RegMap
{
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t OAR1;
	volatile uint32_t OAR2;
	volatile uint32_t DR;
	volatile uint32_t SR1;
	volatile uint32_t SR2;
	volatile uint32_t CCR;
	volatile uint32_t TRISE;
	volatile uint32_t FLTR;

};

/**
 * @brief UART register map structure.
 */
typedef struct UART_RegMap UART_RegMap;
struct UART_RegMap
{
	volatile uint32_t SR;
	volatile uint32_t DR;
	volatile uint32_t BRR;
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t CR3;
	volatile uint32_t GTPR;
};

/**
 * @defgroup Peripheral_Pointers Peripheral pointer macros
 * @{
 */
#define RCC		((RCC_RegMap*)0x40023800U)
#define GPIOA	((GPIO_RegMap*)0x40020000U)
#define GPIOB	((GPIO_RegMap*)0x40020400U)
#define GPIOC	((GPIO_RegMap*)0x40020800U)
#define GPIOD	((GPIO_RegMap*)0x40020C00U)
#define GPIOE	((GPIO_RegMap*)0x40021000U)
#define GPIOH	((GPIO_RegMap*)0x40021C00U)
#define EXTI	((EXTI_RegMap*)0x40013C00U)
#define NVIC	((NVIC_RegMap*)0xE000E100U)
#define SYSCFG 	((SYSCFG_RegMap*)0x40013800U)
#define I2C1	((I2C_RegMap*)0x40005400U)
#define I2C2	((I2C_RegMap*)0x40005800U)
#define I2C3	((I2C_RegMap*)0x40005C00U)
#define UART1	((UART_RegMap*)0x40011000U)
#define UART2	((UART_RegMap*)0x40004400u)
#define UART6	((UART_RegMap*)0x40011400U)
/**
 * @}
 */

/**
 * @defgroup Clock_Enable_Macros Peripheral clock enable macros
 *@{
 */
#define GPIOA_CLK_EN()	(RCC->AHB1ENR |= 0x1U << 0)
#define GPIOB_CLK_EN()	(RCC->AHB1ENR |= 0x1U << 1)
#define GPIOC_CLK_EN()	(RCC->AHB1ENR |= 0x1U << 2)
#define GPIOD_CLK_EN()	(RCC->AHB1ENR |= 0x1U << 3)
#define GPIOE_CLK_EN()	(RCC->AHB1ENR |= 0x1U << 4)
#define GPIOH_CLK_EN()	(RCC->AHB1ENR |= 0x1U << 7)
#define SYSCFG_CLK_EN()	(RCC->APB2ENR |= 0x1U << 14)
#define I2C1_CLK_EN()	(RCC->APB1ENR |= 0x1U << 21)
#define I2C2_CLK_EN()	(RCC->APB1ENR |= 0x1U << 22)
#define I2C3_CLK_EN()	(RCC->APB1ENR |= 0x1U << 23)
#define UART1_CLK_EN()	(RCC->APB2ENR |= 0x1U << 4)
#define UART2_CLK_EN()	(RCC->APB1ENR |= 0x1U << 17)
#define UART6_CLK_EN()	(RCC->APB2ENR |= 0x1U << 5)
/**
 * @}
 */

/**
 * @brief GPIO base address to port code macro.
 *
 * @param x GPIOX port base address
 *
 * @retval 0x0U GPIOA
 * @retval 0x1U GPIOB
 * @retval 0x2U GPIOC
 * @retval 0x3U GPIOD
 * @retval 0x4U GPIOE
 * @retval 0x5U GPIOH
 * @retval 0xFFU Undefined port
 */
#define GPIO_GET_PORT_CODE(x)	(x == GPIOA ? 0x0U : \
								 x == GPIOB ? 0x1U : \
								 x == GPIOC ? 0x2U : \
								 x == GPIOD ? 0x3U : \
								 x == GPIOE ? 0x4U : \
								 x == GPIOH ? 0x5U : 0x0FFU)

#include "stm32f401re_gpio.h"
#include "stm32f401re_i2c.h"
#include "stm32f401re_uart.h"
#include "stm32f401re_ll_rcc.h"

#endif /* INC_STM32F401RE_H_ */
