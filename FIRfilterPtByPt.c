#include "FIRfilterPtByPt.h"

/**
* \brief   Function used for initialization FIR filter structure used for filtering
* \param[in]   signalBufferLength    Length of cyclic buffer storing signal
* \param[in]   signalCyclicBuffer[]   Cyclic buffer storing signal
* \param[in]   coeffLength              Number of filter coefficients
* \param[in]   coefficients[]            Filter coefficients
* \return        FIRfilterObject          FIR filter structure
*/
FIRfilterObject CreateFIRfilterObject(uint16_t signalBufferLength, float signalCyclicBuffer[], uint16_t coeffLength, float coefficients[])
{
		FIRfilterObject _FIRfilterObject;
		_FIRfilterObject.coefficients = &coefficients[0];
		_FIRfilterObject.coefficientsLength = coeffLength;
		_FIRfilterObject.signalBuffer = &signalCyclicBuffer[0];
		_FIRfilterObject.signalBufferHead = 0;
		_FIRfilterObject.signalBufferLength = signalBufferLength;
		_FIRfilterObject.signalBufferMask = signalBufferLength - 1;
		
	//======= Zero out signalBuffer values  =======//
		uint16_t i;
		uint16_t counterBy8 = _FIRfilterObject.signalBufferLength >> 3;  //  Division by 8
		uint16_t remainder = _FIRfilterObject.signalBufferLength - ((_FIRfilterObject.signalBufferLength >> 3) << 3);  // % 8
		float* signalBufferPtr = &_FIRfilterObject.signalBuffer[0];
	
		for(i=0; i<counterBy8; i++)
		{
				*signalBufferPtr++ = 0.0f;
				*signalBufferPtr++ = 0.0f;
				*signalBufferPtr++ = 0.0f;
				*signalBufferPtr++ = 0.0f;
				*signalBufferPtr++ = 0.0f;
				*signalBufferPtr++ = 0.0f;
				*signalBufferPtr++ = 0.0f;
				*signalBufferPtr++ = 0.0f;
		}
		
		for(i=0; i<remainder; i++)
		{
				*signalBufferPtr++ = 0.0f;
		}
	//===================================//
		
		return _FIRfilterObject;
}

/**
* \brief   Function filtering input signal point-by-point
* \param[in]     filterObject    Pointer to FIR filter structure
* \param[in]     signal           One sample of input signal
* \param[out]   result            Result of filtering
*/
void FIRfilterPtByPt(FIRfilterObject* filterObject, float signal, float* result)
{
    *result = 0;
    filterObject->signalBuffer[filterObject->signalBufferHead] = signal;
    float* coefficientsPtr;
    coefficientsPtr	= &filterObject->coefficients[0];
    float* signalPtr;
    signalPtr	= &filterObject->signalBuffer[filterObject->signalBufferHead];
    int16_t i;
    int16_t counterBy8;
    int16_t remainder;
	
		if( filterObject->signalBufferHead >=  filterObject->coefficientsLength)
		{
        counterBy8 = filterObject->coefficientsLength >> 3;   //  filterObject->coefficientsLength / 8 
        remainder = filterObject->coefficientsLength - ((filterObject->coefficientsLength >> 3) << 3);  // filterObject->coefficientsLength % 8 
				for(i=0; i<counterBy8; i++)
				{
						*result += *signalPtr-- * *coefficientsPtr++;
						*result += *signalPtr-- * *coefficientsPtr++;
						*result += *signalPtr-- * *coefficientsPtr++;
						*result += *signalPtr-- * *coefficientsPtr++;
						*result += *signalPtr-- * *coefficientsPtr++;
						*result += *signalPtr-- * *coefficientsPtr++;
						*result += *signalPtr-- * *coefficientsPtr++;
						*result += *signalPtr-- * *coefficientsPtr++;
				}
				
				for(i=0; i<remainder; i++)
				{
						*result += *signalPtr-- * *coefficientsPtr++;
				}
		}
		else
		{	
        counterBy8 = filterObject->signalBufferHead >> 3;  //  filterObject->signalBufferHead / 8 
        remainder = filterObject->signalBufferHead - ((filterObject->signalBufferHead >> 3) << 3);  // filterObject->signalBufferHead % 8 
                
				for(i=0; i<counterBy8; i++)
				{
						*result += *signalPtr-- * *coefficientsPtr++;
						*result += *signalPtr-- * *coefficientsPtr++;
						*result += *signalPtr-- * *coefficientsPtr++;
						*result += *signalPtr-- * *coefficientsPtr++;
						*result += *signalPtr-- * *coefficientsPtr++;
						*result += *signalPtr-- * *coefficientsPtr++;
						*result += *signalPtr-- * *coefficientsPtr++;
						*result += *signalPtr-- * *coefficientsPtr++;
				}
				
        for(i=0; i<remainder; i++)
        {
            *result += *signalPtr-- * *coefficientsPtr++;
        }
				
        *result += filterObject->signalBuffer[0] * *coefficientsPtr++;
        signalPtr = &filterObject->signalBuffer[filterObject->signalBufferLength-1];
				
        counterBy8 = filterObject->coefficientsLength - filterObject->signalBufferHead - 1;
        remainder = counterBy8 - ((counterBy8 >> 3) << 3);  // counterBy8 % 8 
        counterBy8 = counterBy8 >> 3;  //  counterBy8 / 8 
				
        for(i=0; i<counterBy8; i++)
        {
            *result += *signalPtr-- * *coefficientsPtr++;
            *result += *signalPtr-- * *coefficientsPtr++;
            *result += *signalPtr-- * *coefficientsPtr++;
            *result += *signalPtr-- * *coefficientsPtr++;
            *result += *signalPtr-- * *coefficientsPtr++;
            *result += *signalPtr-- * *coefficientsPtr++;
            *result += *signalPtr-- * *coefficientsPtr++;
            *result += *signalPtr-- * *coefficientsPtr++;
        }		
				
        for(i=0; i<remainder; i++)
        {
            *result += *signalPtr-- * *coefficientsPtr++;
        }
    }
	
    filterObject->signalBufferHead = (filterObject->signalBufferHead + 1) & filterObject->signalBufferMask;
}
