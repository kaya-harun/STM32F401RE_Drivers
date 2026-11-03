#include "stm32f401re_gpio.h"

void GPIO_Init(GPIO_RegMap *GPIOX, GPIO_Conf *config)
{
	uint8_t aFRX, aFRBit, portCode, EXTICRX, EXTICRBit, IPRX, IPRBit, ISERX, ISERBit;

	GPIOX->MODER &= ~(0x3U << (2 * config->pinNumber)); //Clear field

	GPIOX->OTYPER &= ~(0x1U << config->pinNumber); //Clear field

	GPIOX->PUPDR &= ~(0x3U << (2 * config->pinNumber)); //Clear field

	aFRX = config->pinNumber / 8; //Decide AFRH or AFRL
	aFRBit = config->pinNumber % 8; //Corresponding bit
	GPIOX->AFR[aFRX] &= ~(0xFU << (4 * aFRBit)); //Clear field

	EXTI->RTSR &= ~(0x1U << config->pinNumber); //Clear field
	EXTI->FTSR &= ~(0x1U << config->pinNumber); //Clear field

	portCode = GPIO_GET_PORT_CODE(GPIOX);
	EXTICRX = config->pinNumber / 4; //EXTICR number
	EXTICRBit = config->pinNumber % 4; //Corresponding bit
	SYSCFG->EXTICR[EXTICRX] &= ~(0xFU << (4 * EXTICRBit)); //Clear field

	IPRX = config->IRQNumber / 4; //IPR number
	IPRBit = config->IRQNumber % 4; //Corresponding bit
	NVIC->IPR[IPRX] &= ~(0xFFU << (8 * IPRBit)); //Clear field

	ISERX = config->IRQNumber / 32; //ICER number
	ISERBit = config->IRQNumber % 32; //Corresponding bit
	NVIC->ICER[ISERX] |= 0x1U << ISERBit; //Disable interrupt

	EXTI->IMR &= ~(0x1U << config->pinNumber); //Mask line


	if(config->mode <= GPIO_MODE_AF)
	{
		GPIOX->MODER |= config->mode << (2 * config->pinNumber); //Set mode

		GPIOX->OTYPER |= config->oType << config->pinNumber; //Set output type

		GPIOX->PUPDR |= config->puPd << (2 * config->pinNumber); //Set pull-up / pull-down type

		GPIOX->AFR[aFRX] |= config->aF << (4 * aFRBit); //Set alternate function number
	}
	else
	{
		GPIOX->PUPDR |= config->puPd << (2 * config->pinNumber); //Set pull-up / pull-down type

		switch(config->mode)
		{
		case GPIO_MODE_IT_RT:
			EXTI->RTSR |= 0x1U << config->pinNumber; //Set edge trigger type
			break;

		case GPIO_MODE_IT_FT:
			EXTI->FTSR |= 0x1U << config->pinNumber; //Set edge trigger type
			break;

		case GPIO_MODE_IT_RFT:
			EXTI->RTSR |= 0x1U << config->pinNumber; //Set edge trigger type
			EXTI->FTSR |= 0x1U << config->pinNumber; //Set edge trigger type
			break;

		default:
			break;
		};

		SYSCFG->EXTICR[EXTICRX] |= portCode << (4 * EXTICRBit); //Set line mapping

		NVIC->IPR[IPRX] |= config->priority << ((8 * IPRBit) + 4); //Set priority

		NVIC->ISER[ISERX] |= 0x1U << ISERBit; //Enable interrupt

		EXTI->IMR |= 0x1U << config->pinNumber; //Unmask line
	}
}
