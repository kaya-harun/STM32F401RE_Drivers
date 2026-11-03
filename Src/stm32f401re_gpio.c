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

		ISERX = config->IRQNumber / 32; //ICER number
		ISERBit = config->IRQNumber % 32; //Corresponding bit
		NVIC->ISER[ISERX] |= 0x1U << ISERBit; //Enable interrupt

		EXTI->IMR |= 0x1U << config->pinNumber; //Unmask line
	}
}

void GPIO_Deinit(GPIO_RegMap *GPIOX, GPIO_Pin_Number pinNumber, GPIO_IRQ_Number IRQNumber)
{
	uint8_t aFRX, aFRBit, IPRX, IPRBit, ISERX, ISERBit;
	//Reset MODER
	if(GPIOX == GPIOA)
	{
		switch(pinNumber)
		{
		case GPIO_PIN_15:
			break;
		case GPIO_PIN_14:
			break;
		case GPIO_PIN_13:
			break;
		default:
			GPIOX->MODER &= ~(((0x1U << 2) - 1) << (2 * pinNumber));
			break;
		};
	}
	else if(GPIOX == GPIOB)
	{
		switch(pinNumber)
		{
		case GPIO_PIN_4:
			break;
		case GPIO_PIN_3:
			break;
		default:
			GPIOX->MODER &= ~(((0x1U << 2) - 1) << (2 * pinNumber));
			break;
		};
	}
	else
	{
		GPIOX->MODER &= ~(((0x1U << 2) - 1) << (2 * pinNumber));
	}


	//Reset OTYPER
	GPIOX->OTYPER &= ~(0x1U << pinNumber);

	//Reset PUPDR
	if(GPIOX == GPIOA)
	{
		switch(pinNumber)
		{
		case GPIO_PIN_15:
			break;
		case GPIO_PIN_14:
			break;
		case GPIO_PIN_13:
			break;
		default:
			GPIOX->PUPDR &= ~(((0x1U << 2) - 1) << (2 * pinNumber));
			break;
		};
	}
	else if(GPIOX == GPIOB)
	{
		switch(pinNumber)
		{
		case GPIO_PIN_4:
			break;
		default:
			GPIOX->PUPDR &= ~(((0x1U << 2) - 1) << (2 * pinNumber));
			break;
		};
	}
	else
	{
		GPIOX->PUPDR &= ~(((0x1U << 2) - 1) << (2 * pinNumber));
	}

	//Clear AFR
	aFRX = pinNumber / 8; //Decide AFRH or AFRL
	aFRBit = pinNumber % 8; //Corresponding bit
	GPIOX->AFR[aFRX] &= ~(0xFU << (4 * aFRBit)); //Clear field

	//Clear RTSR and FTSR
	EXTI->RTSR &= ~(0x1U << pinNumber); //Clear field
	EXTI->FTSR &= ~(0x1U << pinNumber); //Clear field

	//Clear IPR
	IPRX = IRQNumber / 4; //IPR number
	IPRBit = IRQNumber % 4; //Corresponding bit
	NVIC->IPR[IPRX] &= ~(0xFFU << (8 * IPRBit)); //Clear field

	//Disable interrupt
	switch(IRQNumber)
	{
	case GPIO_IRQ_EXTI9_5:
		if(!(EXTI->IMR & (((0x1U << 5) - 1) << 5)))
		{
			ISERX = IRQNumber / 32; //ICER number
			ISERBit = IRQNumber % 32; //Corresponding bit
			NVIC->ICER[ISERX] |= 0x1U << ISERBit; //Disable interrupt
		}
		break;
	case GPIO_IRQ_EXTI15_10:
		if(!(EXTI->IMR & (((0x1U << 6) - 1) << 10)))
		{
			ISERX = IRQNumber / 32; //ICER number
			ISERBit = IRQNumber % 32; //Corresponding bit
			NVIC->ICER[ISERX] |= 0x1U << ISERBit; //Disable interrupt
		}
		break;
	default:
		ISERX = IRQNumber / 32; //ICER number
		ISERBit = IRQNumber % 32; //Corresponding bit
		NVIC->ICER[ISERX] |= 0x1U << ISERBit; //Disable interrupt
		break;
	};

	//Mask line
	EXTI->IMR &= ~(0x1U << pinNumber); //Mask line
}

void GPIO_Write(GPIO_RegMap *GPIOX, GPIO_Pin_State state, GPIO_Pin_Number pinNumber)
{
	if(state)
		GPIOX->BSRR |= 0x1U << pinNumber;
	else
		GPIOX->BSRR |= 0x1U << (pinNumber + 16);
}

GPIO_Pin_State GPIO_Read(GPIO_RegMap *GPIOX, GPIO_Pin_Number pinNumber)
{
	return ((GPIOX->IDR >> pinNumber) & 0x1U);
}

void GPIO_Toggle(GPIO_RegMap *GPIOX, GPIO_Pin_Number pinNumber)
{
	GPIOX->ODR ^= 0x1U << pinNumber;
}

void GPIO_IRQ_Handle(GPIO_Pin_Number pinNumber)
{
	EXTI->PR |= 0x1U << pinNumber;
}
