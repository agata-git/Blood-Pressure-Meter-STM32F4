#include "BloodPressureMeterFunctions.h"

float IIRforwardCoeffLowpass_20Hz[5] = {0.000183216023370f, 0.000732864093479f, 0.001099296140218f, 0.000732864093479f, 0.000183216023370f};
float IIRreverseCoeffLowpass_20Hz[5] = {1.000000000000000f, -3.344067837711877f,  4.238863950884074f, -2.409342856586326f, 0.517478199788043f};

uint16_t pressureArray[32];  /**< Cyclic buffer with pressure samples from sensor */ 
volatile uint16_t pressureArrayHead;  /**< Head of  pressureArray buffer */ 
volatile uint16_t pressureArrayTail;  /**< Tail of  pressureArray buffer */ 

float Last128SamplesBuffer[128];  /**< Cyclic buffer with previous samples of signal */ 
uint16_t Last128SamplesBufferHead;  /**< Head of  Last128SamplesBuffer buffer */ 
							
/**
* \brief  Function for setting core clock, AHB1, APB1 clock on 42MHz
*/
void RCCClockConfiguration(void)
{
		RCC->CR |= (uint32_t)0x00000001;                //  Enable HSI
		while(! (RCC->CR & RCC_CR_HSIRDY)) { }     //  Wait until HSI ready

		/* Reset CFGR register */
		RCC->CFGR = 0x00000000;

		/* Reset HSEON, CSSON and PLLON bits */
		RCC->CR &= (uint32_t)0xFEF6FFFF;

		/* Reset PLLCFGR register */
		RCC->PLLCFGR = 0x24003010;

		/* Reset HSEBYP bit */
		RCC->CR &= (uint32_t)0xFFFBFFFF;

		/* Disable all interrupts */
		RCC->CIR = 0x00000000;
	
		RCC->PLLCFGR |= RCC_PLLCFGR_PLLM_4;  // PLLM = 16
	
		RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN;  // PLLN = 336
		RCC->PLLCFGR |= (336U << 6);
	
		RCC->PLLCFGR |= RCC_PLLCFGR_PLLP_0; // PLLP = 8
		RCC->PLLCFGR |= RCC_PLLCFGR_PLLP_1;
	
		RCC->CR |= RCC_CR_PLLON;                      //  Enable PLL
		while(! (RCC->CR & RCC_CR_PLLRDY)) { }    //  Wait ubtil PLL enabled
		
		RCC->CFGR &= ~RCC_CFGR_SW;              //  Reset System clock switch bits 
		RCC->CFGR |= RCC_CFGR_SW_1;              //  Switch system clock on PLL
	
		RCC->CFGR &= ~RCC_CFGR_HPRE;    //   AHB Prescaler = 1	
		RCC->CFGR &= ~RCC_CFGR_PPRE1;  //   APB1 Prescaler = 1
		RCC->CFGR &= ~RCC_CFGR_PPRE2;  //   APB2 Prescaler = 1
		
		SystemCoreClockUpdate();
}

/**
* \brief  Function used for pump, valve and button pins initialization
*/
void PumpValveButtonConfiguration(void)
{
		enableClockToRegister(PUMP_PORT);
		enableClockToRegister(VALVE_PORT);
		enableClockToRegister(BUTTON_PORT);
	
		setPinAsOutput(PUMP_PORT, PUMP_PIN);
		setPinAsOutput(VALVE_PORT, VALVE_PIN);
		setPinAsInput(BUTTON_PORT, BUTTON_PIN);
		
		PUMP_PORT->ODR |= (1U << PUMP_PIN);
		VALVE_PORT->ODR &= ~(1U << VALVE_PIN);
		BUTTON_PORT->OTYPER &= ~(1U << BUTTON_PIN);
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

/**
* \brief		Function sets selected GPIO pin as output
* \param[in]  GPIO_port  Selected GPIO port
* \param[in]  pin  Selected GPIO pin
*/
static inline void setPinAsOutput( GPIO_TypeDef * GPIO_port, uint32_t pin )
{
	   GPIO_port-> MODER  |= (0x1U << (pin*2));
     GPIO_port->  MODER &= ~(0x2U << (pin*2));  
}

/**
* \brief		Function sets selected GPIO pin as input
* \param[in]  GPIO_port  Selected GPIO port
* \param[in]  pin  Selected GPIO pin
*/
static inline void setPinAsInput( GPIO_TypeDef * GPIO_port, uint32_t pin )
{
	   GPIO_port-> MODER  &= ~(0x1U << (pin*2));
     GPIO_port->  MODER &= ~(0x2U << (pin*2));  
}
