#include "stm32f4xx.h"                  // Device header
#include "Handlers.h"
#include "LCDHD44780NonBlocking.h"
#include "BloodPressureMeterFunctions.h"
#include "PressureSensorABPMANN005PG2A3.h"
#include "FIRbandpass_3_10_Hz_Coeff.h"
#include "FIRfilterPtByPt.h"
#include "IIRfilterPtByPt.h"

/*=================================
Formula for pressure calculation:

pressure = ((Output - 1638) * 5)/(14745 - 1638) 

1 psi = 51.7149326 mmHg
3.480619445 psi = 180 mmHg

10762 = 180 mmHg
//================================*/

//===  Instances of structures and pointers to structures  ===//
pulseWave_  _pulseWave;
pulseWave_* pulseWave = &_pulseWave;

FIRfilterObject _BandpassFIRfilter;
FIRfilterObject* BandpassFIRfilter = &_BandpassFIRfilter;
IIR4thOrderFilterObj _LowpassIIRfilter;
IIR4thOrderFilterObj* LowpassIIRfilter = &_LowpassIIRfilter;

//=== Extern variables enabling calculations and display  ===//
extern volatile uint8_t calculate;
extern volatile uint8_t displayOnLCDevery1sec;
extern volatile uint8_t displayOnLCDevery250milisec;

//===========  FIR filter coefficients  ===========//
extern float FIRbandpass_3_10_Hz_Coeff[166];

//==== Circular buffers to pass into FIRfilterObject ===//
float signalBufferForBandpassFIRfilter[256];


