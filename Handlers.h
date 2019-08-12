#ifndef __Handlers_H__
#define __Handlers_H__

#include "stm32f4xx.h"                  
#include "LCDHD44780NonBlocking.h"
#include "BloodPressureMeterFunctions.h"

/**
* \defgroup  Handlers  Handlers
* \addtogroup Handlers
* \brief  Module containing handlers of all Timers used in project
* \{
*/
//=====================================
//  Global variables used in main loop of program
//=====================================
volatile uint8_t calculate = 0;                                    /**< Variable controlling calculation on samples from pressureArray */
volatile uint8_t displayOnLCDevery1sec = 0;                        /**< Variable controlling displaying data on LCD every one second */
volatile uint8_t displayOnLCDevery250milisec = 0;               /**< Variable controlling displaying data on LCD every 250 millisecond */
//=====================================

/**
* \brief Function used for initializing of Timers used in project
*/
void TimersInitialization(void)
{
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
		TIM3->PSC = 1000;
		TIM3->ARR = 168;
		TIM3->DIER |= TIM_DIER_UIE;
		NVIC_SetPriority(TIM3_IRQn,1);
		NVIC_EnableIRQ(TIM3_IRQn);

		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
		TIM4->PSC = 4000;
		TIM4->ARR = 2625;
		NVIC_SetPriority(TIM4_IRQn,1);

		RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
		TIM5->PSC = 8400;
		TIM5->ARR = 1000;
		TIM5->CR1 |= TIM_CR1_CEN;
		TIM5->DIER |= TIM_DIER_UIE;
		NVIC_EnableIRQ(TIM5_IRQn);
		NVIC_SetPriority(TIM5_IRQn,0);
}

/**
* \brief  I2C1 interrupt handling communication with pressure sensor 
*/
void I2C1_EV_IRQHandler()
{
		static volatile int8_t byteToReceive=1;
		static uint32_t reg;	
		static uint16_t tempHead=0, received=0;
		reg = I2C1->SR1;
	
	if(reg	& I2C_SR1_SB)
	{
		reg = I2C1->SR1;
		I2C1->DR   = ((0x28 << 1) | 0x01); 
	}
	else
	{
			if(reg & I2C_SR1_ADDR)
			{
				reg = I2C1->SR1;
				reg = I2C1->SR2;
				byteToReceive = 1;
			}
			else
			{
					if( byteToReceive )
					{
						received = (I2C1->DR);
						received = (received << 8) & 0b0011111100000000;	
						I2C1->CR1 &= ~I2C_CR1_ACK;   // disable acknowledge	
					}
					
					if( ! byteToReceive)
					{
						I2C1->CR1 |= I2C_CR1_STOP;
						received = received | I2C1->DR;
						tempHead = (pressureArrayHead + 1) & pressureArrayMask;
						if(tempHead != pressureArrayTail) 
							{  
									pressureArray[pressureArrayHead] = received;
									pressureArrayHead = tempHead;
							}
						else
							{ 
									TIM3->CR1 &= ~TIM_CR1_CEN;
									I2C1->CR1 &= ~I2C_CR1_PE;
									LCDLocateXY(0,0);
									LCDSendText("     ERROR      ");
									LCDLocateXY(0,1);
									LCDSendText("  RESET DEVICE  ");
							}
					}
					byteToReceive = 0;
			}
	}
}

/**
* \brief  TIM3 Handler triggering I2C communication every 4 ms  ( 250 Hz )
*/
void TIM3_IRQHandler()
{
		TIM3->SR &= ~TIM_SR_UIF;
		
		if(I2C1->SR2 & I2C_SR2_BUSY) 
			{ 
					LCDLocateXY(0,0);
					LCDSendText("     ERROR      ");
					LCDLocateXY(0,1);
					LCDSendText("  RESET DEVICE  ");
			}

		I2C1->CR1 |= I2C_CR1_ACK;      // Enable acknowledge
		I2C1->CR1 |= I2C_CR1_START;  //  Start of I2C communication
}

/**
* \brief  TIM4 Handler updating variables for controlling data display on LCD every 250 ms
*/
void TIM4_IRQHandler()
{
		TIM4->SR &= ~TIM_SR_UIF;
	
		static uint8_t counter = 0;
		if(! (counter & 3) ) { displayOnLCDevery1sec = 1; counter = 0; }
		counter++;
		displayOnLCDevery250milisec = 1;
}

/**
* \brief  TIM5 Handler for checking if button is pressed and pump/valve turn on/off controlling.
*          TIM5 enables TIM3 and TIM4 when button pressed.
*/
void TIM5_IRQHandler(void)
{
		static float pressure = 0.0f;
		static uint8_t buttonPressed = 0, _1secDelay = 0;
		static uint8_t _180mmHg_achieved = 0;
		TIM5->SR &= ~TIM_SR_UIF;
	
		if(_1secDelay)
		{
				_1secDelay++;
				if( _1secDelay > 5)
				{
						calculate = 1;
						buttonPressed = 0;
						_1secDelay = 0;
						
						TIM5->CR1 &= ~TIM_CR1_CEN;		                        //  Disable TIM5
						VALVE_PORT->ODR &= ~(1U << VALVE_PIN);    //  Turn off valve after one second of waiting on pressure stabilization
				}
		}
	
		if(_180mmHg_achieved)
		{
				PUMP_PORT->ODR |=  (1U << PUMP_PIN);            //  Turn off Pump
				_1secDelay++;                                                       //  Start counting one second
				_180mmHg_achieved = 0;			
				LCDLocateXY(0,0);
				LCDSendText("                ");				
		}
	
		if(buttonPressed) 
		{
					pressure = ((pressureArray[pressureArrayTail] - 1638.0f) * 5.0f) / (14745.0f - 1638.0f) * 52.0f; 
					if( (uint32_t) pressure > 179) { _180mmHg_achieved = 1; }
		}
		else 
		{
				if(! (GPIOA->IDR & GPIO_IDR_ID0) ) 
				{
							buttonPressed = 1;
							LCDLocateXY(0,0);
							LCDSendText("PUMP TO 180mmHg");
							LCDLocateXY(0,1);
							LCDSendText("Pressure:      ");	
							TIM4->CNT = 0;
							TIM4->SR &= ~TIM_SR_UIF;
							TIM4->CR1 |= TIM_CR1_CEN;
							TIM4->DIER |= TIM_DIER_UIE;			
							NVIC_EnableIRQ(TIM4_IRQn);                          // Enable TIM4
							VALVE_PORT->ODR |= (1U << VALVE_PIN);   // Turn on valve
							PUMP_PORT->ODR &= ~(1U << PUMP_PIN);  // Turn on Pump
							TIM3->CR1 |= TIM_CR1_CEN;                          // Enable TIM3
				}								
		}
}

/**
* \}
*/

#endif
