#include "FIRfilterPtByPt.h"

/**
* \brief   Function used for initialization FIR filter structure used for filtering
* \param[in]   signalBufferLength    Length of cyclic buffer storing signal
* \param[in]   signalCyclicBuffer[]   Cyclic buffer storing signal
* \param[in]   coeffLength              Number of filter coefficients
* \param[in]   coefficients[]            Filter coefficients
*/
FIRfilterObject CreateFIRfilterObject(unsigned int signalBufferLength, double signalCyclicBuffer[], unsigned int coeffLength, double coefficients[])
{
		FIRfilterObject _FIRfilterObject;
		_FIRfilterObject.coefficients = &coefficients[0];
		_FIRfilterObject.coefficientsLength = coeffLength;
		_FIRfilterObject.signalBuffer = &signalCyclicBuffer[0];
		_FIRfilterObject.signalBufferHead = 0;
        _FIRfilterObject.signalBufferLength = signalBufferLength;
		_FIRfilterObject.signalBufferMask = signalBufferLength - 1;
		
		return _FIRfilterObject;
}

/**
* \brief   Function filtering input signal point-by-point
* \param[in]     filterObject    Pointer to FIR filter structure
* \param[in]     signal           One sample of input signal
* \param[out]    result            Result of filtering
*/
void FIRfilterPtByPt(FIRfilterObject* filterObject, double signal, double* result)
{
        *result = 0;
        filterObject->signalBuffer[filterObject->signalBufferHead] = signal;
		double* coefficientsPtr;
		coefficientsPtr	= &filterObject->coefficients[0];
		double* signalPtr;
		signalPtr	= &filterObject->signalBuffer[filterObject->signalBufferHead];
        int i;
        int counterBy8;
		int remainder;
	
		if( filterObject->signalBufferHead >=  filterObject->coefficientsLength)
		{
            counterBy8 = filterObject->coefficientsLength >> 3;
            remainder = filterObject->coefficientsLength - ((filterObject->coefficientsLength >> 3) << 3);
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
				
				if(remainder)
				{
						for(i=0; i<remainder; i++)
						{
								*result += *signalPtr-- * *coefficientsPtr++;
						}
				}
		}
		else
		{	
                counterBy8 = filterObject->signalBufferHead >> 3;
                remainder = filterObject->signalBufferHead - ((filterObject->signalBufferHead >> 3) << 3);
                
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
                remainder = counterBy8 - ((counterBy8 >> 3) << 3);
				counterBy8 = counterBy8 >> 3;
				
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
