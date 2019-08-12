#include "IIRfilterPtByPt.h"

/**
* \brief   Function used for initialization IIR 2nd order filter structure used for filtering
* \param[in]   forwardCoeff[3]            Filter forward coefficients
* \param[in]   reverseCoeff[3]            Filter reverse coefficients
* \return        IIR2ndOrderFilterObj    IIR 2nd order filter structure
*/
IIR2ndOrderFilterObj IIR_2ndOrderSignalFilteringPtByPtInitialize(float forwardCoeff[3], float reverseCoeff[3])
{
    IIR2ndOrderFilterObj  IIR2ndOrderFilterObj_;
    IIR2ndOrderFilterObj_.forwardCoeff = forwardCoeff;
    IIR2ndOrderFilterObj_.reverseCoeff = reverseCoeff;
    
    return IIR2ndOrderFilterObj_;
}

/**
* \brief   Function used for initialization IIR 3rd order filter structure used for filtering
* \param[in]   forwardCoeff[4]            Filter forward coefficients
* \param[in]   reverseCoeff[4]            Filter reverse coefficients
* \return        IIR3rdOrderFilterObj     IIR 3rd order filter structure
*/
IIR3rdOrderFilterObj IIR_3rdOrderSignalFilteringPtByPtInitialize(float forwardCoeff[4], float reverseCoeff[4])
{
    IIR3rdOrderFilterObj  IIR3rdOrderFilterObj_;
    IIR3rdOrderFilterObj_.forwardCoeff = forwardCoeff;
    IIR3rdOrderFilterObj_.reverseCoeff = reverseCoeff;
    
    return IIR3rdOrderFilterObj_;
}

/**
* \brief   Function used for initialization IIR 4th order filter structure used for filtering
* \param[in]   forwardCoeff[5]            Filter forward coefficients
* \param[in]   reverseCoeff[5]            Filter reverse coefficients
* \return        IIR4thOrderFilterObj     IIR 4th order filter structure
*/
IIR4thOrderFilterObj IIR_4thOrderSignalFilteringPtByPtInitialize(float forwardCoeff[5], float reverseCoeff[5])
{
    IIR4thOrderFilterObj  IIR4thOrderFilterObj_;
    IIR4thOrderFilterObj_.forwardCoeff = forwardCoeff;
    IIR4thOrderFilterObj_.reverseCoeff = reverseCoeff;
    
    return IIR4thOrderFilterObj_;
}

/**
* \brief   Function for recursive Infinite Impulse Response 2nd order filtering of input signal point-by-point
* \warning This IIR filter implemantation can be used only when first reverse coefficient value == 1
* \param[in]  filterObject       Pointer to structure of 2nd order IIR filter 
* \param[in]  signal               Input signal last sample
* \param[out] result              Output filtered signal sample
*/
void IIR_2ndOrderSignalFilteringPtByPt(IIR2ndOrderFilterObj* filterObject, float signal, float* result)
{
		static float as;           
           
		filterObject->stateBuffer[0] += signal * filterObject->forwardCoeff[0];
    filterObject->stateBuffer[1] += signal * filterObject->forwardCoeff[1];
    filterObject->stateBuffer[2] += signal * filterObject->forwardCoeff[2];

		as = -filterObject->stateBuffer[0];

		filterObject->stateBuffer[1] += as * filterObject->reverseCoeff[1];
    filterObject->stateBuffer[2] += as * filterObject->reverseCoeff[2];

		*result = filterObject->stateBuffer[0];

		filterObject->stateBuffer[0] = filterObject->stateBuffer[1];
    filterObject->stateBuffer[1] = filterObject->stateBuffer[2];
    filterObject->stateBuffer[2] = 0;
}

/**
* \brief   Function for recursive Infinite Impulse Response 3rd order filtering of input signal point-by-point
* \warning This IIR filter implemantation can be used only when first reverse coefficient value == 1
* \param[in]  filterObject       Pointer to structure of 3rd order IIR filter 
* \param[in]  signal               Input signal last sample
* \param[out] result              Output filtered signal sample
*/
void IIR_3rdOrderSignalFilteringPtByPt(IIR3rdOrderFilterObj* filterObject, float signal, float* result)
{
		static float as;           
           
		filterObject->stateBuffer[0] += signal * filterObject->forwardCoeff[0];
    filterObject->stateBuffer[1] += signal * filterObject->forwardCoeff[1];
    filterObject->stateBuffer[2] += signal * filterObject->forwardCoeff[2];
	  filterObject->stateBuffer[3] += signal * filterObject->forwardCoeff[3];

		as = -filterObject->stateBuffer[0];

		filterObject->stateBuffer[1] += as * filterObject->reverseCoeff[1];
    filterObject->stateBuffer[2] += as * filterObject->reverseCoeff[2];
	  filterObject->stateBuffer[3] += as * filterObject->reverseCoeff[3];

		*result = filterObject->stateBuffer[0];

		filterObject->stateBuffer[0] = filterObject->stateBuffer[1];
    filterObject->stateBuffer[1] = filterObject->stateBuffer[2];
    filterObject->stateBuffer[2] = filterObject->stateBuffer[3];
	  filterObject->stateBuffer[3] = 0;
}

/**
* \brief   Function for recursive Infinite Impulse Response 4th order filtering of input signal point-by-point
* \warning This IIR filter implemantation can be used only when first reverse coefficient value == 1
* \param[in]  filterObject       Pointer to structure of 4th order IIR filter 
* \param[in]  signal               Input signal last sample
* \param[out] result              Output filtered signal sample
*/
void IIR_4thOrderSignalFilteringPtByPt(IIR4thOrderFilterObj* filterObject, float signal, float* result)
{
		static float as;           
           
		filterObject->stateBuffer[0] += signal * filterObject->forwardCoeff[0];
    filterObject->stateBuffer[1] += signal * filterObject->forwardCoeff[1];
    filterObject->stateBuffer[2] += signal * filterObject->forwardCoeff[2];
	  filterObject->stateBuffer[3] += signal * filterObject->forwardCoeff[3];
	  filterObject->stateBuffer[4] += signal * filterObject->forwardCoeff[4];

		as = -filterObject->stateBuffer[0];

		filterObject->stateBuffer[1] += as * filterObject->reverseCoeff[1];
    filterObject->stateBuffer[2] += as * filterObject->reverseCoeff[2];
	  filterObject->stateBuffer[3] += as * filterObject->reverseCoeff[3];
	  filterObject->stateBuffer[4] += as * filterObject->reverseCoeff[4];

		*result = filterObject->stateBuffer[0];

		filterObject->stateBuffer[0] = filterObject->stateBuffer[1];
    filterObject->stateBuffer[1] = filterObject->stateBuffer[2];
    filterObject->stateBuffer[2] = filterObject->stateBuffer[3];
	  filterObject->stateBuffer[3] = filterObject->stateBuffer[4];
		filterObject->stateBuffer[4] = 0;
}
