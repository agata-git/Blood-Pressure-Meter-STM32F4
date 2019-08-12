#ifndef __BloodPressureMeterFunctions__H__
#define __BloodPressureMeterFunctions__H__

#include "stm32f4xx.h"                  // Device header
#include "stm32f446xx.h"
#include <stdint.h>

/**
* \defgroup   BloodPressureMeterFunctions  Blood Pressure Meter Functions
* \addtogroup  BloodPressureMeterFunctions
*
* \brief   Module containing various functions used in Blood Pressure Meter project.
* \{
*/

//==============================================
//       SECTION OF USER MODIFICATION AND SETTINGS
//==============================================
#define	PUMP_PORT	GPIOC                    /**< Set GPIO port used for Pump controlling */
#define	PUMP_PIN	1                                /**< Set GPIO pin used for Pump controlling  */
#define	VALVE_PORT		GPIOB                 /**< Set GPIO port used for Valve controlling */
#define	VALVE_PIN	0                             /**< Set GPIO pin used for Valve controlling */
#define BUTTON_PORT	GPIOA                /**< Set GPIO port used for Button controlling */
#define BUTTON_PIN	 	0                          /**< Set GPIO pin used for Button controlling  */
//==============================================
//                     END OF USER MODIFICATIONS
//==============================================


extern float IIRforwardCoeffLowpass_20Hz[5];  /**< Array containing forward coefficients of IIR lowpass filter with 20 Hz cut-off frequency */
extern float IIRreverseCoeffLowpass_20Hz[5];  /**< Array containing reverse coefficients of IIR lowpass filter with 20 Hz cut-off frequency */

extern uint16_t pressureArray[32];                  /**< FIFO cyclic buffer storing data from pressure sensor */
extern volatile uint16_t pressureArrayHead;     /**< Variable as index of the newest sample in pressureArray[32] buffer  */
extern volatile uint16_t pressureArrayTail;       /**< Variable as index of the last sample in pressureArray[32] buffer  */
#define		pressureArrayMask		31                 /** Bit mask used with pressureArray buffer */

extern float Last128SamplesBuffer[128];          /**< Buffer storing last 128 samples of pressure signal */
extern uint16_t Last128SamplesBufferHead;    /**< Buffer pointing on the newest sample in Last128SamplesBuffer[128] buffer  */

/**
* \brief Struct contains pressure parameters of blood pulse wave
*/
typedef struct {
			volatile float systolicPressure;
			volatile float diastolicPressure;
			volatile float MAPpressure;
			volatile float currentPressure;
			volatile float workingMAPpressure;
	    volatile uint32_t numberOfSamples;
			volatile uint32_t previousPeakXcoordinate;
} pulseWave_;


void RCCClockConfiguration(void);
void PumpValveButtonConfiguration(void);

static inline void enableClockToRegister(GPIO_TypeDef * GPIO_port);
static inline void setPinAsOutput( GPIO_TypeDef * GPIO_port, uint32_t pin );
static inline void setPinAsInput( GPIO_TypeDef * GPIO_port, uint32_t pin );

/**
* \}
*/

#endif
