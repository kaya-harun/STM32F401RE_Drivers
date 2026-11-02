/**
 * @file stm32f401re.h
 * @brief Device specific header for STM32F401RE
 * @author Harun Kaya
 * @date 02.11.2026
 */

#ifndef INC_STM32F401RE_H_
#define INC_STM32F401RE_H_

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
 * @defgroup Peripheral_Pointers Peripheral pointer macros
 * @{
 */
#define RCC		((RCC_RegMap_t*)RCC_BASEADDR)
#define GPIOA	((GPIO_RegMap_t*)GPIOA_BASEADDR)
#define GPIOB	((GPIO_RegMap_t*)GPIOB_BASEADDR)
#define GPIOC	((GPIO_RegMap_t*)GPIOC_BASEADDR)
#define GPIOD	((GPIO_RegMap_t*)GPIOD_BASEADDR)
#define GPIOE	((GPIO_RegMap_t*)GPIOE_BASEADDR)
#define GPIOH	((GPIO_RegMap_t*)GPIOH_BASEADDR)
#define EXTI	((EXTI_RegMap_t*)EXTI_BASEADDR)
#define NVIC	((NVIC_RegMap_t*)NVIC_BASEADDR)
#define SYSCFG 	((SYSCFG_RegMap_t*)SYSCFG_BASEADDR)
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

#endif /* INC_STM32F401RE_H_ */
