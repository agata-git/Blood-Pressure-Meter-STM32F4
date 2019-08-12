#ifndef __LCD_HD44780_Timer__H_
#define __LCD_HD44780_Timer__H_

#include "stm32f4xx.h"                
#include "stm32f446xx.h"
#include "stdint.h"

/** 
* \defgroup	 LCDHD44780NonBlocking		LCD HD44780 Non Blocking
*
* \brief					 LCDHD44780NonBlocking is basic library for hd44780 based LCD displays.
*									 Library allows "non-blocking" usage of LCD display. TIM2 is involved in sending
*                  data to display in equal periods of time with length determined by user. Only writing 
*                  data to LCD is possible, reading is not supported yet. Commands and data are stored
*                  within FIFO buffer. Length of FIFO is determined by the user and must be a power of 2.
*                  Data from FIFO are downloaded during Timer interrupt service routine. 
*                  Library supports STM32F4 microcontroller series.
*
* \addtogroup  LCDHD44780NonBlocking
* \{
*/
//==============================================
//       SECTION OF USER MODIFICATION AND SETTINGS
//==============================================

#define _4_bit_mode  																		  /**< Uncomment this macro if 4 bit mode slected */
//#define _8_bit_mode  																	  /**< Uncomment this macro if 8 bit mode slected */
#define backlight_controlled_by_STM32							/**< Uncomment this macro if LCD backligh is controlled by microcontroller pin */

//=================================
//                 Timer configuration
//=================================
#define	 PSC_VALUE		2000														/**< Set value of Timer's Prescaler register */
#define	 ARR_VALUE		105															/**< Set value of Timer's auto-reload register */

//=================================
//             Ports and pins definitions
//=================================
#ifdef backlight_controlled_by_STM32               
#define	BACKLIGHT_PORT     GPIOB							/**< Set GPIO port used for backlight controlling */
#define	BACKLIGHT_PIN         6											/**< Set GPIO pin used for backlight controlling  */
#endif

#define RS_PORT    GPIOA															/**< Set GPIO port used for Register Select line controlling */
#define RS_PIN        9																			/**< Set GPIO pin used for Register Select line controlling  */

#define E_PORT       GPIOC															/**< Set GPIO port used for Enable line controlling */
#define E_PIN          7																			/**< Set GPIO pin used for Enable line controlling  */

#define D4_PORT     GPIOB															/**< Set GPIO port used for Data4 line controlling */
#define D4_PIN         5																			/**< Set GPIO pin used for Data4 line controlling  */

#define D5_PORT     GPIOB															/**< Set GPIO port used for Data5 line controlling */
#define D5_PIN         4																			/**< Set GPIO pin used for Data5 line controlling  */

#define D6_PORT     GPIOB															/**< Set GPIO port used for Data6 line controlling */
#define D6_PIN         10																		/**< Set GPIO pin used for Data6 line controlling  */

#define D7_PORT     GPIOA															/**< Set GPIO port used for Data7 line controlling */
#define D7_PIN         8																			/**< Set GPIO pin used for Data7 line controlling  */

#ifdef _8_bit_mode
#define D0_PORT     GPIOB
#define D0_PIN         6

#define D1_PORT     GPIOA
#define D1_PIN         7

#define D2_PORT     GPIOA
#define D2_PIN         10

#define D3_PORT     GPIOB
#define D3_PIN         3
#endif
//=================================
//               Length of FIFO buffer
//=================================
#define LCD_FIFO_Length  128												/**< Set FIFO length which can be only a power of 2 */

//==============================================
//                     END OF USER MODIFICATIONS
//==============================================

#define LCD_FIFO_Mask  (LCD_FIFO_Length - 1)
static uint16_t LCD_FIFO[ LCD_FIFO_Length ];
static volatile uint16_t LCD_FIFO_Head = 0;
static volatile uint16_t LCD_FIFO_Tail = 0;
static volatile uint16_t LCD_temp_FIFO_Head = 0;

static enum commands {
	heart = 0x00,
	clear_screen = 0x01,
	_4bit_mode_2lines_5x7font = 0x28,
	_8bit_mode_2lines_5x7font = 0x38,
	CGRAM_set = 0x40,
	DDRAM_set = 0x80,
} commands;

void LCDInitialize(void);
void LCDSendCommand(uint8_t cmd);
void LCDSendData(unsigned char data);
void LCDSendText(char *text);
void LCDSend8bitInteger(uint8_t number);
void LCDLocateXY(uint8_t x, uint8_t y);

static void LCDSendHalfByte(uint8_t cmd);
static inline void setPinAsOutput( GPIO_TypeDef * GPIO_port, uint32_t pin );
static inline void enableClockToRegister(GPIO_TypeDef * GPIO_port);

/**
* \}
*/

#endif
