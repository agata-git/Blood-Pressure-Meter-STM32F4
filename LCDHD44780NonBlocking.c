#include "LCDHD44780NonBlocking.h"

/**
* \brief  Timer2 interrupt Handler sending data to LCD from FIFO
*/
void TIM2_IRQHandler()
{
		TIM2->SR &= ~TIM_SR_UIF;
	
		LCD_FIFO_Tail = (LCD_FIFO_Tail + 1) & LCD_FIFO_Mask;             /* Update FIFO Tail */
		uint16_t cmd = LCD_FIFO[ LCD_FIFO_Tail ];                                  /* Download data from FIFO */
	
		E_PORT->BSRR |= (1U << (E_PIN));                                             /*  Set E */
	
		#ifdef _8_bit_mode
		if( cmd & 0x100 ) { RS_PORT-> BSRR |= (1U << RS_PIN);  }            /* Set RS */
		else { RS_PORT-> BSRR |= (1U << (RS_PIN + 16)); }                      /* Reset RS */
		if( cmd & 0b00000001 ) { D0_PORT-> BSRR |= (1U << D0_PIN);  }    /* Set D0 */
		else { D0_PORT-> BSRR |= (1U << (D0_PIN + 16)); }                       /* Reset D0 */
		if( cmd & 0b00000010 ) { D1_PORT-> BSRR |= (1U << D1_PIN);  }    /* Set D1 */
		else { D1_PORT-> BSRR |= (1U << (D1_PIN + 16)); }                       /* Reset D1 */
		if( cmd & 0b00000100 ) { D2_PORT-> BSRR |= (1U << D2_PIN);  }    /* Set D2 */
		else { D2_PORT-> BSRR |= (1U << (D2_PIN + 16)); }                       /* Reset D2 */
		if( cmd & 0b00001000 ) { D3_PORT-> BSRR |= (1U << D3_PIN);  }    /* Set D3*/
		else { D3_PORT-> BSRR |= (1U << (D3_PIN + 16)); }                       /* Reset D3 */
		if( cmd & 0b00010000 ) { D4_PORT-> BSRR |= (1U << D4_PIN);  }    /* Set D4 */
		else { D4_PORT-> BSRR |= (1U << (D4_PIN + 16)); }                       /* Reset D4 */
		if( cmd & 0b00100000 ) { D5_PORT-> BSRR |= (1U << D5_PIN);  }    /* Set D5 */
		else { D5_PORT-> BSRR |= (1U << (D5_PIN + 16)); }                       /* Reset D5 */
		if( cmd & 0b01000000 ) { D6_PORT-> BSRR |= (1U << D6_PIN);  }    /* Set D6 */
		else { D6_PORT-> BSRR |= (1U << (D6_PIN + 16)); }                       /* Reset D6 */
		if( cmd & 0b10000000 ) { D7_PORT-> BSRR |= (1U << D7_PIN);  }    /* Set D7 */
		else { D7_PORT-> BSRR |= (1U << (D7_PIN + 16)); }                       /* Reset D7 */
		
		#else
		if( cmd & 0x100 ) { RS_PORT-> BSRR |= (1U << RS_PIN);  }            /* Set RS */
		else { RS_PORT-> BSRR |= (1U << (RS_PIN + 16)); }                      /* Reset RS */
		if( cmd & 0b00010000 ) { D4_PORT-> BSRR |= (1U << D4_PIN);  }    /* Set D4 */
		else { D4_PORT-> BSRR |= (1U << (D4_PIN + 16)); }                       /* Reset D4 */
		if( cmd & 0b00100000 ) { D5_PORT-> BSRR |= (1U << D5_PIN);  }    /* Set D5 */
		else { D5_PORT-> BSRR |= (1U << (D5_PIN + 16)); }                       /* Reset D5 */
		if( cmd & 0b01000000 ) { D6_PORT-> BSRR |= (1U << D6_PIN);  }    /* Set D6 */
		else { D6_PORT-> BSRR |= (1U << (D6_PIN + 16)); }                       /* Reset D6 */
		if( cmd & 0b10000000 ) { D7_PORT-> BSRR |= (1U << D7_PIN);  }    /* Set D7 */
		else { D7_PORT-> BSRR |= (1U << (D7_PIN + 16)); }                       /* Reset D7 */	
		E_PORT->BSRR |= (1U << (E_PIN + 16));                                       /*  Reset E */
		
		E_PORT->BSRR |= (1U << (E_PIN));                                              /* Set E */
		if( cmd & 0b00000001 ) { D4_PORT-> BSRR |= (1U << D4_PIN);  }    /* Set D4 */
		else { D4_PORT-> BSRR |= (1U << (D4_PIN + 16)); }                       /* Reset D4 */
		if( cmd & 0b00000010 ) { D5_PORT-> BSRR |= (1U << D5_PIN);  }    /* Set D5 */
		else { D5_PORT-> BSRR |= (1U << (D5_PIN + 16)); }                       /* Reset D5 */
		if( cmd & 0b00000100 ) { D6_PORT-> BSRR |= (1U << D6_PIN);  }    /* Set D6 */
		else { D6_PORT-> BSRR |= (1U << (D6_PIN + 16)); }                       /* Reset D6 */
		if( cmd & 0b00001000 ) { D7_PORT-> BSRR |= (1U << D7_PIN);  }    /* Set D7 */
		else { D7_PORT-> BSRR |= (1U << (D7_PIN + 16)); }                       /* Reset D7 */
		#endif
		
		E_PORT->BSRR |= (1U << (E_PIN + 16));                                       /* Reset E */
		if( LCD_FIFO_Head == LCD_FIFO_Tail ) { TIM2->DIER &= ~TIM_DIER_UIE; }  /* Turn off Timer if FIFO buffer empty */
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
* \brief		Function initializes LCD
*/
void LCDInitialize(void)
{		
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
		TIM2->SR = 0;
		TIM2->PSC = PSC_VALUE;
		TIM2->ARR = ARR_VALUE;
		
		#ifdef backlight_controlled_by_STM32
		enableClockToRegister(BACKLIGHT_PORT);
		setPinAsOutput(BACKLIGHT_PORT, BACKLIGHT_PIN);
		BACKLIGHT_PORT->BSRR |= (1U << BACKLIGHT_PIN);
		#endif
		
		enableClockToRegister(RS_PORT);
		enableClockToRegister(E_PORT); 
		enableClockToRegister(D4_PORT);
		enableClockToRegister(D5_PORT);
		enableClockToRegister(D6_PORT);
		enableClockToRegister(D7_PORT);
	
		setPinAsOutput(RS_PORT, RS_PIN);
		setPinAsOutput(E_PORT, E_PIN);
		setPinAsOutput(D4_PORT, D4_PIN);
		setPinAsOutput(D5_PORT, D5_PIN);
		setPinAsOutput(D6_PORT, D6_PIN);
		setPinAsOutput(D7_PORT, D7_PIN);
	
		#ifdef _8_bit_mode
		enableClockToRegister(D0_PORT);
		enableClockToRegister(D1_PORT);
		enableClockToRegister(D2_PORT);
		enableClockToRegister(D3_PORT);
	
		setPinAsOutput(D0_PORT, D0_PIN);
		setPinAsOutput(D1_PORT, D1_PIN);
		setPinAsOutput(D2_PORT, D2_PIN);
		setPinAsOutput(D3_PORT, D3_PIN);
	
		volatile uint32_t i =0;
		uint32_t delay = 720000;
		
		for(i=0; i<delay; i++) { }
		LCDSendHalfByte(0x03);
		for(i=0; i<delay; i++) { }
		LCDSendHalfByte(0x03);
		for(i=0; i<delay; i++) { }
		LCDSendHalfByte(0x03);
		for(i=0; i<delay; i++) { }
		
		LCD_FIFO[0] = _8bit_mode_2lines_5x7font;
		LCD_FIFO[1] = 0x08;
		LCD_FIFO[2] = clear_screen;
		LCD_FIFO[3] = 0x06;
		LCD_FIFO[4] = 0x0E;
		
		LCD_FIFO_Head = 4;
		#else 
		volatile uint32_t i =0;
		uint32_t delay = 720000;
		
		for(i=0; i<delay; i++) { }
		LCDSendHalfByte(0x03);
		for(i=0; i<delay; i++) { }
		LCDSendHalfByte(0x03);
		for(i=0; i<delay; i++) { }
		LCDSendHalfByte(0x03);
		for(i=0; i<delay; i++) { }
		
		LCDSendHalfByte(0x02);
		for(i=0; i<delay; i++) { }

		LCD_FIFO[0] = _4bit_mode_2lines_5x7font;
		LCD_FIFO[1] = 0x08;
		LCD_FIFO[2] = clear_screen;
		LCD_FIFO[3] = 0x06;
		LCD_FIFO[4] = 0x0E;
		
		LCD_FIFO_Head = 4;
		#endif
		
		//===============================
		//    Write Heart character to LCD CGRAM  
		LCDSendCommand(CGRAM_set);
		LCDSendData(0x00);
		LCDSendData(0xA);
		LCDSendData(0x1F);
		LCDSendData(0x1F);
		LCDSendData(0xE);
		LCDSendData(0x4);
		LCDSendData(0x0);
		LCDSendData(0x0);
		//===============================
		
		NVIC_SetPriority(TIM2_IRQn, 2);
		TIM2->DIER |= TIM_DIER_UIE;    // Enable interrupt
		TIM2->CR1 |= TIM_CR1_CEN;     // Enable counter
		NVIC_EnableIRQ(TIM2_IRQn);
}

/**
* \brief Function writes command to LCD FIFO buffer
* \param[in]  cmd  LCD command
*/
void LCDSendCommand (uint8_t cmd)
{
		LCD_temp_FIFO_Head = (LCD_FIFO_Head + 1) & LCD_FIFO_Mask;
		if( LCD_temp_FIFO_Head != LCD_FIFO_Tail ) 
			{
					LCD_FIFO_Head = LCD_temp_FIFO_Head;
					LCD_FIFO[LCD_FIFO_Head] = cmd; 
					TIM2->DIER |= TIM_DIER_UIE;
			}
}

/**
* \brief Function writes data to LCD FIFO buffer
* \param[in]  data  LCD data
*/
void LCDSendData (unsigned char data)
{
		LCD_temp_FIFO_Head = (LCD_FIFO_Head + 1) & LCD_FIFO_Mask;
		if( LCD_temp_FIFO_Head != LCD_FIFO_Tail ) 
			{ 
					LCD_FIFO_Head = LCD_temp_FIFO_Head;
					LCD_FIFO[LCD_FIFO_Head] = (uint16_t) data | 0x100; 
					TIM2->DIER |= TIM_DIER_UIE;
			}
}

/**
* \brief Function writes data to LCD FIFO buffer
* \param[in]  text  String message
*/
void LCDSendText(char *text)
{
		while(*text)
		{
				LCD_temp_FIFO_Head = (LCD_FIFO_Head + 1) & LCD_FIFO_Mask;
				if( LCD_temp_FIFO_Head != LCD_FIFO_Tail ) 
				{ 
						LCD_FIFO_Head = LCD_temp_FIFO_Head;
						LCD_FIFO[LCD_FIFO_Head] = 0x100 | *text; 
				}
				text++;
		}
		TIM2->DIER |= TIM_DIER_UIE;
}

/**
* \brief Function writes half byte of data to LCD FIFO buffer
* \param[in]  cmd  LCD data
*/
static void LCDSendHalfByte(uint8_t cmd)
{
		RS_PORT-> BSRR |= (1U << (RS_PIN + 16));
		E_PORT->BSRR |= (1U << E_PIN);        /*  Set E  */
	
		if( cmd & 0b00000001 ) { D4_PORT-> BSRR |= (1U << D4_PIN);  }    /* Set D4 */
		else { D4_PORT-> BSRR |= (1U << (D4_PIN + 16)); }                       /* Reset D4 */
		if( cmd & 0b00000010 ) { D5_PORT-> BSRR |= (1U << D5_PIN);  }    /* Set D5 */
		else { D5_PORT-> BSRR |= (1U << (D5_PIN + 16)); }                       /* Reset D5 */
		if( cmd & 0b00000100 ) { D6_PORT-> BSRR |= (1U << D6_PIN);  }    /* Set D6 */
		else { D6_PORT-> BSRR |= (1U << (D6_PIN + 16)); }                       /* Reset D6 */
		if( cmd & 0b00001000 ) { D7_PORT-> BSRR |= (1U << D7_PIN);  }    /* Set D7 */
		else { D7_PORT-> BSRR |= (1U << (D7_PIN + 16)); }                       /* Reset D7 */
		
		E_PORT->BSRR |= (1U << (E_PIN + 16) );  /*  Reset E  */
}

/**
* \brief Function displays an 8-bit integer 
* \param[in]  number  Number in range 0 - 255
*/
void LCDSend8bitInteger(uint8_t number)
{
		uint8_t k100 = number/100;
		uint8_t var = (uint8_t) number % 100;
		uint8_t k10 = var/10;
		uint8_t k1 = var % 10;
		LCDSendData('0' + k100);
		LCDSendData('0' + k10);
		LCDSendData('0' + k1);
}

/**
* \brief Function sets cursor on selected position
* \param[in]  x   Horizontal coordinate of LCD display
* \param[in]  y   Vertical coordinate of LCD display
*/
void LCDLocateXY(uint8_t x, uint8_t y)
{
		LCD_temp_FIFO_Head = (LCD_FIFO_Head + 1) & LCD_FIFO_Mask;
		if( LCD_temp_FIFO_Head != LCD_FIFO_Tail ) 
			{
					LCD_FIFO_Head = LCD_temp_FIFO_Head;
					LCD_FIFO[LCD_FIFO_Head] = DDRAM_set | (x + (0x40 * y)); 
					TIM2->DIER |= TIM_DIER_UIE;
			}
}