int main()
{
		RCCClockConfiguration();
		/* FPU initialization */
		SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */

		PumpValveButtonConfiguration();
		LCDInitialize();
		TimersInitialization();

		LCDLocateXY(0,0);
		LCDSendText("Put cuff on arm ");
		LCDLocateXY(0,1);
		LCDSendText("press  button   ");
		PressureSensorInitialize();
		
    //====== Initialization of pointers ======//
		float LPFiltered;
		float BPFiltered;
		float* lowPassFiltered;	
		float* bandPassFiltered;
		lowPassFiltered = &LPFiltered;
		bandPassFiltered = &BPFiltered; 
		//==============================//
		
		volatile float previousValue, derivative, previousDerivative;                // Variables for determining change of 1st derivative sign of lowpass filtered output
		volatile float previousValueBP, derivativeBP, previousDerivativeBP;  // Variables for determining change of 1st derivative sign of bandpass filtered output

		_BandpassFIRfilter = CreateFIRfilterObject(256, signalBufferForBandpassFIRfilter, 166, FIRbandpass_3_10_Hz_Coeff);
		_LowpassIIRfilter = IIR_4thOrderSignalFilteringPtByPtInitialize(IIRforwardCoeffLowpass_20Hz, IIRreverseCoeffLowpass_20Hz);

		unsigned int BPfilterDelay = (unsigned int) (_BandpassFIRfilter.coefficientsLength - 1) / 2;     //  Length of bandpass FIR filter group delay
		
		volatile uint8_t displayHeart = 0;      //  Variable controlling display of heart character on LCD
		volatile uint8_t clearHeart = 0;         //  Variable controlling clear of heart character on LCD
		volatile uint8_t calculateMAP = 0;    //  Variable controlling start of MAP evaluation
		

	while(1)
	{
			if(calculate)
			{
					if( pressureArrayHead != pressureArrayTail) 
					{		
						//=======================   Filtration of data from pressure sensor   ========================//
						  IIR_4thOrderSignalFilteringPtByPt(LowpassIIRfilter, pressureArray[pressureArrayTail], lowPassFiltered);
							FIRfilterPtByPt(BandpassFIRfilter, pressureArray[pressureArrayTail], bandPassFiltered); 
						
						//=======================  Update of buffers and variables values  ========================//
							pulseWave->currentPressure = ((*lowPassFiltered- 1638.0f) * 5.0f) / (14745.0f - 1638.0f) * 52.0f; 
							pulseWave->numberOfSamples++;
							Last128SamplesBufferHead = (Last128SamplesBufferHead+1) & 127; 
						  Last128SamplesBuffer[Last128SamplesBufferHead] = *lowPassFiltered;
						  pressureArrayTail = (pressureArrayTail + 1) & pressureArrayMask;

						
							//=======  New derivative value calculation  =======//			
							derivative = *lowPassFiltered - previousValue;
							derivativeBP = *bandPassFiltered - previousValueBP;
						
						  //======================   Peak detection   ======================//
							if( ((derivative * previousDerivative) < 0) && ((derivative - previousDerivative) < 0) ) 
							{ 
									if((pulseWave->numberOfSamples - pulseWave->previousPeakXcoordinate ) > 64)  // if two Peaks distance > approximate 250 ms
									{ 
											displayHeart = 1; 
											pulseWave->previousPeakXcoordinate = pulseWave->numberOfSamples;
									}
									
									if( ! pulseWave->systolicPressure ) 
									{ 
											calculateMAP = 1;    // if Systolic peak detected start evaluating MAP
											pulseWave->systolicPressure = pulseWave->currentPressure;  
									}
							}					

							//========================   MAP calculation   =========================//
							if( calculateMAP )
							{
									if( ((derivativeBP * previousDerivativeBP) < 0) && ((derivativeBP - previousDerivativeBP) < 0) ) 
									{ 
											if( *bandPassFiltered > pulseWave->workingMAPpressure ) 
											{ 
													pulseWave->MAPpressure = ((Last128SamplesBuffer[(Last128SamplesBufferHead - BPfilterDelay) & 127] - 1638.0f) * 5.0f) / (14745.0f - 1638.0f) * 52.0f; 
													pulseWave->workingMAPpressure = *bandPassFiltered;
											}
									}
							}
							
							//============  Variables update  =============//
							previousDerivative = derivative;
							previousValue = *lowPassFiltered;	
							previousDerivativeBP = derivativeBP;
							previousValueBP = *bandPassFiltered;	 
								
							//============  Ending calculations  ==============//
							if( pulseWave->currentPressure < 50 ) 
							{ 	
									LCDLocateXY(0,0);
									LCDSendText("SYS:");
									LCDLocateXY(7,0);
									LCDSendData(' ');
									LCDLocateXY(8,0);
									LCDSendText("DIA:");
								
									calculate = 0; 
									calculateMAP = 0;
									TIM4->CR1 &= ~TIM_CR1_CEN;  // Turn off Timer controlling displaying on LCD
									displayOnLCDevery1sec = 0;
									displayOnLCDevery250milisec = 0;
									pulseWave->diastolicPressure = ( pulseWave->MAPpressure * 3 - pulseWave->systolicPressure ) / 2 ;  
								
									LCDLocateXY(4,0); 
									LCDSend8bitInteger((uint8_t) pulseWave->systolicPressure);
						
									LCDLocateXY(12,0);
									LCDSend8bitInteger((uint8_t) pulseWave->diastolicPressure);
				
									LCDLocateXY(0,1);
									LCDSendText("MAP:  ");
				
									LCDLocateXY(6,1);
									LCDSend8bitInteger((uint8_t) pulseWave->MAPpressure);
				
									LCDLocateXY(12,1);
									LCDSendText("    ");
							}
				}			
		}
		else 
		{
				if( pressureArrayHead != pressureArrayTail) 
				{
						//=======================   Filtration of data from pressure sensor   ========================//
              IIR_4thOrderSignalFilteringPtByPt(LowpassIIRfilter, pressureArray[pressureArrayTail], lowPassFiltered);					
							FIRfilterPtByPt(BandpassFIRfilter, pressureArray[pressureArrayTail], bandPassFiltered); 

					 //=======================  Update of buffers and variables values  ========================//
							Last128SamplesBufferHead = (Last128SamplesBufferHead+1) & 127; 
							Last128SamplesBuffer[Last128SamplesBufferHead] = *lowPassFiltered;
					    pressureArrayTail = (pressureArrayTail + 1) & pressureArrayMask;

						  pulseWave->currentPressure = ((*lowPassFiltered- 1638.0f) * 5.0f) / (14745.0f - 1638.0f) * 52.0f; 
						  previousDerivative = *lowPassFiltered - previousValue; 
					  	previousDerivativeBP = *bandPassFiltered - previousValueBP; 
					  	previousValue = *lowPassFiltered; 
					  	previousValueBP = *bandPassFiltered;
				}
		}

		if(displayOnLCDevery250milisec) 
		{
				if(clearHeart) 
				{
						LCDLocateXY(8,0);
						LCDSendData(' ');
						clearHeart = 0;
				}
				
				if(displayHeart) 
				{
						LCDLocateXY(8,0);
						LCDSendData(heart);
						displayHeart = 0;
						clearHeart = 1;
				}
				
				displayOnLCDevery250milisec = 0;
		}
		
		if(displayOnLCDevery1sec)
		{
				LCDLocateXY(12,1);
				LCDSend8bitInteger((uint8_t) pulseWave->currentPressure);
			
				displayOnLCDevery1sec = 0;
		}
	}
}
