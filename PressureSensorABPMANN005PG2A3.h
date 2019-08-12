#ifndef __PressureSensorABPMANN005PG2A3_H__
#define __PressureSensorABPMANN005PG2A3_H__

#include "stm32f4xx.h"                 
#include "STM32F446xx.h"

/**
* \defgroup PressureSensorABPMANN005PG2A3   Pressure Sensor ABPMANN005PG2A3
* \addtogroup  PressureSensorABPMANN005PG2A3
*
* \brief      Module used for initialization I2C1 bus used for communication with a piezoresistive 
*              silicon pressure sensor from Honeywell Basic Amplified ABP Series. Calibrated output 
*              values for pressure are updated at approximately 1 kHz for analog and 2 kHz for digital.
*              According to Honeywell Technical note, Honeywell's digital output pressure sensors are designed to
*              work as slaves and will therefore only respond to requests from a master device. Following the address 
* 						 and read bit from the master, Honeywell digital output pressure sensors are designed to output up to 
*	  					 4 bytes of data, depending on the sensor options and the needs of the application. Sensor used in project
*              gives two data bytes that are the compensated pressure output, along with sensor status bits.
* @image html  TwoDataByteReadout.png
*              For Honeywell digital output pressure sensors, the output of the device can be expressed by the transfer 
*              function of the device as shown in following equation:
*@image html  PressureOutputFunction.png
*              Example calculations below:
*@image html  ExampleCalculaions.png
* \{
*/
//==============================================
//       SECTION OF USER MODIFICATION AND SETTINGS
//==============================================
#define I2C_SDA_PORT			GPIOB                  /**< Set GPIO port used for I2C1 SDA line controlling */
#define I2C_SDA_PIN				9                          /**< Set GPIO pin used for I2C1 SDA line controlling  */

#define I2C_SCL_PORT			GPIOB                 /**< Set GPIO port used for I2C1 SCL line controlling */
#define I2C_SCL_PIN					8                         /**< Set GPIO pin used for I2C1 SCL line controlling  */

#define APBClockSpeed_MHz     42               /**< Set Clock value on APB1 BUS */
#define CCR_VALUE		250                            /**< Set calculated value of CCR[11:0] in CCR register */
#define TRISE_VALUE		51                            /**< Set value of TRISE[5:0] in TRISE register */
//==============================================
//                     END OF USER MODIFICATIONS
//==============================================

void PressureSensorInitialize(void);
static inline void enableClockToRegister(GPIO_TypeDef * GPIO_port);

/**
* \}
*/

#endif
