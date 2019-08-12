#include "PressureSensorABPMANN005PG2A3.h"


void PressureSensorInitialize(void)
{
		enableClockToRegister(I2C_SDA_PORT);
		enableClockToRegister(I2C_SCL_PORT);
		
		RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;       //  Enable Clock to peripheral

/*  Initialization procedure of I2C bus preventing from hanging and other problems:
1. Disable the I2C peripheral by clearing the PE bit in I2Cx_CR1 register.
2. Configure the SCL and SDA I/Os as General Purpose Output Open-Drain, High level
(Write 1 to GPIOx_ODR).
3. Check SCL and SDA High level in GPIOx_IDR.
4. Configure the SDA I/O as General Purpose Output Open-Drain, Low level (Write 0 to
GPIOx_ODR).
5. Check SDA Low level in GPIOx_IDR.
6. Configure the SCL I/O as General Purpose Output Open-Drain, Low level (Write 0 to
GPIOx_ODR).
7. Check SCL Low level in GPIOx_IDR.
8. Configure the SCL I/O as General Purpose Output Open-Drain, High level (Write 1 to
GPIOx_ODR).
9. Check SCL High level in GPIOx_IDR.
10. Configure the SDA I/O as General Purpose Output Open-Drain , High level (Write 1 to
GPIOx_ODR).
11. Check SDA High level in GPIOx_IDR.
12. Configure the SCL and SDA I/Os as Alternate function Open-Drain.
13. Set SWRST bit in I2Cx_CR1 register.
14. Clear SWRST bit in I2Cx_CR1 register.
15. Enable the I2C peripheral by setting the PE bit in I2Cx_CR1 register.
**/
		//         1
		I2C1->CR1 &= ~I2C_CR1_PE;
		
		//         2
		I2C_SCL_PORT->MODER |= (0x1U << (I2C_SCL_PIN*2));
		I2C_SCL_PORT->MODER &= ~(0x2U << (I2C_SCL_PIN*2));
		
		I2C_SDA_PORT->MODER |= (0x1U << (I2C_SDA_PIN*2));
		I2C_SDA_PORT->MODER &= ~(0x2U << (I2C_SDA_PIN*2));
	
		I2C_SCL_PORT->OTYPER |= (1U << I2C_SCL_PIN);
		I2C_SDA_PORT->OTYPER |= (1U << I2C_SDA_PIN);
		
		I2C_SCL_PORT->ODR |= (1U << I2C_SCL_PIN);
		I2C_SDA_PORT->ODR |= (1U << I2C_SDA_PIN);
		
		//        3
		while( !(I2C_SCL_PORT->IDR & (1<<I2C_SCL_PIN) ) && !(I2C_SDA_PORT->IDR & (1<<I2C_SDA_PIN)) )  { }
	
		//        4
		I2C_SDA_PORT->MODER |= (0x1U << (I2C_SDA_PIN*2));
		I2C_SDA_PORT->MODER &= ~(0x2U << (I2C_SDA_PIN*2));
		I2C_SDA_PORT->OTYPER |= (1U << I2C_SDA_PIN);
		I2C_SDA_PORT->ODR &= ~(1U << I2C_SDA_PIN);
		
		//        5
		while( I2C_SDA_PORT->IDR & (1<<I2C_SDA_PIN)  ) { }
		
		//        6
		I2C_SCL_PORT->MODER |= (0x1U << (I2C_SCL_PIN*2));
		I2C_SCL_PORT->MODER &= ~(0x2U << (I2C_SCL_PIN*2));
		I2C_SCL_PORT->OTYPER |= (1U << I2C_SCL_PIN);
		I2C_SCL_PORT->ODR &= ~(1U << I2C_SCL_PIN);
		
		//        7
		while( I2C_SCL_PORT->IDR & (1<<I2C_SCL_PIN)  ) { }
		
		//        8
		I2C_SCL_PORT->MODER |= (0x1U << (I2C_SCL_PIN*2));
		I2C_SCL_PORT->MODER &= ~(0x2U << (I2C_SCL_PIN*2));
		I2C_SCL_PORT->OTYPER |= (1U << I2C_SCL_PIN);
		I2C_SCL_PORT->ODR |= (1<<I2C_SCL_PIN);
		
		//         9
		while( !(I2C_SCL_PORT->IDR & (1<<I2C_SCL_PIN) ) ) { }
		
		//         10
		I2C_SDA_PORT->MODER |= (0x1U << (I2C_SDA_PIN*2));
		I2C_SDA_PORT->MODER &= ~(0x2U << (I2C_SDA_PIN*2));
		I2C_SDA_PORT->OTYPER |= (1U << I2C_SDA_PIN);
		I2C_SDA_PORT->ODR |= (1<<I2C_SDA_PIN);
		
		//         11
		while( !(I2C_SDA_PORT->IDR & (1<<I2C_SDA_PIN) ) ) { }
		
		//          12
		I2C_SDA_PORT->MODER |= (0x2U << (I2C_SDA_PIN*2));
		I2C_SDA_PORT->MODER &= ~(0x1U << (I2C_SDA_PIN*2));
		I2C_SDA_PORT->AFR[I2C_SDA_PIN/8] |= (1U << 6);
		I2C_SDA_PORT->OTYPER |= (1U << I2C_SDA_PIN);
		I2C_SDA_PORT->PUPDR |= (0x1U << (I2C_SDA_PIN*2));
		I2C_SDA_PORT->PUPDR &= ~(0x2U << (I2C_SDA_PIN*2));
		
		I2C_SCL_PORT->MODER |= (0x2U << (I2C_SCL_PIN*2));
		I2C_SCL_PORT->MODER &= ~(0x1U << (I2C_SCL_PIN*2));
		I2C_SCL_PORT->AFR[I2C_SCL_PIN/8] |= (1U << 2);
		I2C_SCL_PORT->OTYPER |= (1U << I2C_SCL_PIN);
		I2C_SCL_PORT->PUPDR |= (0x1U << (I2C_SCL_PIN*2));
		I2C_SCL_PORT->PUPDR &= ~(0x2U << (I2C_SCL_PIN*2));
		
		//         13
		I2C1->CR1 |= I2C_CR1_SWRST;
		
		//         14
		I2C1->CR1 &= ~I2C_CR1_SWRST;

		I2C1->CR2 &= ~I2C_CR2_FREQ;
		I2C1->CR2 |= APBClockSpeed_MHz;                       
		
		I2C1->CCR &= ~I2C_CCR_FS;        //   Sm mode I2C in master mode selection


		I2C1->CCR &= ~ I2C_CCR_CCR;
		I2C1->CCR |= CCR_VALUE;
		I2C1->TRISE &= ~ I2C_TRISE_TRISE;
		I2C1->TRISE |= TRISE_VALUE;
		
		I2C1->CR2 |= I2C_CR2_ITEVTEN;    //  Event Interrupt enabled
		I2C1->CR2 |= I2C_CR2_ITBUFEN; 
		I2C1->CR1 |= I2C_CR1_PE;            //   Peripheral enable  
		while(! (I2C1->CR1 & I2C_CR1_PE) ) { }
		
		I2C1->CR1 &= ~I2C_CR1_POS;      //   Disable POS
		NVIC_SetPriority(I2C1_EV_IRQn,0);
		NVIC_EnableIRQ(I2C1_EV_IRQn);
}

/**
* \brief		Function enables clock to selected GPIO port 
* \param[in]  GPIO_port  Selected GPIO port
*/
static inline void enableClockToRegister(GPIO_TypeDef * GPIO_port)
{
		if( GPIO_port == GPIOA ) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; }
		if( GPIO_port == GPIOB ) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; }
		if( GPIO_port == GPIOC ) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; }
		if( GPIO_port == GPIOD ) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; }
		if( GPIO_port == GPIOE ) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN; }
		if( GPIO_port == GPIOF ) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN; }
		if( GPIO_port == GPIOG ) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN; }
		if( GPIO_port == GPIOH ) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN; }
}
